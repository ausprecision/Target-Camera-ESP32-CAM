

void udpSendConfig(String diagWiFiSSID, String diagWiFiPASS) { 
  IPAddress temp_IP(192,168,4,255);
  String udpMessageConfig = "[WIRELESS] | " + diagWiFiSSID + " | " + diagWiFiPASS; 
  Serial.print("udpserver_tx: "); 
  udpSendMessage(temp_IP,udpMessageConfig,2090); 
  Serial.println(udpMessageConfig); 
}

void process_command_tel(String str, ESPTelnet &telnet, String &diagCfgRES, String &diagCfgAEC2, String &diagCfgXCLK, String &diagQUAL, String &diagHFLIP, String &diagVFLIP, String &diagCPU, String &diagWiFiPOWR, String &diagWiFiCHAN, String &diagWiFiSSID, String &diagWiFiPASS ){
  int res = 0;
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
  else if(str.indexOf("camera set quality") >=0)  { res = s->set_quality(s, str.substring(19).toInt()); diagQUAL = str.substring(19).toInt(); if (res == 0) { tel_ok_msg(telnet, "camera: set_qual: " + str.substring(19) + " (OK!)"); receivecfg("/camera/cfg-qual.txt",str.substring(19),"cameras: set_qual " + str.substring(19) + " (OK!)"); } else { tel_fail(telnet,"camera: set_qual (FAILED!)"); } }  

  else  if(str == "cpu set 240") { if (setCpuFrequencyMhz(240) == true) { delay(200); diagCPU = "240"; telnet.println(""); telnet.print("esp32-cam: CPU Frequency: "); telnet.print(getCpuFrequencyMhz()); telnet.println(" MHz (OK!)"); telnet.println(""); telnet.print("C:\\>"); receivecfg("/cpu.ini","240", ""); } else { tel_fail(telnet, "esp32-cam: set_cpu (FAILED!)"); } }
  else  if(str == "cpu set 160") { if (setCpuFrequencyMhz(160) == true) { delay(200); diagCPU = "160"; telnet.println(""); telnet.print("esp32-cam: CPU Frequency: "); telnet.print(getCpuFrequencyMhz()); telnet.println(" MHz (OK!)"); telnet.println(""); telnet.print("C:\\>"); receivecfg("/cpu.ini","160", ""); } else { tel_fail(telnet, "esp32-cam: set_cpu (FAILED!)"); } }
  else  if(str == "cpu set 80")  { if (setCpuFrequencyMhz(80) == true)  { delay(200); diagCPU = "80";  telnet.println(""); telnet.print("esp32-cam: CPU Frequency: "); telnet.print(getCpuFrequencyMhz()); telnet.println(" MHz (OK!)"); telnet.println(""); telnet.print("C:\\>"); receivecfg("/cpu.ini","80", "");  } else { tel_fail(telnet, "esp32-cam: set_cpu (FAILED!)"); } }

  else if(str.indexOf("wireless get config")>=0)   { telnet.println(""); telnet.println("Sending wireless config via UDP"); udpSendConfig(diagWiFiSSID, diagWiFiPASS); telnet.println(""); telnet.print("C:\\>"); }
  else if(str.indexOf("wireless set channel") >=0) { diagWiFiCHAN = str.substring(21); receivecfg("/wifichan.ini", str.substring(21), "wireless: set_chan (OK!) Reboot Required"); tel_ok_msg(telnet, "wireless: set_chan: " + str.substring(21) + " (OK!) Reboot Required"); }
  else if(str.indexOf("wireless set name")    >=0) { diagWiFiSSID = str.substring(18); receivecfg("/wifissid.ini", str.substring(18), "wireless: set_ssid (OK!) Reboot Required"); tel_ok_msg(telnet, "wireless: set_ssid: " + str.substring(18) + " (OK!) Reboot Required"); }
  else if(str.indexOf("wireless set password")>=0) { diagWiFiPASS = str.substring(22); receivecfg("/wifipass.ini", str.substring(22), "wireless: set_pass (OK!) Reboot Required"); tel_ok_msg(telnet, "wireless: set_pass: " + str.substring(22) + " (OK!) Reboot Required"); }

  else if (str == "")  { telnet.println(""); telnet.print("C:\\>"); } 
  else                 { telnet.println(""); telnet.println("Bad command or file name"); telnet.println(""); telnet.print("C:\\>"); }
}

void process_command_udp(int &len, char packetBuffer[255], WiFiUDP &udpListener, String &diagCfgRES, String &diagCfgAEC2, String &diagCfgXCLK, String &diagQUAL, String &diagHFLIP, String &diagVFLIP, String &diagCPU, String &diagWiFiPOWR, String &diagWiFiCHAN, String &diagWiFiSSID, String &diagWiFiPASS ){
  int res = 0;
  packetBuffer[len] = 0; String str = String(packetBuffer); sensor_t *s = esp_camera_sensor_get(); 
  // TESTED - OK!
  if(str.indexOf("camera set res VGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_VGA);  if (res == 0) { diagCfgRES = "FRAMESIZE_VGA";  receivecfg("/cfg-res.ini","FRAMESIZE_VGA", "camera: set_famesize FRAMESIZE_VGA (OK!)");  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_VGA (FAILED!)"); } }  
  if(str.indexOf("camera set res SVGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SVGA); if (res == 0) { diagCfgRES = "FRAMESIZE_SVGA"; receivecfg("/cfg-res.ini","FRAMESIZE_SVGA","camera: set_famesize FRAMESIZE_SVGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SVGA (FAILED!)"); } }  
  if(str.indexOf("camera set res XGA") >=0)  { res = s->set_framesize(s, FRAMESIZE_XGA);  if (res == 0) { diagCfgRES = "FRAMESIZE_XGA";  receivecfg("/cfg-res.ini","FRAMESIZE_XGA", "camera: set_famesize FRAMESIZE_XGA (OK!)");  } else { Serial.println("cameraserver: set_famesize FRAMESIZE_XGA (FAILED!)"); } }  
  if(str.indexOf("camera set res HD") >=0)   { res = s->set_framesize(s, FRAMESIZE_HD);   if (res == 0) { diagCfgRES = "FRAMESIZE_HD";   receivecfg("/cfg-res.ini","FRAMESIZE_HD",  "camera: set_famesize FRAMESIZE_HD (OK!)");   } else { Serial.println("cameraserver: set_famesize FRAMESIZE_HD (FAILED!)"); } }  
  if(str.indexOf("camera set res SXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_SXGA); if (res == 0) { diagCfgRES = "FRAMESIZE_SXGA"; receivecfg("/cfg-res.ini","FRAMESIZE_SXGA","camera: set_famesize FRAMESIZE_SXGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_SXGA (FAILED!)"); } }  
  if(str.indexOf("camera set res UXGA") >=0) { res = s->set_framesize(s, FRAMESIZE_UXGA); if (res == 0) { diagCfgRES = "FRAMESIZE_UXGA"; receivecfg("/cfg-res.ini","FRAMESIZE_UXGA","camera: set_famesize FRAMESIZE_UXGA (OK!)"); } else { Serial.println("cameraserver: set_famesize FRAMESIZE_UXGA (FAILED!)"); } }  
  
  if(str.indexOf("camera set hflip on")  >=0) { res = s->set_hmirror(s, 1); diagHFLIP = "1"; receivecfg("/cfg-hflp.ini","1", "camera: set_hflip on (OK!)");  }
  if(str.indexOf("camera set hflip off") >=0) { res = s->set_hmirror(s, 0); diagHFLIP = "0"; receivecfg("/cfg-hflp.ini","0", "camera: set_hflip off (OK!)"); }
  if(str.indexOf("camera set vflip on")  >=0) { res = s->set_vflip(s, 1);   diagVFLIP = "1"; receivecfg("/cfg-vflp.ini","1", "camera: set_vflip on (OK!)");  }
  if(str.indexOf("camera set vflip off") >=0) { res = s->set_vflip(s, 0);   diagVFLIP = "0"; receivecfg("/cfg-vflp.ini","0", "camera: set_vflip off (OK!)"); }

  if(str.indexOf("camera set aec2 on")  >=0) { res = s->set_aec2(s, 1); diagCfgAEC2 = "1"; receivecfg("/cfg-aec2.ini","1", "camera: set_aec2 on (OK!)");  }
  if(str.indexOf("camera set aec2 off") >=0) { res = s->set_aec2(s, 0); diagCfgAEC2 = "0"; receivecfg("/cfg-aec2.ini","0", "camera: set_aec2 off (OK!)"); }

  if(str.indexOf("cpu set 240") >=0) { setCpuFrequencyMhz(240); diagCPU = "240"; Serial.print("esp32-cam: CPU Frequency: "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz (OK!)"); receivecfg("/cpu.ini","240", ""); }
  if(str.indexOf("cpu set 160") >=0) { setCpuFrequencyMhz(160); diagCPU = "160"; Serial.print("esp32-cam: CPU Frequency: "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz (OK!)"); receivecfg("/cpu.ini","160", ""); }
  if(str.indexOf("cpu set 80")  >=0) { setCpuFrequencyMhz(80);  diagCPU = "80";  Serial.print("esp32-cam: CPU Frequency: "); Serial.print(getCpuFrequencyMhz()); Serial.println(" MHz (OK!)"); receivecfg("/cpu.ini","80",  ""); }
  if(str.indexOf("reboot") >= 0)     { Serial.println("Rebooting in 5 seconds..."); delay(5000); ESP.restart(); }

  if(str.indexOf("camera set quality") >=0)  { res = s->set_quality(s, str.substring(19).toInt()); diagQUAL = str.substring(19).toInt(); if (res == 0) { receivecfg("/camera/cfg-qual.txt",str.substring(19),"camera: set_qual " + str.substring(19) + " (OK!)"); } else { Serial.println("cameraserver: set_qual (FAILED!)"); } }  
  if(str.indexOf("camera set xclk") >=0)     { res = s->set_xclk(s, LEDC_TIMER_0, str.substring(16).toInt()); diagCfgXCLK = str.substring(16).toInt(); if (res == 0) { receivecfg("/camera/cfg-xclx.txt",str.substring(16),"camera: set_xclk " + str.substring(16) + " (OK!)"); } else { Serial.println("cameraserver: set_xclk (FAILED!)"); } }  

  if(str.indexOf("wireless get config")>=0)   { Serial.println("wireless: sending wireless config via UDP"); udpSendConfig(diagWiFiSSID, diagWiFiPASS); }
  if(str.indexOf("wireless set channel") >=0) { diagWiFiCHAN = str.substring(21); receivecfg("/wifichan.ini", str.substring(21), "wireless: set_chan (OK!) Reboot Required"); }
  if(str.indexOf("wireless set name")    >=0) { diagWiFiSSID = str.substring(18); receivecfg("/wifissid.ini", str.substring(18), "wireless: set_ssid (OK!) Reboot Required"); }
  if(str.indexOf("wireless set password")>=0) { diagWiFiPASS = str.substring(22); receivecfg("/wifipass.ini", str.substring(22), "wireless: set_pass (OK!) Reboot Required"); }

  if(str.indexOf("wireless set power")   >=0) { 
    switch (str.substring(19).toInt()) {
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
