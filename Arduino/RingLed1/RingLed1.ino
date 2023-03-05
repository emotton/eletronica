#include <Adafruit_NeoPixel.h>

#define PIN 9
#define NUMPIXELS 12
int delayval = 50;
int currentColor = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(100);
  strip.show(); 
}

void loop() {
  colorir(180,0,0);
  colorir(0,200,0);
  colorir(0,0,200);
}

void colorir(int r, int g, int b){
  for(int i=0; i<NUMPIXELS; i++){
     strip.setPixelColor(i, strip.Color(r,g,b));
     strip.show();
     delay(delayval);
  }
}
