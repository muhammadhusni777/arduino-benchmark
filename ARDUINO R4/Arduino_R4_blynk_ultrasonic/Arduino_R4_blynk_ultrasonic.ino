uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6neSyoKz4"
#define BLYNK_TEMPLATE_NAME "ArduinoR4"
#define BLYNK_AUTH_TOKEN "0-S54gofntoxioABreYSw9uM3cnbFXp2"

#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>

// Your WiFi credentials.
// Set password to "" for open networks.
/*
char ssid[] = "Syergie Indo Prima";
char pass[] = "cahayaharapanhati";
*/
char ssid[] = "Wifihusni12";
char pass[] = "12345678";

BlynkTimer timer;

#include <ArduinoR4DigitDisplay.h>

#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

int pinValue;
int led_status;

int led2_status;

const int trigPin = 6;
const int echoPin = 7;
float duration;
int distance;
int distance_prev;


void clear_frame() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 12; col++) {
      frame[row][col] = 0;
    }
  }
}

BLYNK_WRITE(V0){
  pinValue = param.asInt();

  
}

BLYNK_WRITE(V3){
  led_status = param.asInt();  
  digitalWrite(13, led_status);
}

BLYNK_WRITE(V4){
  led2_status = param.asInt();
  digitalWrite(8, led2_status);
}

void sendSensor(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  
  Blynk.virtualWrite(V1, distance);
  Blynk.virtualWrite(V2, random(0,100));

  
}





void setup()
{
  // Debug console
  Serial.begin(9600);
  matrix.begin();
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  timer.setInterval(1000L, sendSensor);

}

void loop()
{
  Blynk.run();
  timer.run();
  Serial.println(pinValue);
  drawNumber(distance ,frame);
  matrix.renderBitmap(frame, 8, 12);

  if (distance != distance_prev){
    clear_frame();
    Serial.println("clear");
  }
  delay(500);
}
