#include <FastLED.h>


#define NUM_LEDS 90


#define DATA_PIN 3
#define CLOCK_PIN 13


CRGB leds[NUM_LEDS];

CRGB cores[] = { CRGB::Red, CRGB::BurlyWood, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::AliceBlue, CRGB::GreenYellow, CRGB::Pink, CRGB::OrangeRed, CRGB::Violet };

int corAtual = -1;
int qtdeCores = sizeof(cores)/3;

void setup() { 
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    Serial.begin(115200);
    Serial.print(qtdeCores);
    Serial.println(" cores");
}

void loop() { 
  ida();
  volta();
}

void ida(){
    corAtual++;
    corAtual = corAtual < qtdeCores ? corAtual : 0;
    Serial.println(corAtual);
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = cores[corAtual];
      FastLED.show();
      delay(20);
      leds[i] = CRGB::Black;  
      FastLED.show();
      delay(20);
    }
}

void volta(){
    for(int i=NUM_LEDS-1; i>=0; i--){
      leds[i] = cores[corAtual];
      FastLED.show();
      delay(20);
      leds[i] = CRGB::Black;  
      FastLED.show();
      delay(20);
    }
}

