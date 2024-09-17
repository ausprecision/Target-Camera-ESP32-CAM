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
#include "esp32-hal-cpu.h"
#include "WiFiGeneric.h"

#include "FS.h"
#include "SPIFFS.h"
#include "ESPTelnet.h"
#include "printHelpers.h"
#include <esp_wifi.h>
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
String diagWiFiPROT = "SR";
String diagCfgRES = "FRAMESIZE_UXGA";
String diagCfgXCLK = "8";
String diagCfgAEC2 = "1";
String diagCPU = "240";

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
  String udpMessage = "[ESP32-CAM] Uptime: " + diagUptime + " | WiFi Clients: " + String(WiFi.softAPgetStationNum()) + " | Batt ADC: " + diagBattery + " (PWR ON)";
  udpMessage = udpMessage + " | CPU: " + diagCPU + "Mhz | " + " WiFi Prot: " + diagWiFiPROT + " | WiFi Chan: " + diagWiFiCHAN + " | CamXCLK: " + diagCfgXCLK + " | CamAEC2: " + diagCfgAEC2;
  Serial.print("udpserver_tx: "); udpSendMessage(temp_IP,udpMessage,2090); Serial.println(udpMessage); 
  }

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
void onTelnetReconnect(String ip)         { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" reconnected"); telnet.println("reconnected"); telnet.print("C:\\>"); }
void onTelnetConnectionAttempt(String ip) { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" tried to connected"); }

void onTelnetInput(String str) {
  sensor_t *s = esp_camera_sensor_get(); 
  if (str == "ver")                 { cmd_ver(telnet); } 
  else if (str == "vol")            { cmd_vol(telnet); }
  else if (str == "help")           { cmd_help(telnet);} 
  else if (str == "dir")            { cmd_dir(telnet); }
  else if (str == "del")            { telnet.println("Required parameter missing"); telnet.println(""); telnet.print("C:\\>"); }
  else if (str.indexOf("del ") >=0) { if (SPIFFS.remove(String("/" + str.substring(4)))) {  telnet.println(""); telnet.print("C:\\>"); } else { telnet.println("File not found"); telnet.println(""); telnet.print("C:\\>"); } }
  else if (str == "reboot")         { telnet.println("Rebooting..."); telnet.println(""); telnet.print("C:\\>"); delay(2000); ESP.restart(); }
  else if (str == "exit")           { telnet.println("Disconnecting from AP-DOS..."); telnet.disconnectClient(); } 
  else if (str == "camera set res vga") { res = s->set_framesize(s, FRAMESIZE_VGA);  if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_VGA",  "cameraserver: set_famesize FRAMESIZE_VGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_VGA (FAILED!)");  telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res svga"){ res = s->set_framesize(s, FRAMESIZE_SVGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_SVGA", "cameraserver: set_famesize FRAMESIZE_SVGA (OK!)", diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_SVGA (FAILED!)"); telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res xga") { res = s->set_framesize(s, FRAMESIZE_XGA);  if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_XVGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_XGA (FAILED!)");  telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res hd")  { res = s->set_framesize(s, FRAMESIZE_HD);   if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_HD",   "cameraserver: set_famesize FRAMESIZE_XGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_HD (FAILED!)");   telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res sxga"){ res = s->set_framesize(s, FRAMESIZE_SXGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_SXGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_SXGA (FAILED!)"); telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res uxga"){ res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_UXGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res uxga"){ res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_UXGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); telnet.println(""); telnet.print("C:\\>"); } }  
  else if (str == "camera set res uxga"){ res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_UXGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)",  diagCfgRES); telnet.println(""); telnet.println("OK"); telnet.println(""); telnet.print("C:\\>");  } else { telnet.println(""); telnet.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); telnet.println(""); telnet.print("C:\\>"); } }  

  else if (str.indexOf("camera set xclk ") >=0) { 
    res = s->set_xclk(s, LEDC_TIMER_0, str.substring(16).toInt()); 
    if (res == 0) 
    { 
      receivecfg("/camera/cfg-xclx.txt",str.substring(16),"cameraserver: set_xclk " + str.substring(16) + " (OK!)", diagCfgXCLK);
      telnet.println(""); 
      telnet.print(str.substring(16)); 
      telnet.println(" OK"); 
      telnet.println(""); 
      telnet.print("C:\\>"); 
    } 
    else 
    { 
      telnet.println("cameraserver: set_xclk (FAILED!)"); 
      telnet.println(""); telnet.print("C:\\>"); 
    } 
  }  
  else if (str.indexOf("camera set aec2 ") >=0) 
  { 
    s->set_aec2(s, str.substring(16).toInt()); 
    if (res == 0) 
    { 
      receivecfg("/camera/cfg-aec2.txt",str.substring(16),"cameraserver: set_aec2 " + str.substring(16) + " (OK!)", diagCfgAEC2); 
      telnet.println(""); 
      telnet.print(str.substring(16)); 
      telnet.println(" OK"); 
      telnet.println(""); 
      telnet.print ("C:\\>"); 
    }
    else 
    { 
      telnet.println("cameraserver: set_xclk (FAILED!)"); 
      telnet.println(""); 
      telnet.print("C:\\>"); 
    } 
  }  
  else if (str == "wifi set prot lr")     { receivecfg("/wifiprot.ini","LR", "wifiserver  : set_protocol_lr (OK!)", diagWiFiPROT); telnet.println("LR (OK) Reboot Required!"); telnet.println(""); telnet.print("C:\\>");   }  
  else if (str == "wifi set prot sr")     { receivecfg("/wifiprot.ini","SR", "wifiserver  : set_protocol_sr (OK!)", diagWiFiPROT); telnet.println("SR (OK) Reboot Required!"); telnet.println(""); telnet.print("C:\\>");   }  
  else if (str == "")               { telnet.print("C:\\>"); } 
  else                              { telnet.println("Bad command or file name"); telnet.println(""); telnet.print("C:\\>"); }
}


void webServerInit() {
  server.on("/update", HTTP_POST, []() { handleUpdateEnd(); }, []() { handleUpdate(); } );
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.onNotFound([]() { server.send(200, "text/html", indexHtml); });
  server.begin();
  Serial.printf("http_service: %s\n", WiFi.softAPIP().toString().c_str());
}                

void handleUpdateEnd()  { server.sendHeader("Connection", "close"); if (Update.hasError()) { server.send(502, "text/plain", Update.errorString()); } else { server.sendHeader("Refresh", "10"); server.sendHeader("Location", "/"); server.send(307); delay(2000); ESP.restart(); } }
void handleUpdate()     { size_t fsize = UPDATE_SIZE_UNKNOWN; if (server.hasArg("size")) { fsize = server.arg("size").toInt(); } HTTPUpload &upload = server.upload(); if (upload.status == UPLOAD_FILE_START) { Serial.printf("Receiving Update: %s, Size: %d\n", upload.filename.c_str(), fsize);    if (!Update.begin(fsize)) { otaDone = 0; Update.printError(Serial); }    } else if (upload.status == UPLOAD_FILE_WRITE) {    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {      Update.printError(Serial);    } else {      otaDone = 100 * Update.progress() / Update.size();    }  } else if (upload.status == UPLOAD_FILE_END) {    if (Update.end(true)) {      Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize);    } else {      Serial.printf("%s\n", Update.errorString()); otaDone = 0; }  }}

void setup() {
  Serial.begin(9600); 
  Serial.println(""); 
  Serial.println("");
  uptime::calculateUptime();
  diagUptime = String(uptime::getHours()) + "H:" + String(uptime::getMinutes()) + "M:" + String(uptime::getSeconds()) + "S";

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

  //s->set_aec2(s, 1);
  //s->set_xclk(s, LEDC_TIMER_0, 8);
  //s->set_framesize(s, FRAMESIZE_UXGA);

  //setCpuFrequencyMhz(160);

  Serial.print("XTAL Crystal Frequency: "); Serial.print(getXtalFrequencyMhz());  Serial.println(" MHz");
  Serial.print("CPU Frequency: ");          Serial.print(getCpuFrequencyMhz());   Serial.println(" MHz");
  Serial.print("APB Bus Frequency: ");      Serial.print(getApbFrequency());      Serial.println(" Hz");

  spiffs_init(); listDir(SPIFFS, "/", 1);
  
  diagCPU      = readConfig("/cpu.ini");
  diagWiFiCHAN = readConfig("/wifichan.ini");
  diagWiFiPASS = readConfig("/wifipass.ini");
  diagWiFiPROT = readConfig("/wifiprot.ini");
  diagWiFiSSID = readConfig("/wifissid.ini");

  if      (diagCfgRES == "1600x1200") { s->set_framesize(s, FRAMESIZE_UXGA); }
  else if (diagCfgRES == "1280x1024") { s->set_framesize(s, FRAMESIZE_SXGA); }
  else if (diagCfgRES == "1280x720")  { s->set_framesize(s, FRAMESIZE_HD); }
  else if (diagCfgRES == "800x600")   { s->set_framesize(s, FRAMESIZE_SVGA); }
  else if (diagCfgRES == "640x480")   { s->set_framesize(s, FRAMESIZE_VGA); }
  else    { s->set_framesize(s, FRAMESIZE_UXGA); }

  s->set_xclk(s, LEDC_TIMER_0, diagCfgXCLK.toInt());
  s->set_aec2(s, diagCfgAEC2.toInt());
  s->set_vflip(s, 1);

  #if defined(LED_GPIO_NUM)
    setupLedFlash(LED_GPIO_NUM);
  #endif
 
  battery_init();
  battery_read(diagBattery);
  
  WiFi.mode(WIFI_AP);  if (diagWiFiPROT == "LR") { esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_LR ); }  WiFi.softAP("apcamera", "tspass2000",diagWiFiCHAN.toInt(),0,4);  // Set up the SoftAP
  Serial.println(WIFI_POWER_11dBm);
  Serial.print("Set power status=");
  //Serial.println(WiFi.setTxPower(WIFI_POWER_5dBm));
  //Serial.println(WiFi.setTxPower(WIFI_POWER_11dBm));
  
  webServerInit(); setupTelnet();
  tkSecond.attach(1, everySecond);  tkBlink.attach(0.5, timerBlink);  tkUdpSend.attach(5, udpSend);

  udpListener.begin(2090);
  Serial.println("udpserver   : UDP Server Initialized!");
  Serial.println("system_stats: Initialization Complete!");
  Serial.println("cameraserver: Camera Server Starting!");
  startCameraServer();
}


void receivecfg(String filename, String cfgoption, String serialoutput, String diagString) {
  File file;
  file = SPIFFS.open(filename, FILE_WRITE);
  file.println(cfgoption);
  file.close();
  diagString = cfgoption;
  Serial.println(serialoutput);
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
        packetBuffer[len] = 0;
        String szData = String(packetBuffer);
        String szLog = "udpserver_rx: " + szData;
        Serial.println(szLog);
        sensor_t *s = esp_camera_sensor_get(); 
        if(szData.indexOf("camera set res VGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_VGA);  if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_VGA", "cameraserver: set_famesize FRAMESIZE_VGA (OK!)", diagCfgRES);  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_VGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res SVGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SVGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_SVGA","cameraserver: set_famesize FRAMESIZE_SVGA (OK!)", diagCfgRES); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SVGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res XGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_XGA);  if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_XGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)", diagCfgRES);  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_XGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res HD") >=0)   { res = s->set_framesize(s, FRAMESIZE_HD);   if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_HD",  "cameraserver: set_famesize FRAMESIZE_HD (OK!)", diagCfgRES);   } else { Serial.println("cameraserver: set_famesize FRAMESIZE_HD (FAILED!)"); } }  
        if(szData.indexOf("camera set res SXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SXGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_SXGA","cameraserver: set_famesize FRAMESIZE_SXGA (OK!)", diagCfgRES); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SXGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res UXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { receivecfg("/cfg-res.ini","FRAMESIZE_UXGA","cameraserver: set_famesize FRAMESIZE_UXGA (OK!)", diagCfgRES); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); } }  
        if(szData.indexOf("camera set xclk") >=0)     { res = s->set_xclk(s, LEDC_TIMER_0, szData.substring(16).toInt()); if (res == 0) { receivecfg("/camera/cfg-xclx.txt",szData.substring(16),"cameraserver: set_xclk " + szData.substring(16) + " (OK!)", diagCfgXCLK); } else { Serial.println("cameraserver: set_xclk (FAILED!)"); } }  
        if(szData.indexOf("del") >=0) { if (SPIFFS.remove(szData.substring(4))) {  Serial.println(""); } else { Serial.println("File not found"); } }
      }
    }
  }
