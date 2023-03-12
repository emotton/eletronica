#include <SoftwareSerial.h>

SoftwareSerial serialNextion(8,9); // RX, TX
#define pinLed 13
bool ledEstado = false;

void setup() {
  Serial.begin(115200);
  serialNextion.begin(9600);
  pinMode(pinLed, OUTPUT);
}

void loop() {
  if(serialNextion.available()){
    String recebido = serialNextion.readString();
    Serial.println(recebido);
    // if (recebido == "botao"){
      ledEstado = !ledEstado;
      digitalWrite(pinLed, ledEstado?HIGH:LOW);
    // }
  }

}
