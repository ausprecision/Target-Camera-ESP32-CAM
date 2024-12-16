#include <ArduinoBLE.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <Ticker.h>
#include <AsyncUDP.h>

#include "html.h"
#include "lib_battery.h"
#include "lib_spiffs.h"
#include "lib_udp.h"
#include "lib_term.h"

#include "esp_camera.h"
#include "esp_wifi.h"

#include "FS.h"
#include "SPIFFS.h"

#include "ESPTelnet.h"
#include "printHelpers.h"
#include <uptime.h>
#include <uptime_formatter.h>
#include <string_asukiaaa.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

#define LED_GRN  14
#define LED_YEL  15
#define LED_RED  13
#define LED_BLU  2

int ledState = LOW;
uint8_t otaDone = 0;
int res = 0;

String diagBattery;
float  diagTempCelsius;
String diagUptime;
bool telnetRunning = false;

String diagWiFiCHAN = "4";
String diagWiFiSSID = "apcamera";
String diagWiFiPASS = "tspass2000";
String diagWiFiPOWR = "10";
String diagWiFiPROT = "";

String diagCfgRES = "FRAMESIZE_UXGA";
String diagCfgXCLK = "8";
String diagCfgAEC2 = "1";

String diagCPU = "240";

String diagVFLIP = "1";
String diagHFLIP = "1";
String diagQUAL = "12";     //10-63 lower number means higher quality

WebServer server(8090);
WiFiUDP udpListener;
char packetBuffer[255];

Ticker tkSecond;
Ticker tkBlink;
Ticker tkUdpSend;

IPAddress temp_IP(192,168,4,255);
IPAddress local_IP(192,168,4,255);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

void startCameraServer();
void setupLedFlash(int pin);

ESPTelnet telnet;
IPAddress ip;
uint16_t  port = 23;

void everySecond(){ if (otaDone > 1) { Serial.printf("ota: %d%%\n", otaDone); } }
void timerBlink() { if (ledState == LOW) { ledState = HIGH; } else { ledState = LOW; } digitalWrite(LED_BLU, ledState); } 

void udpSend() { 
  uptime::calculateUptime();
  diagUptime = string_asukiaaa::padStart(String(uptime::getHours()), 2, '0') + "h:" + string_asukiaaa::padStart(String(uptime::getMinutes()), 2, '0') + "m:" + string_asukiaaa::padStart(String(uptime::getSeconds()), 2, '0') + "s";
  String udpMessage = "[ESP32-CAM] Uptime: " + diagUptime + " | Res: " + diagCfgRES.substring(10) + " | Clients: " + String(WiFi.softAPgetStationNum()) + " | Batt ADC: " + diagBattery + " (PWR ON)";
  udpMessage = udpMessage + " | CPU: " + diagCPU + "Mhz | " + " WiFi Prot: " + diagWiFiPROT + " | WiFi CH: " + diagWiFiCHAN + " | WiFi TX: " + diagWiFiPOWR.substring(11) + " | XCLK: " + diagCfgXCLK + " | AEC2: " + diagCfgAEC2;
  udpMessage = udpMessage + " | HF: " + diagHFLIP + " | VF: " + diagVFLIP + " | QUAL: " + diagQUAL;
  Serial.print("udpserver_tx: "); udpSendMessage(temp_IP,udpMessage,2090); Serial.println(udpMessage); 
  }

void udpSendConfig() { String udpMessageConfig = "[WIRELESS] | " + diagWiFiSSID + " | " + diagWiFiPASS; Serial.print("udpserver_tx: "); udpSendMessage(temp_IP,udpMessageConfig,2090); Serial.println(udpMessageConfig); }

// https://github.com/LennartHennigs/ESPTelnet
void setupTelnet() {  
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);
  telnet.onInputReceived(onTelnetInput);
  Serial.print("telnetserver: "); if (telnet.begin(port)) { Serial.println("running");  }
  telnetRunning = true;
}

void onTelnetConnect(String ip)           { telnet.println("Starting AP-DOS..."); telnet.println(""); telnet.println(""); telnet.print("C:\\>"); }
void onTelnetDisconnect(String ip)        { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" disconnected"); }
void onTelnetReconnect(String ip)         { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" reconnected"); telnet.println(""); telnet.println("Reconnected to AP-DOS Version 1.00"); telnet.println(""); telnet.println("");  telnet.print("C:\\>"); }
void onTelnetConnectionAttempt(String ip) { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" tried to connect"); }

void onTelnetInput(String str) {
  sensor_t *s = esp_camera_sensor_get(); 
  Serial.print("telnetserver: "); Serial.println(str);
  if (str == "ver")                 { cmd_ver(telnet); } 
  else if (str == "vol")            { cmd_vol(telnet); }
  else if (str == "dir")            { cmd_dir(telnet); }
  else if (str == "del")            { telnet.println("Required parameter missing"); telnet.println(""); telnet.print("C:\\>"); }
  else if (str == "reboot")         { telnet.println("Rebooting..."); telnet.println(""); telnet.print("C:\\>"); delay(2000); ESP.restart(); }
  else if (str == "exit")           { telnet.println("Disconnecting from AP-DOS..."); telnet.disconnectClient(); } 
  else if (str == "help")           { cmd_help(telnet);} 
  else if (str.indexOf("del ") >=0) { if (SPIFFS.remove(String("/" + str.substring(4)))) {  telnet.println(""); telnet.print("C:\\>"); } else { telnet.println("File not found"); telnet.println(""); telnet.print("C:\\>"); } }

  else if (str == "camera set res vga") { res = s->set_framesize(s, FRAMESIZE_VGA);  if (res == 0) { diagCfgRES = "FRAMESIZE_VGA";  receivecfg("/cfg-res.ini","FRAMESIZE_VGA",  "camera: set FRAMESIZE_VGA (OK!)");  tel_ok(telnet); } else { tel_fail(telnet,"camera: set FRAMESIZE_VGA (FAILED!)");  } }  
  else if (str == "camera set res svga"){ res = s->set_framesize(s, FRAMESIZE_SVGA); if (res == 0) { diagCfgRES = "FRAMESIZE_SVGA"; receivecfg("/cfg-res.ini","FRAMESIZE_SVGA", "camera: set FRAMESIZE_SVGA (OK!)"); tel_ok(telnet); } else { tel_fail(telnet,"camera: set FRAMESIZE_SVGA (FAILED!)"); } }  
  else if (str == "camera set res xga") { res = s->set_framesize(s, FRAMESIZE_XGA);  if (res == 0) { diagCfgRES = "FRAMESIZE_XGA";  receivecfg("/cfg-res.ini","FRAMESIZE_XGA",  "camera: set FRAMESIZE_XGA (OK!)");  tel_ok(telnet); } else { tel_fail(telnet,"camera: set FRAMESIZE_XGA (FAILED!)");  } }  
  else if (str == "camera set res hd")  { res = s->set_framesize(s, FRAMESIZE_HD);   if (res == 0) { diagCfgRES = "FRAMESIZE_HD";   receivecfg("/cfg-res.ini","FRAMESIZE_HD",   "camera: set FRAMESIZE_HD (OK!)");   tel_ok(telnet); } else { tel_fail(telnet,"camera: set FRAMESIZE_HD (FAILED!)");   } }  
  else if (str == "camera set res sxga"){ res = s->set_framesize(s, FRAMESIZE_SXGA); if (res == 0) { diagCfgRES = "FRAMESIZE_SXGA"; receivecfg("/cfg-res.ini","FRAMESIZE_SXGA", "camera: set FRAMESIZE_SXGA (OK!)"); tel_ok(telnet); } else { tel_fail(telnet,"camera: set FRAMESIZE_SXGA (FAILED!)"); } }  
  else if (str == "camera set res uxga"){ res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { diagCfgRES = "FRAMESIZE_UXGA"; receivecfg("/cfg-res.ini","FRAMESIZE_UXGA", "camera: set FRAMESIZE_UXGA (OK!)"); tel_ok(telnet); } else { tel_fail(telnet,"camera: set FRAMESIZE_UXGA (FAILED!)"); } }  

  else if (str == "camera set aec2 on") { res = s->set_aec2(s, 1); if (res == 0) { diagCfgAEC2 = "1"; receivecfg("/camera/cfg-aec2.txt","1","camera: set_aec2 on (OK!)");  tel_ok(telnet); } else { tel_fail(telnet,"camera: set_aec2 (FAILED!)"); } }  
  else if (str == "camera set aec2 off"){ res = s->set_aec2(s, 0); if (res == 0) { diagCfgAEC2 = "0"; receivecfg("/camera/cfg-aec2.txt","0","camera: set_aec2 off (OK!)"); tel_ok(telnet); } else { tel_fail(telnet,"camera: set_aec2 (FAILED!)"); } }  
  else if (str.indexOf("camera set xclk ") >=0) { res = s->set_xclk(s, LEDC_TIMER_0, str.substring(16).toInt()); if (res == 0) { diagCfgXCLK = str.substring(16).toInt(); telnet.println(""); receivecfg("/camera/cfg-xclx.txt",str.substring(16),"camera: set_xclk " + str.substring(16) + " (OK!)"); tel_ok(telnet); } else { tel_fail(telnet,"camera: set_xclk (FAILED!)"); } }  

  else if(str == "wireless set power 1") { telnet.println(""); telnet.print("wireless: set_tx_power 5.0 dBm = ");  diagWiFiPOWR = "WIFI_POWER_5dBm";    telnet.print(WiFi.setTxPower(WIFI_POWER_5dBm));    telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_5dBm", ""   ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 2") { telnet.println(""); telnet.print("wireless: set_tx_power 7.0 dBm = ");  diagWiFiPOWR = "WIFI_POWER_7dBm";    telnet.print(WiFi.setTxPower(WIFI_POWER_7dBm));    telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_7dBm", ""   ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 3") { telnet.println(""); telnet.print("wireless: set_tx_power 8.5 dBm = ");  diagWiFiPOWR = "WIFI_POWER_8_5dBm";  telnet.print(WiFi.setTxPower(WIFI_POWER_8_5dBm));  telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_8_5dBm", "" ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 4") { telnet.println(""); telnet.print("wireless: set_tx_power 11.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_11dBm";   telnet.print(WiFi.setTxPower(WIFI_POWER_11dBm));   telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_11dBm", ""  ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 5") { telnet.println(""); telnet.print("wireless: set_tx_power 13.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_13dBm";   telnet.print(WiFi.setTxPower(WIFI_POWER_13dBm));   telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_13dBm", ""  ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 6") { telnet.println(""); telnet.print("wireless: set_tx_power 15.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_15dBm";   telnet.print(WiFi.setTxPower(WIFI_POWER_15dBm));   telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_15dBm", ""  ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 7") { telnet.println(""); telnet.print("wireless: set_tx_power 17.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_17dBm";   telnet.print(WiFi.setTxPower(WIFI_POWER_17dBm));   telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_17dBm", ""  ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 8") { telnet.println(""); telnet.print("wireless: set_tx_power 18.5 dBm = "); diagWiFiPOWR = "WIFI_POWER_18_5dBm"; telnet.print(WiFi.setTxPower(WIFI_POWER_18_5dBm)); telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_18dBm", ""  ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 9") { telnet.println(""); telnet.print("wireless: set_tx_power 19.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_19dBm";   telnet.print(WiFi.setTxPower(WIFI_POWER_19dBm));   telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_19dBm", ""  ); telnet.println(""); telnet.print("C:\\>"); }
  else if(str == "wireless set power 10"){ telnet.println(""); telnet.print("wireless: set_tx_power 19.5 dBm = "); diagWiFiPOWR = "WIFI_POWER_19_5dBm"; telnet.print(WiFi.setTxPower(WIFI_POWER_19_5dBm)); telnet.print(" - TX_POWER  = "); telnet.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_19_5dBm", ""); telnet.println(""); telnet.print("C:\\>"); }

  else if(str == "camera set hflip on")  { res = s->set_hmirror(s, 1); if (res == 0) { diagHFLIP = "1"; receivecfg("/cfg-hflp.ini","1", "camera: set_hflip on (OK!)");  tel_ok(telnet); } else { tel_fail(telnet, "camera: set_hflip (FAILED!)"); } }
  else if(str == "camera set hflip off") { res = s->set_hmirror(s, 0); if (res == 0) { diagHFLIP = "0"; receivecfg("/cfg-hflp.ini","0", "camera: set_hflip off (OK!)"); tel_ok(telnet); } else { tel_fail(telnet, "camera: set_hflip (FAILED!)"); } }
  else if(str == "camera set vflip on")  { res = s->set_vflip(s, 1);   if (res == 0) { diagVFLIP = "1"; receivecfg("/cfg-vflp.ini","1", "camera: set_vflip on (OK!)");  tel_ok(telnet); } else { tel_fail(telnet, "camera: set_vflip (FAILED!)"); } }
  else if(str == "camera set vflip off") { res = s->set_vflip(s, 0);   if (res == 0) { diagVFLIP = "0"; receivecfg("/cfg-vflp.ini","0", "camera: set_vflip off (OK!)"); tel_ok(telnet); } else { tel_fail(telnet, "camera: set_vflip (FAILED!)"); } }

  else  if(str == "cpu set 240") { if (setCpuFrequencyMhz(240) == true) { delay(200); diagCPU = "240"; telnet.println(""); telnet.print("esp32-cam: CPU Frequency: "); telnet.print(getCpuFrequencyMhz()); telnet.println(" MHz (OK!)"); telnet.println(""); telnet.print("C:\\>"); receivecfg("/cpu.ini","240", ""); } else { tel_fail(telnet, "esp32-cam: set_cpu (FAILED!)"); } }
  else  if(str == "cpu set 160") { if (setCpuFrequencyMhz(160) == true) { delay(200); diagCPU = "160"; telnet.println(""); telnet.print("esp32-cam: CPU Frequency: "); telnet.print(getCpuFrequencyMhz()); telnet.println(" MHz (OK!)"); telnet.println(""); telnet.print("C:\\>"); receivecfg("/cpu.ini","160", ""); } else { tel_fail(telnet, "esp32-cam: set_cpu (FAILED!)"); } }
  else  if(str == "cpu set 80")  { if (setCpuFrequencyMhz(80) == true)  { delay(200); diagCPU = "80";  telnet.println(""); telnet.print("esp32-cam: CPU Frequency: "); telnet.print(getCpuFrequencyMhz()); telnet.println(" MHz (OK!)"); telnet.println(""); telnet.print("C:\\>"); receivecfg("/cpu.ini","80", "");  } else { tel_fail(telnet, "esp32-cam: set_cpu (FAILED!)"); } }

  else if(str.indexOf("wireless get config")>=0)   { Serial.println("wireless: sending wireless config via UDP"); udpSendConfig(); }
  else if(str.indexOf("wireless set channel") >=0) { diagWiFiCHAN = str.substring(21); receivecfg("/wifichan.ini", str.substring(21), "wireless: set_chan (OK!) Reboot Required"); tel_ok_msg(telnet, "wireless: set_chan: " + str.substring(21) + " (OK!) Reboot Required"); }
  else if(str.indexOf("wireless set name")    >=0) { diagWiFiSSID = str.substring(18); receivecfg("/wifissid.ini", str.substring(18), "wireless: set_ssid (OK!) Reboot Required"); tel_ok_msg(telnet, "wireless: set_ssid: " + str.substring(18) + " (OK!) Reboot Required"); }
  else if(str.indexOf("wireless set password")>=0) { diagWiFiPASS = str.substring(22); receivecfg("/wifipass.ini", str.substring(22), "wireless: set_pass (OK!) Reboot Required"); tel_ok_msg(telnet, "wireless: set_pass: " + str.substring(22) + " (OK!) Reboot Required"); }

  else if(str.indexOf("camera set quality") >=0)  { res = s->set_quality(s, str.substring(19).toInt()); diagQUAL = str.substring(19).toInt(); if (res == 0) { tel_ok_msg(telnet, "camera: set_qual: " + str.substring(19) + " (OK!)"); receivecfg("/camera/cfg-qual.txt",str.substring(19),"cameras: set_qual " + str.substring(19) + " (OK!)"); } else { tel_fail(telnet,"camera: set_qual (FAILED!)"); } }  

  else if (str == "")               { telnet.println(""); telnet.print("C:\\>"); } 
  else                              { telnet.println(""); telnet.println("Bad command or file name"); telnet.println(""); telnet.print("C:\\>"); }
}

void webServerInit() {
  server.on("/update", HTTP_POST, []() { handleUpdateEnd(); }, []() { handleUpdate(); } );
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.onNotFound([]() { server.send(200, "text/html", indexHtml); });
  server.begin();
  Serial.printf("http_service: %s\n", WiFi.softAPIP().toString().c_str());
}                

void handleUpdateEnd()  { server.sendHeader("Connection", "close"); if (Update.hasError()) { server.send(502, "text/plain", Update.errorString()); } else { server.sendHeader("Refresh", "10"); server.sendHeader("Location", "/"); server.send(307); delay(2000); ESP.restart(); delay(200); } }
void handleUpdate()     { size_t fsize = UPDATE_SIZE_UNKNOWN; if (server.hasArg("size")) { fsize = server.arg("size").toInt(); } HTTPUpload &upload = server.upload(); if (upload.status == UPLOAD_FILE_START) { Serial.printf("Receiving Update: %s, Size: %d\n", upload.filename.c_str(), fsize);    if (!Update.begin(fsize)) { otaDone = 0; Update.printError(Serial); }    } else if (upload.status == UPLOAD_FILE_WRITE) {    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {      Update.printError(Serial);    } else {      otaDone = 100 * Update.progress() / Update.size();    }  } else if (upload.status == UPLOAD_FILE_END) {    if (Update.end(true)) {      Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize); delay(200);   } else {      Serial.printf("%s\n", Update.errorString()); otaDone = 0; }  }}
void receivecfg(String filename, String cfgoption, String serialoutput) { File file; file = SPIFFS.open(filename, FILE_WRITE); file.println(cfgoption); file.close(); if(serialoutput.length() >= 1) {  Serial.println(serialoutput); } }

void setup() {
  Serial.begin(9600); Serial.println(""); Serial.println(""); uptime::calculateUptime(); diagUptime = String(uptime::getHours()) + "H:" + String(uptime::getMinutes()) + "M:" + String(uptime::getSeconds()) + "S";

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;  // CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 2;   // was 1 initially
  config.grab_mode = CAMERA_GRAB_LATEST;

  // camera init
  esp_err_t err = esp_camera_init(&config); if (err != ESP_OK) { Serial.printf("Camera init failed with error 0x%x", err);  return;  }
  sensor_t *s = esp_camera_sensor_get();

  spiffs_init(); listDir(SPIFFS, "/", 1);     delay(10);
  
  diagCPU      = readConfig("/cpu.ini");      delay(10);
  
  diagWiFiCHAN = readConfig("/wifichan.ini"); delay(10);
  diagWiFiPASS = readConfig("/wifipass.ini"); delay(10);
  diagWiFiSSID = readConfig("/wifissid.ini"); delay(10);
  diagWiFiPOWR = readConfig("/wifitxpw.ini"); delay(10);

  diagCfgRES   = readConfig("/cfg-res.ini");  delay(10);
  diagHFLIP    = readConfig("/cfg-hflp.ini"); delay(10);
  diagVFLIP    = readConfig("/cfg-vflp.ini"); delay(10);
  diagQUAL     = readConfig("/cfg-qual.ini"); delay(10);

  diagCPU.trim(); diagCfgRES.trim(); diagWiFiCHAN.trim(); diagWiFiSSID.trim(); diagWiFiPASS.trim(); diagWiFiPOWR.trim(); diagCfgRES.trim(); diagHFLIP.trim(); diagVFLIP.trim(); diagQUAL.trim();

  if      (diagCfgRES == "FRAMESIZE_UXGA"){ s->set_framesize(s, FRAMESIZE_UXGA); }
  else if (diagCfgRES == "FRAMESIZE_SXGA"){ s->set_framesize(s, FRAMESIZE_SXGA); }
  else if (diagCfgRES == "FRAMESIZE_HD")  { s->set_framesize(s, FRAMESIZE_HD);   }
  else if (diagCfgRES == "FRAMESIZE_XGA") { s->set_framesize(s, FRAMESIZE_XGA); }
  else if (diagCfgRES == "FRAMESIZE_SVGA"){ s->set_framesize(s, FRAMESIZE_SVGA); }
  else if (diagCfgRES == "FRAMESIZE_VGA") { s->set_framesize(s, FRAMESIZE_VGA);  }
  else    { s->set_framesize(s, FRAMESIZE_UXGA); }

  delay(200);

  Serial.print("esp32-cam: Setting CPU Speed to ");    Serial.println(diagCPU);  setCpuFrequencyMhz(diagCPU.toInt());  delay(10);
  Serial.print("esp32-cam: XTAL Crystal Frequency: "); Serial.print(getXtalFrequencyMhz());  Serial.println(" MHz");   delay(10);
  Serial.print("esp32-cam: CPU Frequency: ");          Serial.print(getCpuFrequencyMhz());   Serial.println(" MHz");   delay(10);
  Serial.print("esp32-cam: APB Bus Frequency: ");      Serial.print(getApbFrequency());      Serial.println(" Hz");    delay(10);

  Serial.print("wireless: SSID = "); Serial.println(diagWiFiSSID);
  Serial.print("wireless: PASS = "); Serial.println(diagWiFiPASS);

  s->set_xclk(s, LEDC_TIMER_0, diagCfgXCLK.toInt());
  s->set_aec2(s, diagCfgAEC2.toInt());
  s->set_vflip(s, diagVFLIP.toInt());
  s->set_hmirror(s, diagHFLIP.toInt());
  s->set_quality(s, diagQUAL.toInt());

  #if defined(LED_GPIO_NUM)
    setupLedFlash(LED_GPIO_NUM);
  #endif
 
  battery_init(); battery_read(diagBattery);

  // Wireless Startup and Configuration //
  Serial.print("wireless: TX_POWER_CFG = ");  Serial.println(diagWiFiPOWR);  diagWiFiPOWR.trim();
  WiFi.mode(WIFI_AP); WiFi.softAP(diagWiFiSSID, diagWiFiPASS,diagWiFiCHAN.toInt(),0,4);  // Set up the SoftAP
  uint8_t getprotocol;
  esp_wifi_get_protocol(WIFI_IF_AP, &getprotocol);
    if (getprotocol & WIFI_PROTOCOL_11B) { Serial.println("wireless: WIFI_PROTOCOL_11B = OK!"); diagWiFiPROT = "B"; }                 else { Serial.println("wireless: WIFI_PROTOCOL_11B = NOT SET!"); }
    if (getprotocol & WIFI_PROTOCOL_11G) { Serial.println("wireless: WIFI_PROTOCOL_11G = OK!"); diagWiFiPROT = diagWiFiPROT + "G"; }  else { Serial.println("wireless: WIFI_PROTOCOL_11G = NOT SET!"); }
    if (getprotocol & WIFI_PROTOCOL_11N) { Serial.println("wireless: WIFI_PROTOCOL_11N = OK!"); diagWiFiPROT = diagWiFiPROT + "N"; }  else { Serial.println("wireless: WIFI_PROTOCOL_11N = NOT SET!"); }
    if (getprotocol & WIFI_PROTOCOL_LR)  { Serial.println("wireless: WIFI_PROTOCOL_LR  = OK!"); diagWiFiPROT = diagWiFiPROT + "LR"; } else { Serial.println("wireless: WIFI_PROTOCOL_LR  = NOT SET!"); }
  // Must be applied once WiFi Mode is running in AP!
  if (diagWiFiPOWR == "WIFI_POWER_5dBm")    { Serial.print("wireless: set_tx_power 5.0 dBm (");  Serial.print(WiFi.setTxPower(WIFI_POWER_5dBm));    Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_7dBm")    { Serial.print("wireless: set_tx_power 7.0 dBm (");  Serial.print(WiFi.setTxPower(WIFI_POWER_7dBm));    Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_8_5dBm")  { Serial.print("wireless: set_tx_power 8.5 dBm (");  Serial.print(WiFi.setTxPower(WIFI_POWER_8_5dBm));  Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_13dBm")   { Serial.print("wireless: set_tx_power 13.0 dBm ("); Serial.print(WiFi.setTxPower(WIFI_POWER_13dBm));   Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_15dBm")   { Serial.print("wireless: set_tx_power 15.0 dBm ("); Serial.print(WiFi.setTxPower(WIFI_POWER_15dBm));   Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_17dBm")   { Serial.print("wireless: set_tx_power 17.0 dBm ("); Serial.print(WiFi.setTxPower(WIFI_POWER_17dBm));   Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_18_5dBm") { Serial.print("wireless: set_tx_power 18.5 dBm ("); Serial.print(WiFi.setTxPower(WIFI_POWER_18_5dBm)); Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_19dBm")   { Serial.print("wireless: set_tx_power 19.0 dBm ("); Serial.print(WiFi.setTxPower(WIFI_POWER_19dBm));   Serial.println("!)"); }
  if (diagWiFiPOWR == "WIFI_POWER_19_5dBm") { Serial.print("wireless: set_tx_power 19.5 dBm ("); Serial.print(WiFi.setTxPower(WIFI_POWER_19_5dBm)); Serial.println("!)"); }
  // Output the current power once configured
  Serial.print("wireless: TX_POWER  = "); Serial.println(WiFi.getTxPower());

  // Web Server Initialization, Timer Initialization, UDP Initialization and Camera Server Intitialization
  webServerInit(); setupTelnet(); tkSecond.attach(1, everySecond);  tkBlink.attach(0.5, timerBlink);  tkUdpSend.attach(5, udpSend); udpListener.begin(2090);
  Serial.println("udpserver   : UDP Server Initialized!");
  Serial.println("system_stats: Initialization Complete!");
  Serial.println("cameraserver: Camera Server Starting!");
  startCameraServer();
}

void loop() {
  server.handleClient();
  telnet.loop();
  
  int packetSize = udpListener.parsePacket();
  if (packetSize)
    {
      int len = udpListener.read(packetBuffer, 255);
      if (len >= 5) 
      {  
        packetBuffer[len] = 0; String szData = String(packetBuffer); sensor_t *s = esp_camera_sensor_get(); 
        // TESTED - OK!
        if(szData.indexOf("camera set res VGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_VGA);  if (res == 0) { diagCfgRES = "FRAMESIZE_VGA";  receivecfg("/cfg-res.ini","FRAMESIZE_VGA", "camera: set_famesize FRAMESIZE_VGA (OK!)");  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_VGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res SVGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SVGA); if (res == 0) { diagCfgRES = "FRAMESIZE_SVGA"; receivecfg("/cfg-res.ini","FRAMESIZE_SVGA","camera: set_famesize FRAMESIZE_SVGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SVGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res XGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_XGA);  if (res == 0) { diagCfgRES = "FRAMESIZE_XGA";  receivecfg("/cfg-res.ini","FRAMESIZE_XGA", "camera: set_famesize FRAMESIZE_XGA (OK!)");  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_XGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res HD") >=0)   { res = s->set_framesize(s, FRAMESIZE_HD);   if (res == 0) { diagCfgRES = "FRAMESIZE_HD";   receivecfg("/cfg-res.ini","FRAMESIZE_HD",  "camera: set_famesize FRAMESIZE_HD (OK!)");   } else { Serial.println("cameraserver: set_famesize FRAMESIZE_HD (FAILED!)"); } }  
        if(szData.indexOf("camera set res SXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SXGA); if (res == 0) { diagCfgRES = "FRAMESIZE_SXGA"; receivecfg("/cfg-res.ini","FRAMESIZE_SXGA","camera: set_famesize FRAMESIZE_SXGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SXGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res UXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { diagCfgRES = "FRAMESIZE_UXGA"; receivecfg("/cfg-res.ini","FRAMESIZE_UXGA","camera: set_famesize FRAMESIZE_UXGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); } }  
  
        if(szData.indexOf("camera set hflip on")  >=0) { res = s->set_hmirror(s, 1); diagHFLIP = "1"; receivecfg("/cfg-hflp.ini","1", "camera: set_hflip on (OK!)");  }
        if(szData.indexOf("camera set hflip off") >=0) { res = s->set_hmirror(s, 0); diagHFLIP = "0"; receivecfg("/cfg-hflp.ini","0", "camera: set_hflip off (OK!)"); }
        if(szData.indexOf("camera set vflip on")  >=0) { res = s->set_vflip(s, 1);   diagVFLIP = "1"; receivecfg("/cfg-vflp.ini","1", "camera: set_vflip on (OK!)");  }
        if(szData.indexOf("camera set vflip off") >=0) { res = s->set_vflip(s, 0);   diagVFLIP = "0"; receivecfg("/cfg-vflp.ini","0", "camera: set_vflip off (OK!)"); }

        if(szData.indexOf("camera set aec2 on")  >=0) { res = s->set_aec2(s, 1); diagCfgAEC2 = "1"; receivecfg("/cfg-aec2.ini","1", "camera: set_aec2 on (OK!)");  }
        if(szData.indexOf("camera set aec2 off") >=0) { res = s->set_aec2(s, 0); diagCfgAEC2 = "0"; receivecfg("/cfg-aec2.ini","0", "camera: set_aec2 off (OK!)"); }

        if(szData.indexOf("cpu set 240") >=0) { setCpuFrequencyMhz(240); diagCPU = "240"; Serial.print("esp32-cam: CPU Frequency: "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz (OK!)"); receivecfg("/cpu.ini","240", ""); }
        if(szData.indexOf("cpu set 160") >=0) { setCpuFrequencyMhz(160); diagCPU = "160"; Serial.print("esp32-cam: CPU Frequency: "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz (OK!)"); receivecfg("/cpu.ini","160", ""); }
        if(szData.indexOf("cpu set 80")  >=0) { setCpuFrequencyMhz(80);  diagCPU = "80";  Serial.print("esp32-cam: CPU Frequency: "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz (OK!)"); receivecfg("/cpu.ini","80",  ""); }
        if(szData.indexOf("reboot") >= 0)     { tkUdpSend.detach(); Serial.println("Rebooting in 5 seconds..."); delay(5000); ESP.restart(); }

        if(szData.indexOf("camera set quality") >=0)  { res = s->set_quality(s, szData.substring(19).toInt()); diagQUAL = szData.substring(19).toInt(); if (res == 0) { receivecfg("/camera/cfg-qual.txt",szData.substring(19),"camera: set_qual " + szData.substring(19) + " (OK!)"); } else { Serial.println("cameraserver: set_qual (FAILED!)"); } }  
        if(szData.indexOf("camera set xclk") >=0)     { res = s->set_xclk(s, LEDC_TIMER_0, szData.substring(16).toInt()); diagCfgXCLK = szData.substring(16).toInt(); if (res == 0) { receivecfg("/camera/cfg-xclx.txt",szData.substring(16),"camera: set_xclk " + szData.substring(16) + " (OK!)"); } else { Serial.println("cameraserver: set_xclk (FAILED!)"); } }  

        if(szData.indexOf("wireless get config")>=0)   { Serial.println("wireless: sending wireless config via UDP"); udpSendConfig(); }
        if(szData.indexOf("wireless set channel") >=0) { diagWiFiCHAN = szData.substring(21); receivecfg("/wifichan.ini", szData.substring(21), "wireless: set_chan (OK!) Reboot Required"); }
        if(szData.indexOf("wireless set name")    >=0) { diagWiFiSSID = szData.substring(18); receivecfg("/wifissid.ini", szData.substring(18), "wireless: set_ssid (OK!) Reboot Required"); }
        if(szData.indexOf("wireless set password")>=0) { diagWiFiPASS = szData.substring(22); receivecfg("/wifipass.ini", szData.substring(22), "wireless: set_pass (OK!) Reboot Required"); }

        if(szData.indexOf("wireless set power")   >=0) 
        { 
          switch (szData.substring(19).toInt())
          {
            case 1:  Serial.print("wireless: set_tx_power 5.0 dBm = ");  diagWiFiPOWR = "WIFI_POWER_5dBm";    Serial.print(WiFi.setTxPower(WIFI_POWER_5dBm));    Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_5dBm", ""   ); break;
            case 2:  Serial.print("wireless: set_tx_power 7.0 dBm = ");  diagWiFiPOWR = "WIFI_POWER_7dBm";    Serial.print(WiFi.setTxPower(WIFI_POWER_7dBm));    Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_7dBm", ""   ); break;
            case 3:  Serial.print("wireless: set_tx_power 8.5 dBm = ");  diagWiFiPOWR = "WIFI_POWER_8_5dBm";  Serial.print(WiFi.setTxPower(WIFI_POWER_8_5dBm));  Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_8_5dBm", "" ); break;
            case 4:  Serial.print("wireless: set_tx_power 11.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_11dBm";   Serial.print(WiFi.setTxPower(WIFI_POWER_11dBm));   Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_11dBm", ""  ); break;
            case 5:  Serial.print("wireless: set_tx_power 13.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_13dBm";   Serial.print(WiFi.setTxPower(WIFI_POWER_13dBm));   Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_13dBm", ""  ); break;
            case 6:  Serial.print("wireless: set_tx_power 15.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_15dBm";   Serial.print(WiFi.setTxPower(WIFI_POWER_15dBm));   Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_15dBm", ""  ); break;
            case 7:  Serial.print("wireless: set_tx_power 17.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_17dBm";   Serial.print(WiFi.setTxPower(WIFI_POWER_17dBm));   Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_17dBm", ""  ); break;
            case 8:  Serial.print("wireless: set_tx_power 18.5 dBm = "); diagWiFiPOWR = "WIFI_POWER_18_5dBm"; Serial.print(WiFi.setTxPower(WIFI_POWER_18_5dBm)); Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_18_5dBm", ""); break;
            case 9:  Serial.print("wireless: set_tx_power 19.0 dBm = "); diagWiFiPOWR = "WIFI_POWER_19dBm";   Serial.print(WiFi.setTxPower(WIFI_POWER_19dBm));   Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_19dBm", ""  ); break;
            case 10: Serial.print("wireless: set_tx_power 19.5 dBm = "); diagWiFiPOWR = "WIFI_POWER_19_5dBm"; Serial.print(WiFi.setTxPower(WIFI_POWER_19_5dBm)); Serial.print(" - TX_POWER  = "); Serial.println(WiFi.getTxPower()); receivecfg("/wifitxpw.ini","WIFI_POWER_19_5dBm", ""); break;
          }
        }
      }
    }
  }
