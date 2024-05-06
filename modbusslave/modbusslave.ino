String readString;

#include <Wire.h> 
#include  <TimerOne.h>          

#include <ModbusRtu.h>
#define ID   1 // slave id 

uint16_t au16data[10];
Modbus bus;   
int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  bus = Modbus(1,Serial1,2);  
  bus.begin(9600);

}



void loop() {
  bus.poll( au16data,10);
  au16data[1] = random(0,100);  

  counter = counter + 1;
  Serial.println(au16data[0]);
  //delay(1000);
}
