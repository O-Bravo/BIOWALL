#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


#include <BlynkSimpleEsp32.h>
#include "DHT.h"
// You  get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon). 
char auth[] = "1oRviXjgLJBPaNXqwSwJF1yR5RVSyNbO";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Bravo";
char pass[] = "255141A2";

BlynkTimer timer;

#define RelayPin7 25  //D25
#define RelayPin8 26  //D26

#define SwitchPin7 15  //D15
#define SwitchPin8 4   //D4

#define wifiLed    2   //D2

#define VPIN_BUTTON_7    V7 
#define VPIN_BUTTON_8    V8

int toggleState_7 = 1; //Define integer to remember the toggle state for relay 7
int toggleState_8 = 1; //Define integer to remember the toggle state for relay 8

int wifiFlag = 0;


void relayOnOff(int relay){

    switch(relay){
            case 7: 
             if(toggleState_7 == 1){
              digitalWrite(RelayPin7, LOW); // turn on relay 7
              toggleState_7 = 0;
              Serial.println("Device7 ON");
              }
             else{
              digitalWrite(RelayPin7, HIGH); // turn off relay 7
              toggleState_7 = 1;
              Serial.println("Device7 OFF");
              }
             delay(100);
      break;
      case 8: 
             if(toggleState_8 == 1){
              digitalWrite(RelayPin8, LOW); // turn on relay 8
              toggleState_8 = 0;
              Serial.println("Device8 ON");
              }
             else{
              digitalWrite(RelayPin8, HIGH); // turn off relay 8
              toggleState_8 = 1;
              Serial.println("Device8 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void with_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin7) == LOW){
      delay(200);
      relayOnOff(7);
      Blynk.virtualWrite(VPIN_BUTTON_7, toggleState_7);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(200);
      relayOnOff(8);
      Blynk.virtualWrite(VPIN_BUTTON_8, toggleState_8);   // Update Button Widget
    }
}
void without_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin7) == LOW){
      delay(200);
      relayOnOff(7);
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(200);
      relayOnOff(8);
    }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_7);
  Blynk.syncVirtual(VPIN_BUTTON_8);
}
// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_7) {
  toggleState_7 = param.asInt();
  digitalWrite(RelayPin7, toggleState_7);
}

BLYNK_WRITE(VPIN_BUTTON_8) {
  toggleState_8 = param.asInt();
  digitalWrite(RelayPin8, toggleState_8);
}


void checkBlynkStatus() { // called every 3 seconds by SimpleTimer
  
    bool isconnected = Blynk.connected();
    if (isconnected == false) {
      wifiFlag = 1;
      digitalWrite(wifiLed, LOW); //Turn off WiFi LED
    }
    if (isconnected == true) {
      wifiFlag = 0;
      digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
    }
  }
  void setup()
  {
    Serial.begin(9600);

      pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

  pinMode(wifiLed, OUTPUT);

    pinMode(SwitchPin7, INPUT_PULLUP);
  pinMode(SwitchPin8, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
    WiFi.begin(ssid, pass);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(auth);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
  }
  else
  {
    Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
