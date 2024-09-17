#include "Arduino.h"
#include <WiFi.h>
#include <WiFiUDP.h>


bool udpSendMessage(IPAddress ipAddr, String udpMsg, int udpPort) {
  /** WiFiUDP class for creating UDP communication */
  //Serial.print("UDP Start... Send Command/Data..."); 
  //Serial.println(udpMsg);
  WiFiUDP udpServer;
  int connOK = udpServer.begin(2090); // Start UDP client for sending packets
   if (connOK == 0) {
    Serial.println("UDP could not get socket");
    return false;
  }
 
  int beginOK = udpServer.beginPacket(ipAddr, udpPort);
   if (beginOK == 0) { // Problem occured!
    udpServer.stop();
    Serial.println("UDP connection failed");
    return false;
  }
  else
  {
  int bytesSent = udpServer.print(udpMsg);
  if (bytesSent == udpMsg.length()) {
    udpServer.endPacket();
    udpServer.stop();
    return true;
  } else {
    Serial.println("Failed to send " + udpMsg + ", sent " + String(bytesSent) + " of " + String(udpMsg.length()) + " bytes");
    udpServer.endPacket();
    udpServer.stop();
    Serial.println("UDP Server Stopping...");
    return false;
  }
  }
}
