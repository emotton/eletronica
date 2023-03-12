#include "Nextion.h"

NexDSButton bt0 = NexDSButton(0,2,"bt0");
NexDSButton bt1 = NexDSButton(0,3,"bt1");

#define pinLed 2
#define pinLed2 4
uint32_t ds_var; 
uint32_t ds_var2; 

void setup() {
  nexInit();
  pinMode(pinLed, OUTPUT);
  pinMode(pinLed2, OUTPUT);
}

void loop() {
   bt0.getValue(&ds_var);
   if(ds_var>0) digitalWrite(pinLed, HIGH);
   else digitalWrite(pinLed, LOW);
   bt1.getValue(&ds_var2);
   if(ds_var2>0) digitalWrite(pinLed2, HIGH);
   else digitalWrite(pinLed2, LOW);

}
