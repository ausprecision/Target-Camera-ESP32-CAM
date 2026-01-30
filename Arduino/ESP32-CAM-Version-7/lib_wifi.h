#include <WiFi.h>
#include "esp_wifi.h"
#include "lib_wifi_scan.h"


void wifi_espwifi_initialization(String &diagWiFiPOWR, String &diagWiFiSSID, String &diagWiFiPASS, String &diagWiFiCHAN, String &diagWiFiPROT) {
  Serial.printf("\nWireless Subsystem Initialization:\n==================================\n");
  diagWiFiCHAN = readConfig("/wifichan.ini", "wireless"); delay(10);
  diagWiFiPASS = readConfig("/wifipass.ini", "wireless"); delay(10);
  diagWiFiSSID = readConfig("/wifissid.ini", "wireless"); delay(10);
  diagWiFiPOWR = readConfig("/wifitxpw.ini", "wireless"); delay(10);
  diagWiFiCHAN.trim(); diagWiFiSSID.trim(); diagWiFiPASS.trim(); diagWiFiPOWR.trim();
  
  Serial.print("wireless: SSID = "); Serial.println(diagWiFiSSID);
  Serial.print("wireless: PASS = "); Serial.println(diagWiFiPASS);
  Serial.print("wireless: TX_POWER_CFG = ");  Serial.println(diagWiFiPOWR);  diagWiFiPOWR.trim();

  if (diagWiFiCHAN == "0") { WiFiSurveyScan(diagWiFiCHAN); }
  // lib wifi scan goes here!
  
  WiFi.mode(WIFI_AP); 
  esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G);
  esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_BW_HT20);
  WiFi.softAP(diagWiFiSSID, diagWiFiPASS,diagWiFiCHAN.toInt(),0,4);  // Set up the SoftAP

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
}

  // ESP32 WiFi Power Saving Code - For Reference Only, Does not impact upon wifi performance, distance, bandwidth, etc.
  // =============================
  // esp_wifi_set_ps(WIFI_PS_NONE);
  // wifi_ps_type_t current_ps_mode;
  // esp_err_t ret = esp_wifi_get_ps(&current_ps_mode);
  // if (ret == ESP_OK) { switch(current_ps_mode) { case WIFI_PS_NONE: Serial.println("wireless: WIFI_PS_NONE = OK!"); } }
