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
#include "esp_camera.h"
#include "FS.h"
#include "SPIFFS.h"
#include "ESPTelnet.h"
#include <uptime.h>
#include <uptime_formatter.h>

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
  //uptime_formatter::getHours()
  //diagUptime = uptime_formatter::getHours() + "H:" + uptime_formatter::getMinutes() + "M:" + uptime_formatter::getSeconds() + "S";
  //diagUptime = uptime::getHours() + "H:" + uptime::getMinutes() + "M:" + uptime::getSeconds() + "S";
  diagUptime = String(uptime::getHours()) + "H:" + String(uptime::getMinutes()) + "M:" + String(uptime::getSeconds()) + "S";
  String udpMessage = "[ESP32-CAM] Uptime: " + diagUptime + " | Batt ADC: " + diagBattery;
  Serial.print("udpserver_tx: "); 
  udpSendMessage(temp_IP,udpMessage,2090); 
  Serial.println(udpMessage); 
  }

void setupTelnet() {  
  // passing on functions for various telnet events
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);
  telnet.onInputReceived(onTelnetInput);
  Serial.print("telnetserver: "); if (telnet.begin(port)) { Serial.println("running");  }
  telnetRunning = true;
}

void onTelnetConnect(String ip) { telnet.println("Starting AP-DOS..."); telnet.println(""); telnet.println(""); telnet.print("C:\\>"); }

void onTelnetDisconnect(String ip)        { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" disconnected"); }
void onTelnetReconnect(String ip)         { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" reconnected");  }
void onTelnetConnectionAttempt(String ip) { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" tried to connected"); }
String padStr(String paddedStr){ String tempStr; if (paddedStr.length() == 6) { tempStr = paddedStr + "   "; } else if (paddedStr.length() == 7) { tempStr = paddedStr + "  "; } else if (paddedStr.length() == 8) { tempStr = paddedStr + " "; } return tempStr; }

void onTelnetInput(String str) {
  // checks for a certain command
  if (str == "ver") { telnet.println(""); telnet.println("AP-DOS Version 1.00"); telnet.println(""); telnet.println(""); telnet.print("C:\\>");
  } else if (str == "exit") { telnet.println("Disconnecting from AP-DOS..."); telnet.disconnectClient();
  } else if (str == "dir") {
    telnet.println("");
    telnet.println(" Volume in drive C is AP-DOS_1");
    telnet.println(" Volume Serial Number is ESP32-SPIFFS"); 
    telnet.println(" Directory of C:\\");
    telnet.println("");
    File root = SPIFFS.open("/");
    if(root.isDirectory()){
      File file = root.openNextFile();
      int fileCount = 0;
      while(file){
        fileCount = fileCount + 1;
        int delimiterDot = String(file.name()).indexOf('.');
        String tempStr = String(file.name()).substring(0, delimiterDot);

        String stringFileName = padStr(tempStr);
        stringFileName.toUpperCase();

        String stringExtension = String(file.name()).substring(delimiterDot+1, delimiterDot + 4);
        stringExtension.toUpperCase();

        telnet.print(stringFileName); telnet.print(stringExtension); telnet.print(" ");
        telnet.println(String(file.size()));
        file = root.openNextFile();
      }

      telnet.println("");
      telnet.print("C:\\>");
    }
  } else if (str == "") { telnet.print("C:\\>");
  } else { telnet.println("Bad command or file name"); telnet.println(""); telnet.print("C:\\>"); }
}




void handleFileList() {
  File root = SPIFFS.open("/");
  String output = "[";
  if(root.isDirectory()){
      File file = root.openNextFile();
      while(file){
          if (output != "[") {
            output += ',';
          }
          output += "{\"type\":\"";
          output += (file.isDirectory()) ? "dir" : "file";
          output += "\",\"name\":\"";
          output += String(file.name()).substring(1);
          output += "\"}";
          
          file = root.openNextFile();
      }
  }
  output += "]";
  server.send(200, "text/json", output);
}

void webServerInit() {
  server.on("/update", HTTP_POST, []() { handleUpdateEnd(); }, []() { handleUpdate(); } );
  server.on("/list", HTTP_GET, handleFileList);
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.onNotFound([]() { server.send(200, "text/html", indexHtml); });
  server.begin();
  Serial.printf("http_service: %s\n", WiFi.softAPIP().toString().c_str());
}                

void handleUpdateEnd()  { server.sendHeader("Connection", "close"); if (Update.hasError()) { server.send(502, "text/plain", Update.errorString()); } else { server.sendHeader("Refresh", "10"); server.sendHeader("Location", "/"); server.send(307); delay(2000); ESP.restart(); } }
void handleUpdate()     { size_t fsize = UPDATE_SIZE_UNKNOWN; if (server.hasArg("size")) { fsize = server.arg("size").toInt(); } HTTPUpload &upload = server.upload(); if (upload.status == UPLOAD_FILE_START) { Serial.printf("Receiving Update: %s, Size: %d\n", upload.filename.c_str(), fsize);    if (!Update.begin(fsize)) { otaDone = 0; Update.printError(Serial); }    } else if (upload.status == UPLOAD_FILE_WRITE) {    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {      Update.printError(Serial);    } else {      otaDone = 100 * Update.progress() / Update.size();    }  } else if (upload.status == UPLOAD_FILE_END) {    if (Update.end(true)) {      Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize);    } else {      Serial.printf("%s\n", Update.errorString()); otaDone = 0; }  }}

void setup() {
  Serial.begin(9600);
  //delay(2000);

  uptime::calculateUptime();
  //uptime_formatter::getUptime();
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
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) { Serial.printf("Camera init failed with error 0x%x", err);  return;  }

  sensor_t *s = esp_camera_sensor_get();

  // camera - read settings from config files
  s->set_aec2(s, 1);
  s->set_xclk(s, LEDC_TIMER_0, 10);
  s->set_framesize(s, FRAMESIZE_UXGA);
  s->set_vflip(s, 1);

  battery_init();
  battery_read(diagBattery);

  spiffs_init();
  listDir(SPIFFS, "/", 1);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP("apcamera", "tspass2000",11,0,4);  // Set up the SoftAP
  webServerInit(); 
  setupTelnet();
  tkSecond.attach(1, everySecond);
  tkBlink.attach(0.5, timerBlink);
  tkUdpSend.attach(5, udpSend);

  udpListener.begin(2090);
  Serial.println("udpserver   : UDP Server Initialized!");
  Serial.println("system_stats: Initialization Complete!");
  Serial.println("cameraserver: Camera Server Starting!");
  startCameraServer();
}


void receivecfg(String filename, String cfgoption, String serialoutput) {
  File file;
  file = SPIFFS.open(filename, FILE_WRITE);
  file.println(cfgoption);
  file.close();
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
        if(szData.indexOf("camera set res VGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_VGA);  if (res == 0) { receivecfg("/camera/cfg-res.txt","VGA", "cameraserver: set_famesize FRAMESIZE_VGA (OK!)");  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_VGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res SVGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SVGA); if (res == 0) { receivecfg("/camera/cfg-res.txt","SVGA","cameraserver: set_famesize FRAMESIZE_SVGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SVGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res XGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_XGA);  if (res == 0) { receivecfg("/camera/cfg-res.txt","XGA", "cameraserver: set_famesize FRAMESIZE_XGA (OK!)");  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_XGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res HD") >=0)   { res = s->set_framesize(s, FRAMESIZE_HD);   if (res == 0) { receivecfg("/camera/cfg-res.txt","HD",  "cameraserver: set_famesize FRAMESIZE_HD (OK!)");   } else { Serial.println("cameraserver: set_famesize FRAMESIZE_HD (FAILED!)"); } }  
        if(szData.indexOf("camera set res SXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SXGA); if (res == 0) { receivecfg("/camera/cfg-res.txt","SXGA","cameraserver: set_famesize FRAMESIZE_SXGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SXGA (FAILED!)"); } }  
        if(szData.indexOf("camera set res UXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { receivecfg("/camera/cfg-res.txt","UXGA","cameraserver: set_famesize FRAMESIZE_UXGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); } }  

        if(szData.indexOf("camera set xclk") >=0)     { res = s->set_xclk(s, LEDC_TIMER_0, szData.substring(16).toInt()); if (res == 0) { receivecfg("/camera/cfg-xclx.txt",szData.substring(16),"cameraserver: set_xclk " + szData.substring(16) + " (OK!)"); } else { Serial.println("cameraserver: set_xclk (FAILED!)"); } }  

        if(szData.indexOf("del") >=0) { if (SPIFFS.remove(szData.substring(4))) {  Serial.println(""); } else { Serial.println("File not found"); } }
      }
    }
  }
