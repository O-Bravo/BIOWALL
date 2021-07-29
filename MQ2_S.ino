
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;
 
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1oRviXjgLJBPaNXqwSwJF1yR5RVSyNbO";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Bravo";
char pass[] = "255141A2";
 
#define MQ2 34
#define GREEN 16
#define RED 17
int sensorValue = 0;
boolean state = false;
 
void setup()
{
 // Debug console
 Serial.begin(115200);
 Blynk.begin(auth, ssid, pass);
 pinMode(MQ2, INPUT);
 pinMode(GREEN, OUTPUT);
 pinMode(RED, OUTPUT);
 timer.setInterval(1000L, sendUptime);
}
 
void sendUptime()
{
 
 sensorValue = analogRead(MQ2);
 Blynk.virtualWrite(V1, sensorValue);
 Serial.println(sensorValue);
 
 if (sensorValue > 600)
 {
 Blynk.notify("Gas Detected!");
 digitalWrite(GREEN, LOW);
 digitalWrite(RED, HIGH);
 }
 
 else
 {
 digitalWrite(GREEN, HIGH);
 digitalWrite(RED, LOW);
 }
}
 
void loop()
{
 Blynk.run();
 timer.run();
}
