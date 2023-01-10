#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial HC05(6, 7); /* RX pino 7 , TX pino 6 */

#define PIN_LED 13

char saida;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200); /* Porta Serial do computador */
  HC05.begin(115200);   /* Porta Serial do HC05 */
  Serial.println("Envie os comandos AT para configurar o HC05");

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(40, 10);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("INICIADO");
  display.display();
}

void loop()
{
  if (HC05.available())
  { /* Enquanto HC05 receber dados */
    while (HC05.available())
    {
      char c = HC05.read(); /* Lê os caractéres */

     if (c == 'H' || c == 'h')
      {
        digitalWrite(PIN_LED, HIGH);
        saida = c;
      }
      else if (c == 'L' || c == 'l')
      {
        digitalWrite(PIN_LED, LOW);
        saida = c;
      }

      Serial.write(c); /* Escreve os dados na Serial */

      display.clearDisplay();
      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE); // Draw white text
      display.setCursor(40, 10);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.write(saida=='H'?"LIGADO":"DESLIGADO");
      display.display();

    }
  }

  if (Serial.available())
  { /* Enquanto a Serial receber dados */
    delay(10);
    String comando = "";

    while (Serial.available())
    { /* Enquanto receber comandos */

      comando += (char)Serial.read(); /* Lê os caracteres */
    }
    Serial.println(comando);
    HC05.println(comando); /* Escreve os dados no HC05 */
  }
}