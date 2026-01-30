#include <WiFi.h>


void WiFiSurveyScan(String &diagWiFiCHAN){
  WiFi.mode(WIFI_STA);      // Set ESP32 to station mode to scan for networks
  WiFi.disconnect(true);    // Disconnect from any previous Wi-Fi

  int wifichan[14]          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int wificongestion[14]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int congestionpercent[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int totalcongestion       = 0;

  int  SearchArraySize      = 0;
  bool SearchFound          = false;
  int  SearchFoundIndex     = -1;
  int  CurrentIndex         = 0;
  int  totalScans           = 0;
  char wifinames[20][32]    = {"","","","","","","","","","","","","","","","","","","",""};

  // Run 3 Scans of the Available Networks
  // =====================================
  // - During each scan, it will skip duplicate networks already detected during previous scans.
  // - Process will then calculate how many networks are on each channel.
  // - Process then calculates how congested the non-overlapping channels 1, 6 and 11 are.
  // - Process then determines the best channel out of 1, 6 and 11.
  
  for (int z = 1; z <= 3; z++) {
    int n = WiFi.scanNetworks();
    Serial.printf("Executing Scan %d: Found Total of %d Networks\n", z, n);
    if (n == 0) { Serial.println("No networks found."); } 
    else {
    // Iterate through the found networks
      for (int i = 0; i < n; i++) {
        SearchArraySize = sizeof(wifinames) / sizeof(wifinames[0]);
        String tempSSID = WiFi.SSID(i) + " CH:" + WiFi.channel(i);
        for (int x = 0; x < SearchArraySize; x++) { if (strcmp(wifinames[x],tempSSID.c_str()) == 0) { SearchFound = true; SearchFoundIndex = x; break; } } // Exit loop once found
        if (!SearchFound) {
          if (CurrentIndex < 20) {
            String currentSSID = WiFi.SSID(i) + " CH:" + WiFi.channel(i);
            //String currentSSID = WiFi.SSID(i);
            currentSSID.toCharArray(wifinames[CurrentIndex], sizeof(wifinames[CurrentIndex]));
            CurrentIndex = CurrentIndex+1;
            if (WiFi.channel(i) == 1) { wifichan[1]++; }
            if (WiFi.channel(i) == 2) { wifichan[2]++; }
            if (WiFi.channel(i) == 3) { wifichan[3]++; }
            if (WiFi.channel(i) == 4) { wifichan[4]++; }
            if (WiFi.channel(i) == 5) { wifichan[5]++; }
            if (WiFi.channel(i) == 6) { wifichan[6]++; }
            if (WiFi.channel(i) == 7) { wifichan[7]++; }
            if (WiFi.channel(i) == 8) { wifichan[8]++; }
            if (WiFi.channel(i) == 9) { wifichan[9]++; }
            if (WiFi.channel(i) == 10) { wifichan[10]++; }
            if (WiFi.channel(i) == 11) { wifichan[11]++; }
            if (WiFi.channel(i) == 12) { wifichan[12]++; }
            if (WiFi.channel(i) == 13) { wifichan[13]++; }
          }
        //Serial.printf("SSID: %s, RSSI: %d dBm, Channel: %d, Encryption: %d\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i), WiFi.encryptionType(i));
        }
        SearchFound = false;
        SearchFoundIndex = -1;
      }
    }
  }

  Serial.printf("\nList of Networks Over 3 Scans:\n=============================\n");
  for (int i = 0; i <= (CurrentIndex -1); i++) { Serial.print("SSID: "); Serial.println(wifinames[i]); } 
  for (int x = 1; x < 5; x++)  { if (wifichan[x] > 0) { wificongestion[1]  = wificongestion[1]  + wifichan[x]; } }   // Channel 01 - Interferrance/Congestion
  for (int x = 4; x < 9; x++)  { if (wifichan[x] > 0) { wificongestion[6]  = wificongestion[6]  + wifichan[x]; } }   // Channel 06 - Interferrance/Congestion
  for (int x = 8; x < 14; x++) { if (wifichan[x] > 0) { wificongestion[11] = wificongestion[11] + wifichan[x]; } }   // Channel 11 - Interferrance/Congestion

  totalcongestion = wificongestion[1] + wificongestion[6] + wificongestion[11];
  congestionpercent[1] =  (wificongestion[1] * 100)  / totalcongestion;
  congestionpercent[6] =  (wificongestion[6] * 100)  / totalcongestion;
  congestionpercent[11] = (wificongestion[11] * 100) / totalcongestion;
  
  Serial.printf("\nTotal Networks on Channels:\n=============================\n");
  for (int i = 0; i <= 13; i++) { if (wifichan[i] > 0) { Serial.printf("Total Networks on Channel %01d: %d\n", i, wifichan[i]); wifichan[i] = 0; }  }
  Serial.printf("\nCongestion Report:\n=============================\n");
  Serial.printf("Total Congestion for Channel 01: %d (%01d%)\n", wificongestion[1],  congestionpercent[1]);
  Serial.printf("Total Congestion for Channel 06: %d (%01d%)\n", wificongestion[6],  congestionpercent[6]);
  Serial.printf("Total Congestion for Channel 11: %d (%01d%)\n", wificongestion[11], congestionpercent[11]);

  if      (congestionpercent[1] <= congestionpercent[6]  && congestionpercent[1] <= congestionpercent[11])  { diagWiFiCHAN = "1"; Serial.printf("Channel 1 Has Lowest Congestion\n"); }
  else if (congestionpercent[6]  <= congestionpercent[1] && congestionpercent[6]  <= congestionpercent[11]) { diagWiFiCHAN = "6"; Serial.printf("Channel 06 Has Lowest Congestion\n"); }
  else    { diagWiFiCHAN = "11"; Serial.printf("Channel 11 Has Lowest Congestion\n"); }
  
  Serial.printf("\n\nFinished Wi-Fi Survey Scan\n\n\n");
}
