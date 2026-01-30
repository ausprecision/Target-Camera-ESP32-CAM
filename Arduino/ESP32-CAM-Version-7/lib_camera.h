#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

void setupLedFlash(int pin);

void esp32_camera_initialization(String &diagCfgRES, String &diagHFLIP, String &diagVFLIP, String &diagQUAL, String &diagCfgAEC2, String &diagCfgXCLK) {
  Serial.printf("\nCamera Subsystem Initialization:\n================================\n");
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

  #if defined(LED_GPIO_NUM)
    setupLedFlash(LED_GPIO_NUM);
  #endif

  sensor_t *s = esp_camera_sensor_get();

  diagCfgRES   = readConfig("/cfg-res.ini",  "cameraserver"); delay(10);
  diagHFLIP    = readConfig("/cfg-hflp.ini", "cameraserver"); delay(10);
  diagVFLIP    = readConfig("/cfg-vflp.ini", "cameraserver"); delay(10);
  diagQUAL     = readConfig("/cfg-qual.ini", "cameraserver"); delay(10);
  diagCfgAEC2  = readConfig("/cfg-aec2.ini", "cameraserver"); delay(10);
  diagCfgXCLK  = readConfig("/cfg-xclk.ini", "cameraserver"); delay(10);
  diagCfgRES.trim();  diagCfgRES.trim(); diagHFLIP.trim(); diagVFLIP.trim(); diagQUAL.trim(); diagCfgAEC2.trim(); diagCfgXCLK.trim();

  if      (diagCfgRES == "FRAMESIZE_UXGA"){ s->set_framesize(s, FRAMESIZE_UXGA); Serial.println("cameraserver: set_framesize = FRAMESIZE_UXGA (1600x1200)"); }
  else if (diagCfgRES == "FRAMESIZE_SXGA"){ s->set_framesize(s, FRAMESIZE_SXGA); Serial.println("cameraserver: set_framesize = FRAMESIZE_UXGA (1280x1024)"); }
  else if (diagCfgRES == "FRAMESIZE_HD")  { s->set_framesize(s, FRAMESIZE_HD);   Serial.println("cameraserver: set_framesize = FRAMESIZE_UXGA (1280x720)"); }
  else if (diagCfgRES == "FRAMESIZE_XGA") { s->set_framesize(s, FRAMESIZE_XGA);  Serial.println("cameraserver: set_framesize = FRAMESIZE_UXGA (1024x768)"); }
  else if (diagCfgRES == "FRAMESIZE_SVGA"){ s->set_framesize(s, FRAMESIZE_SVGA); Serial.println("cameraserver: set_framesize = FRAMESIZE_SVGA (800x600)"); }
  else if (diagCfgRES == "FRAMESIZE_VGA") { s->set_framesize(s, FRAMESIZE_VGA);  Serial.println("cameraserver: set_framesize = FRAMESIZE_UXGA (640x480)"); }
  else    { s->set_framesize(s, FRAMESIZE_UXGA); }

  s->set_xclk(s, LEDC_TIMER_0, diagCfgXCLK.toInt());
  s->set_aec2(s, diagCfgAEC2.toInt());
  s->set_vflip(s, diagVFLIP.toInt());
  s->set_hmirror(s, diagHFLIP.toInt());
  s->set_quality(s, diagQUAL.toInt());
}
