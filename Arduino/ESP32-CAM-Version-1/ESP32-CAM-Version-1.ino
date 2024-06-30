#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <Ticker.h>

#include "html.h"
#include "lib_battery.h"
#include "lib_spiffs.h"
#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

#define LED_GRN  14
#define LED_YEL  15
#define LED_RED  13
#define LED_BLU  2

int ledState = LOW;
uint8_t otaDone = 0;

WebServer server(8090);
Ticker tkSecond;
Ticker tkBlink;

void startCameraServer();
void setupLedFlash(int pin);

void everySecond(){ if (otaDone > 1) { Serial.printf("ota: %d%%\n", otaDone); } }
void timerBlink() { if (ledState == LOW) { ledState = HIGH; } else { ledState = LOW; } digitalWrite(LED_BLU, ledState); } 

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
  delay(5000);

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
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else { config.frame_size = FRAMESIZE_240X240; }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) { Serial.printf("Camera init failed with error 0x%x", err);  return;  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif
  
  
  
  battery_init();
  battery_read();
  spiffs_init();
  listDir(SPIFFS, "/", 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("apcamera", "tspass2000");  // Set up the SoftAP
  webServerInit(); 
  tkSecond.attach(1, everySecond);
  tkBlink.attach(0.5, timerBlink);
  Serial.println("system_stats: Initialization Complete!");

  startCameraServer();

}

void loop() {
  server.handleClient();
}
