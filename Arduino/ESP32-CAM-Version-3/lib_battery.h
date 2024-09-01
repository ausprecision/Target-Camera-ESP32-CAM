#define BATTV_MAX           6.2     // maximum voltage of battery (was 6.2)
#define BATTV_MIN           4.4     // what we regard as an empty battery

// 6.2v = 100% | 5.3v = 50% | 4.8 = 25% | 4.4v = 0%

#define LED_GRN  14
#define LED_YEL  15
#define LED_RED  13
#define LED_BLU  2

int batt_samples;
int batt_counter;

float   battv;         // battery level, in Volts
uint8_t battpc;      // battery level, in percentage
int     batts;
String  str_battery;
String  str_battery_min;



void battery_init() {
  Serial.println("battery_init: pin modes, writing pins");
  pinMode(2, OUTPUT); // Set the pin as output
  pinMode(13, OUTPUT); // Set the pin as output
  pinMode(14, OUTPUT); // Set the pin as output
  pinMode(15, OUTPUT); // Set the pin as output

  pinMode(12, INPUT);

  digitalWrite(LED_BLU, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);

  delay(1000);

  digitalWrite(LED_BLU, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);

  delay(250);

  digitalWrite(2, LOW); //Turn on
  digitalWrite(13, LOW); //Turn on
  Serial.println("battery_init: completed");

  digitalWrite(2, LOW); //Turn on
  digitalWrite(13, LOW); //Turn on

}

void battery_read(String &strBatteryData) {
  Serial.println("battery_read: starting");
  batt_counter = 0;
  batt_samples = 0;
    
   for (int i = 0; i <= 1000; i++) {
    batt_samples = batt_samples + analogRead(12);
    batt_counter = batt_counter+1;
    
    if(batt_counter >= 1000) {
      batts = batt_samples/1000;
      //battv = ((float)batts/1000) * 3.3 * 1.05;  
      //battv = ((float)batts/1000) * 3.3 * 1.01;  
      battv = ((float)batts/1000) * 3.3;  
  
      battpc = (uint8_t)(((battv - BATTV_MIN) / (BATTV_MAX - BATTV_MIN)) * 100);
      str_battery = "Analogue Read Over 1000 Samples: " + (String)(batts)  + " (div 1000) " + (String)((float)(batts/1000)) + " | Voltage: " + ((String)(battv)) + "v | Percentage: " + ((String)(battpc)) + "%";
      
      //str_battery_min = (String)(batts) + ":" + ((String)(battv)) + "v:" + ((String)(battpc)) + "%";
      str_battery_min = (String)(batts);

//1318 = 4.5v (will boot)
//1258 = 4.3v (will not boot)
      
      switch (batt_samples/1000) {
        case 0 ... 1000:
          digitalWrite(LED_BLU, LOW);
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GRN, LOW);
          digitalWrite(LED_YEL, LOW);
          //esp_deep_sleep_start();
        case 1001 ... 1200:
          digitalWrite(LED_RED, HIGH);
          digitalWrite(LED_GRN, LOW);
          digitalWrite(LED_YEL, LOW);
        break;
        case 1201 ... 1500:   // 4.99v and below
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GRN, LOW);
          digitalWrite(LED_YEL, HIGH);
        break;
        case 1501 ... 2500: // 5.00v and above
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GRN, 100);
          digitalWrite(LED_YEL, LOW);
        break;
    }
      batt_counter = 0;
      batt_samples = 0;
      Serial.println(str_battery);
      strBatteryData = str_battery_min;
      
    } else { batt_counter++; }
  }
}
