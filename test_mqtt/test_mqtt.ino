/*
SMART STREET LIGHTING
Written by : Husni and Fandi

MQTT CONNECTION to PHASE ANGLE CONTROL LEADING EDGE DIMMER

*/

#include<avr/wdt.h>

#include <Wire.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


//#include  <TimerOne.h>   

volatile int i=0;               
volatile boolean zero_cross=0;  
int AC_pin = 6;                
int alpha = 0;                                          
int freqStep = 75;    
int zc_condition;
int saturation = 128;
int alpha_buff = 125;

int reset_pin = 7;

unsigned long message_time;
unsigned long message_time_prev;


unsigned long time_now;
unsigned long time_elapsed;
unsigned long time_prev;

unsigned long time_send;
unsigned long time_send_prev;
int time_message = 1000;

int azimuth =0;

int val;
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xE6 }; 

IPAddress ip(123,45,0,103);
IPAddress server(123,45,0,10);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
    
  }

   if (String(topic) == "lamp1") {
   alpha = messageTemp.toInt(); 
   Serial.println(alpha);
   }
   

 messageTemp ="";

}


EthernetClient ethClient;
PubSubClient client(ethClient);
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("SSL_Client")) {
      Serial.println("connected");
      client.subscribe("lamp1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      digitalWrite(reset_pin, HIGH);
    }
  } 
}



void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("booting....");
  client.setServer(server, 1883);
  client.setCallback(callback);


  pinMode (5, OUTPUT);
  Ethernet.begin(mac, ip);
  delay(1500);
  wdt_enable(WDTO_8S);
}


static char yaw_send[15];
void loop() {
  time_now = millis();

  analogWrite(5, alpha);
  
  if (!client.connected()) {
    reconnect();
  }
  
  time_send = millis() - time_send_prev;
  if (time_send > time_message){
  client.publish("system","on");
  time_send_prev = millis();
  }
  
  client.loop();
  time_elapsed = time_now - time_prev;
  time_prev = time_now;

  wdt_reset();
}  
