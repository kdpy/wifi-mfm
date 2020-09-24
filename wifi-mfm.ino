#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_INA219.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "******firebaseio.com" //your firebase host name
#define FIREBASE_AUTH "***************************" //token will be found in firebase setting


#define WIFI_SSID "************" // Change the name of your WIFI
#define WIFI_PASSWORD "**********" // Change the password of your WIFI

//"now < 1601490600000"
//"now < 1601490600000"
//int WIFI_SSID[] = {"JioFi2_A8C3EE","ab17-4534-c815"};
//int WIFI_PASSWORD[] = {"rk8uvkwkbu","12345678"};



Adafruit_INA219 ina219;

unsigned long previousMillis = 0;
unsigned long interval = 100;
const int chipSelect = 10;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float energy = 0;



void setup() {
   Serial.begin(9600);
   WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) 
   {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("");
  Serial.println ("WiFi Connected!");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Wire.begin(0, 2); // SDA, SDL
  ina219.begin();  
}

void loop() {
 

  
 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    
    ina219values();
  
  Firebase.setFloat ("current",current_mA);
  Firebase.setFloat ("voltage",loadvoltage);
  Firebase.setFloat ("energy",energy);
   
  
  // handle error 
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }    
  }
 /* if(Firebase.getInt("reset"))
 {
     Firebase.setInt("reset",0);
      delay(100);
     Serial.println("Resetting ESP");
     ESP.restart(); //ESP.reset();
  }*/
}

void ina219values() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  energy = energy + loadvoltage * current_mA / 3600;
}
  
