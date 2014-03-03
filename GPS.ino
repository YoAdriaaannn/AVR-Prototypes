  

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
               // Special version for 1.0
#define RXPIN 2   
#define TXPIN 3
#define GPSBAUD 4800
TinyGPS gps;
SoftwareSerial nss(RXPIN, TXPIN);    
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  Serial.begin(115200);
  nss.begin(4800);
  lcd.begin(16, 2);
  Serial.println("Reading GPS");
  lcd.setCursor(0,0);
  lcd.print("Reading GPS");
}

void loop() {
  bool newdata = false;
  unsigned long start = millis();
  while (millis() - start < 5000) {  // Update every 5 seconds
    if (feedgps())
      newdata = true;
  }
  if (newdata) {
    gpsdump(gps);
  }
}

// Get and process GPS data
void gpsdump(TinyGPS &gps) {
  float flat, flon;
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);
  Serial.print(flat, 5); Serial.print(", "); 
  Serial.println(flon, 5);
  lcd.setCursor(0,0);
  lcd.print(flat, 5); 
  lcd.print(", "); 
  lcd.print(flon, 5);
}

// Feed data as it becomes available 
bool feedgps() {
  while (nss.available()) {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
}
