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
#include "lib_wifi.h"
#include "lib_cpu.h"
#include "lib_camera.h"
#include "lib_commands.h"

#include "esp_camera.h"
#include "esp_wifi.h"

#include "ESPTelnet.h"
//#include "printHelpers.h"

#include <uptime.h>
#include <uptime_formatter.h>
#include <string_asukiaaa.h>

#define LED_GRN  14
#define LED_YEL  15
#define LED_RED  13
#define LED_BLU  2

int ledState = LOW;
uint8_t otaDone = 0;

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
String diagVFLIP = "1";
String diagHFLIP = "1";
String diagQUAL = "12";     //10-63 lower number means higher quality

String diagCPU = "240";

WebServer server(8090);
WiFiUDP udpListener;
char packetBuffer[255];

Ticker tkSecond;
Ticker tk_wifi_blink;
Ticker tkUdpSend;

IPAddress temp_IP(192,168,4,255);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

void startCameraServer();

ESPTelnet telnet;
IPAddress ip;
uint16_t  port = 23;

void everySecond(){ if (otaDone > 1) { Serial.printf("ota: %d%%\n", otaDone); } }
void timer_wifi_blink() { if (WiFi.softAPgetStationNum() == 0) { if (ledState == LOW) { ledState = HIGH; } else { ledState = LOW; } digitalWrite(LED_BLU, ledState); } else { digitalWrite(LED_BLU, HIGH); } }


void udpSend() { 
  uptime::calculateUptime();
  diagUptime = string_asukiaaa::padStart(String(uptime::getHours()), 2, '0') + "h:" + string_asukiaaa::padStart(String(uptime::getMinutes()), 2, '0') + "m:" + string_asukiaaa::padStart(String(uptime::getSeconds()), 2, '0') + "s";
  
  String udpMessage = "[ESP32-CAM] Uptime: " + diagUptime + " | Res: " + diagCfgRES.substring(10) + " | Clients: " + String(WiFi.softAPgetStationNum()) + " | Batt ADC: " + diagBattery + " (PWR ON)";
  udpMessage = udpMessage + " | CPU: " + diagCPU + "Mhz | " + " WiFi Prot: " + diagWiFiPROT + " | WiFi CH: " + diagWiFiCHAN + " | WiFi TX: " + diagWiFiPOWR.substring(11) + " | XCLK: " + diagCfgXCLK + " | AEC2: " + diagCfgAEC2;
  udpMessage = udpMessage + " | HF: " + diagHFLIP + " | VF: " + diagVFLIP + " | QUAL: " + diagQUAL;
  Serial.print("udpserver_tx: "); udpSendMessage(temp_IP,udpMessage,2090); Serial.println(udpMessage); 
  }

void setupTelnet() { telnet.onConnect(onTelnetConnect); telnet.onConnectionAttempt(onTelnetConnectionAttempt); telnet.onReconnect(onTelnetReconnect); telnet.onDisconnect(onTelnetDisconnect); telnet.onInputReceived(onTelnetInput); Serial.print("telnetserver: "); if (telnet.begin(port)) { Serial.println("running");  }  telnetRunning = true; }
void onTelnetConnect(String ip)           { telnet.println("Starting AP-DOS..."); telnet.println(""); telnet.println(""); telnet.print("C:\\>"); }
void onTelnetDisconnect(String ip)        { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" disconnected"); }
void onTelnetReconnect(String ip)         { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" reconnected"); telnet.println(""); telnet.println("Reconnected to AP-DOS Version 1.00"); telnet.println(""); telnet.println("");  telnet.print("C:\\>"); }
void onTelnetConnectionAttempt(String ip) { Serial.print("telnetserver: "); Serial.print(ip); Serial.println(" tried to connect"); }
void onTelnetInput(String str)            {  process_command_tel(str, telnet, diagCfgRES, diagCfgAEC2, diagCfgXCLK, diagQUAL, diagHFLIP, diagVFLIP, diagCPU, diagWiFiPOWR, diagWiFiCHAN, diagWiFiSSID, diagWiFiPASS); }

void webServerInit() {
  server.on("/update", HTTP_POST, []() { handleUpdateEnd(); }, []() { handleUpdate(); } );
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.onNotFound([]() { server.send(200, "text/html", indexHtml); });
  server.begin();
  Serial.printf("http_service: %s\n", WiFi.softAPIP().toString().c_str());
}                

void handleUpdateEnd()  { server.sendHeader("Connection", "close"); if (Update.hasError()) { server.send(502, "text/plain", Update.errorString()); } else { server.sendHeader("Refresh", "10"); server.sendHeader("Location", "/"); server.send(307); delay(2000); ESP.restart(); delay(200); } }
void handleUpdate()     { size_t fsize = UPDATE_SIZE_UNKNOWN; if (server.hasArg("size")) { fsize = server.arg("size").toInt(); } HTTPUpload &upload = server.upload(); if (upload.status == UPLOAD_FILE_START) { Serial.printf("Receiving Update: %s, Size: %d\n", upload.filename.c_str(), fsize);    if (!Update.begin(fsize)) { otaDone = 0; Update.printError(Serial); }    } else if (upload.status == UPLOAD_FILE_WRITE) {    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {      Update.printError(Serial);    } else {      otaDone = 100 * Update.progress() / Update.size();    }  } else if (upload.status == UPLOAD_FILE_END) {    if (Update.end(true)) {      Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize); delay(200);   } else {      Serial.printf("%s\n", Update.errorString()); otaDone = 0; }  }}

void setup() {
  Serial.begin(9600); Serial.println(""); Serial.println(""); uptime::calculateUptime(); diagUptime = String(uptime::getHours()) + "H:" + String(uptime::getMinutes()) + "M:" + String(uptime::getSeconds()) + "S";
  
  battery_read_initialization(diagBattery);                                                           // Battery Subsystem Initialization
  spiffs_files_initialization(); listDir(SPIFFS, "/", 1); delay(10);                                  // File System Initialization
  esp32_camera_initialization(diagCfgRES, diagHFLIP, diagVFLIP, diagQUAL, diagCfgAEC2, diagCfgXCLK);  // Camera Subsystem Initialization
  esp32_cpusvc_initialization(diagCPU);                                                               // CPU Subsystem Initialization
  

  wifi_espwifi_initialization(diagWiFiPOWR, diagWiFiSSID, diagWiFiPASS, diagWiFiCHAN, diagWiFiPROT);  // Wireless Subsystem Initialization

  // Web Server Initialization, Timer Initialization, UDP Initialization and Camera Server Intitialization
  webServerInit(); setupTelnet(); tkSecond.attach(1, everySecond);  tk_wifi_blink.attach(0.5, timer_wifi_blink);  tkUdpSend.attach(5, udpSend); udpListener.begin(2090);
  Serial.println("udpserver   : UDP Server Initialized!");
  Serial.println("system_stats: Initialization Complete!");
  Serial.println("cameraserver: Camera Server Starting!");
  startCameraServer();
}

void loop() {
  server.handleClient();
  telnet.loop();
  
  int packetSize = udpListener.parsePacket();
  if (packetSize) { int len = udpListener.read(packetBuffer, 255); if (len >= 5) {  process_command_udp(len, packetBuffer, udpListener, diagCfgRES, diagCfgAEC2, diagCfgXCLK, diagQUAL, diagHFLIP, diagVFLIP, diagCPU, diagWiFiPOWR, diagWiFiCHAN, diagWiFiSSID, diagWiFiPASS);    }  }
}
