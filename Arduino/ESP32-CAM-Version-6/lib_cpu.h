

void esp32_cpusvc_initialization(String &diagCPU) {
  Serial.printf("\nESP32 CPU Subsystem Initialization:\n===================================\n");
  diagCPU = readConfig("/cpu.ini", "esp32-cam"); delay(10); diagCPU.trim();  delay(200);
  Serial.print("esp32-cam: Setting CPU Speed to ");    Serial.println(diagCPU);  setCpuFrequencyMhz(diagCPU.toInt());  delay(10);
  Serial.print("esp32-cam: XTAL - Crystal Frequency: "); Serial.print(getXtalFrequencyMhz());  Serial.println(" MHz");   delay(10);
  Serial.print("esp32-cam: CPU  - Frequency: ");          Serial.print(getCpuFrequencyMhz());   Serial.println(" MHz");   delay(10);
  Serial.print("esp32-cam: APB  - Bus Frequency: ");      Serial.print(getApbFrequency());      Serial.println(" Hz");    delay(10);
}
