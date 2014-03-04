#include <HMC5883Llib.h>
#include <Wire.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


//define gps tx rx pins
#define RXPIN 2   
#define TXPIN 3
#define GPSBAUD 4800

//define global variables
double heading; //init var for heading

// create objects

TinyGPS gps;
SoftwareSerial nss(RXPIN, TXPIN);    
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Magnetometer mag;
bool fail;

void setup() {
  Serial.begin(115200);
  nss.begin(4800);
  lcd.begin(16, 2);
  Serial.println("Reading GPS");
  lcd.setCursor(0,0);
  lcd.print("Reading GPS");
   mag.setGain(HMC5833L_GAIN_1370);
  
  
  if (mag.begin() != 0)
    {
        Serial.println("Error connecting to Magnetometer");
        lcd.print("Error connecting to Magnetometer");
        fail = true;
        return;
    }
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
  
      // reads the heading in degrees using the X and Y axis
    
    int8_t ret = mag.readHeadingDeg(&heading);

    switch (ret)
    {
        case HMC5833L_ERROR_GAINOVERFLOW:
            Serial.println("Gain Overflow");
            return;
        case 0:
            // success
            break;
        default:
            Serial.println("Failed to read Magnetometer");
            return;
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
  
  
   // print out headings
    Serial.print("Heading: ");
    Serial.print(heading);
    Serial.println(" degrees");
    
    lcd.print("Heading: ");
    lcd.print(heading);
    lcd.println(" degrees");

    delay(50);
}




// Feed data as it becomes available 
bool feedgps() {
  while (nss.available()) {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
}
