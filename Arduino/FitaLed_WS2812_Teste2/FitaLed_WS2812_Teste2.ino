#include <FastLED.h>


#define NUM_LEDS 90


#define DATA_PIN 3
#define CLOCK_PIN 13


CRGB leds[NUM_LEDS];

CRGB cores[] = { CRGB::Red, CRGB::HotPink, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::AliceBlue, CRGB::Brown, CRGB::Pink, CRGB::OrangeRed, CRGB::Violet };

int corAtual = 0;

void setup() { 
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void loop() { 
  ida();
  volta();
}

void ida(){
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = cores[cor];
      FastLED.show();
      delay(20);
      leds[i] = CRGB::Black;  
      FastLED.show();
      delay(20);
    }
}

void volta(){
    for(int i=NUM_LEDS-1; i>=0; i--){
      leds[i] = cores[cor];
      FastLED.show();
      delay(20);
      leds[i] = CRGB::Black;  
      FastLED.show();
      delay(20);
    }
}

