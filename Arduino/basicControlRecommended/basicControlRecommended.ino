#include <QC3Control.h>

//Pin 4 for Data+
//Pin 5 for Data-

//See How to connect in the documentation for more details.
QC3Control quickCharge(4, 5);

void setup() {
  //Optional
  Serial.begin(115200);
  Serial.println("Iniciou");
  //quickCharge.begin();

  //set voltage to 12V
  quickCharge.set12V();
  Serial.println("12V");

  delay(5000);
}

void loop() {
  //And you can change it on the fly
  delay(5000);
  quickCharge.set9V();
  Serial.println("9V");
  delay(5000);
  quickCharge.set5V();
  Serial.println("5V");
  delay(5000);
  // quickCharge.setMilliVoltage(6000);
  // delay(10000);
  // for (int i = 0; i < 10; i++) quickCharge.decrementVoltage();
  // delay(10000);
  quickCharge.set12V();
  Serial.println("12V");
}