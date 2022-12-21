#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <DigiPotX9Cxxx.h>
#include <IRremote.h>

#define SIZE_MEM 12

#define epsilon ((double)0.000999)
bool is_approximately_equal(double x, double y)
{
	return (abs(x - y) < epsilon);
}

LiquidCrystal_I2C lcd(0x21,20,4);

RTC_DS1307 rtc;

DigiPot potEsq(11,12,14);
DigiPot potDir(5,4,3);

unsigned char frequencyH = 0;
unsigned char frequencyL = 0;

unsigned int frequencyB;
double frequency = 0;
double tensao = 0;

int pinBuzzer = 13;
int pinButEsq = 9;
int pinButDir = 10;
int pinButRec = 7;
int pinButMode = 2;

int pinoLM35 = A1;

int RECV_PIN = 8;

IRrecv irrecv(RECV_PIN);

decode_results results;

double mem[SIZE_MEM] = {88.10, 89.10, 90.50, 90.90, 94.70, 95.30, 96.90, 97.70, 98.50, 100.10, 100.90, 101.70};
char* memDesc[SIZE_MEM] = {"GAZETA ", "89 FM ", "CBN ", "BANDEIRANTES ", "ANTENA 1 ", "NATIVA",
"BAND NEWS ", "97 FM ", "METROPOLITANA ", "TRANSAMERICA ", "JOVEMPAN ", "ALPHA FM "};

int memPoint = -1;

byte start = 0;
int modo = 0; // 0-Step, 1-Memória

int alarme = 0; // 0-Sem beep/1-Com beep (15/30/60) minutos

int volume = 0;

int volumeTempo = 0;

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};

uint8_t grau[8] = {
	B00100,
	B01010,
	B01010,
	B00100,
	B00000,
	B00000,
	B00000
};

uint8_t indicador1[8] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B11111
};

uint8_t indicador2[8] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B11111,
	B11111
};

uint8_t indicador3[8] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B11111,
	B11111,
	B11111
};

uint8_t indicador4[8] = {
	B00000,
	B00000,
	B00000,
	B11111,
	B11111,
	B11111,
	B11111
};

uint8_t indicador5[8] = {
	B00000,
	B00000,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111
};

uint8_t indicador6[8] = {
	B00000,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111
};

uint8_t indicador7[8] = {
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111
};

int horas = 0;
int minutos = 0;
int segundos = 0;

int latency = 0;

int valorLidoTemp = 0;
float temperatura = 0;

void setup()
{
	irrecv.enableIRIn();
	
	potEsq.reset();
	potEsq.decrease(100);
	
	potDir.reset();
	potDir.decrease(100);

	pinMode(pinBuzzer, OUTPUT);
	pinMode(pinButEsq, INPUT);
	pinMode(pinButDir, INPUT);
	pinMode(pinButRec, INPUT);
	
	pinMode(pinButMode, INPUT);
	digitalWrite(pinButMode, HIGH);
	
	// Pino 2 (ChangeMode)
	attachInterrupt(0, changeMode, FALLING  );
	
	digitalWrite(pinButEsq, HIGH);
	digitalWrite(pinButDir, HIGH);
	digitalWrite(pinButRec, HIGH);
	
	lcd.init();
	lcd.backlight();
	
	lcd.createChar(0, bell);
	lcd.createChar(1, grau);
	lcd.createChar(2, indicador1);
	lcd.createChar(3, indicador2);
	lcd.createChar(4, indicador3);
	lcd.createChar(5, indicador4);
	lcd.createChar(6, indicador5);
	lcd.createChar(7, indicador6);
	
	lcd.setCursor(5, 0);
	lcd.print("Radio FM");
	lcd.setCursor(7, 2);
	lcd.print("3HTEC");
	Wire.begin();
	frequency = 88.1;
	setFrequency();

	rtc.begin();
	if (! rtc.isrunning()) {
		rtc.adjust(DateTime(__DATE__, __TIME__));
	}

	// Serial.begin(9600);
	
}

void loop()
{
	if(start == 0){
		start = 1;
		lcd.clear();
	}
	if (irrecv.decode(&results)) {
		// Aumentar Volume
		if(results.value == 3777255591){
			if(volume<99){
				potEsq.increase(1);
				potDir.increase(1);
				volume++;
			}
			volumeTempo = 20;
			delay(200);
			// Diminuir Volume
			} else if(results.value == 3777273951){
			if(volume>0){
				potEsq.decrease(1);
				potDir.decrease(1);
				volume--;
			}
			volumeTempo = 20;
			delay(200);
			// MUTE
			} else if(results.value == 3777257631){
			potEsq.decrease(100);
			potDir.decrease(100);
			volume = 0;
			delay(200);
			// MEM (1)
			} else if(results.value == 3777249471){
			memPoint=0;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (2)
			} else if(results.value == 3777243351){
			memPoint=1;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (3)
			} else if(results.value == 3777259671){
			memPoint=2;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (4)
			} else if(results.value == 3777280071){
			memPoint=3;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (5)
			} else if(results.value == 3777294351){
			memPoint=4;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (6)
			} else if(results.value == 3777271911){
			memPoint=5;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (7)
			} else if(results.value == 3777263751){
			memPoint=6;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (8)
			} else if(results.value == 3777278031){
			memPoint=7;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (9)
			} else if(results.value == 3777296391){
			memPoint=8;
			frequency=mem[memPoint];
			setFrequency();
			// MEM (0)
			} else if(results.value == 3777245391){
			memPoint=9;
			frequency=mem[memPoint];
			setFrequency();
			// Mode
			} else if(results.value == 3777239271){
			changeMode();
			// Record (#)
			} else if(results.value == 3777292311){
			alarme = !alarme;
		}
		
		irrecv.resume();
	}
	
	// Leitura da Tensão da Bateria
	int reading = analogRead(3);
	tensao = ((double)reading/1024.0*5.0*3);

	// Se a tensão estiver baixa, mas não for menor que 1v soa alarme para trocar bateria
	if(tensao < 11.0 && tensao > 1){
		digitalWrite(pinBuzzer, HIGH);
		delay(300);
		digitalWrite(pinBuzzer, LOW);
		} else {
		digitalWrite(pinBuzzer, LOW);
	}

	if(!digitalRead(pinButEsq)){
		// Quando não estiver em modo MEM=3 muda de 0.1 em 0.1
		if(modo == 0){
			frequency-= 0.1;
			if(frequency < 88.0){
				frequency = 108.0;
			}
			// Quando estiver em modo MEM=3 muda entre as memórias gravadas
			} else {
			memPoint--;
			if(memPoint<0){
				memPoint=SIZE_MEM-1;
			}
			frequency=mem[memPoint];
		}
		setFrequency();
	}
	
	// Quando não estiver em modo MEM=3 muda de 0.1 em 0.1
	if(!digitalRead(pinButDir)){
		if(modo == 0){
			frequency+= 0.1;
			if(frequency > 108.0){
				frequency = 88.0;
			}
			// Quando estiver em modo MEM=3 muda entre as memórias gravadas
			} else {
			memPoint++;
			if(memPoint>SIZE_MEM-1){
				memPoint=0;
			}
			frequency=mem[memPoint];
		}
		setFrequency();
	}

	// Estando no modo=1 (HORA), clicando no REC muda para alarme de 15 em 15 min
	if(!digitalRead(pinButRec)){
		alarme = !alarme;
	}

	// Alarme = 1 (Soa de 15 em 15 min)
	if(alarme == 1){
		// Alarme de Hora em Hora
		if(minutos == 0 && segundos == 0){
			digitalWrite(pinBuzzer, HIGH);
			delay(50);
			digitalWrite(pinBuzzer, LOW);
			delay(50);
			digitalWrite(pinBuzzer, HIGH);
			delay(50);
			digitalWrite(pinBuzzer, LOW);
			delay(50);
			digitalWrite(pinBuzzer, HIGH);
			delay(50);
			digitalWrite(pinBuzzer, LOW);
		}
		// Alarme de 30 em 30 min
		if(minutos % 30 == 0 && segundos == 0){
			digitalWrite(pinBuzzer, HIGH);
			delay(50);
			digitalWrite(pinBuzzer, LOW);
			delay(50);
			digitalWrite(pinBuzzer, HIGH);
			delay(50);
			digitalWrite(pinBuzzer, LOW);
		}
		// Alarme de 15 em 15  min
		if(minutos % 15 == 0 && segundos == 0){
			digitalWrite(pinBuzzer, HIGH);
			delay(50);
			digitalWrite(pinBuzzer, LOW);
		}
	}
	
	if(volumeTempo>0){
		displayVolume();
		} else {
		displayRadio();
	}
	
	delay(50);
}

void setFrequency()
{
	frequencyB = 4 * (frequency * 1000000 + 225000) / 32768;
	frequencyH = frequencyB >> 8;
	frequencyL = frequencyB & 0XFF;
	delay(100);
	Wire.beginTransmission(0x60);
	Wire.write(frequencyH);
	Wire.write(frequencyL);
	Wire.write(0xB0);
	Wire.write(0x10);
	Wire.write((byte)0x00);
	Wire.endTransmission();
	delay(100);
}

void changeMode(){
	detachInterrupt(0);
	modo = !modo;
	attachInterrupt(0, changeMode, FALLING  );
}

void displayRadio(){
	
	// Memória
	if(modo == 1){
		if(memPoint == -1){
			memPoint = 0;
		}
		lcd.setCursor(0, 0);
		lcd.print(memDesc[memPoint]);
		String memStr = String(memDesc[memPoint]);
		int tam = memStr.length();
		for(int i=0; i<(18-tam); i++){
			lcd.print(" ");
		}
		lcd.setCursor(17,1);
		lcd.print("MEM");
		} else {
		bool achou = false;
		for(int i=0; i<SIZE_MEM; i++){
			if(is_approximately_equal(frequency,mem[i])){
				memPoint = i;
				lcd.setCursor(0, 0);
				lcd.print(memDesc[memPoint]);
				String memStr = String(memDesc[memPoint]);
				int tam = memStr.length();
				for(int i1=0; i1<(18-tam); i1++){
					lcd.print(" ");
				}
				achou = true;
				break;
			}
		}
		if(!achou){
			lcd.setCursor(0, 0);
			lcd.print("              ");
		}
		lcd.setCursor(17,1);
		lcd.print("   ");
	}
	
	lcd.setCursor(14, 0);
	if(((int)frequency)<100){
		lcd.print(" ");
	}
	
	lcd.print(frequency);
	
	if(frequency == (double)(int)frequency){
		lcd.print("0");
	}
	
	lcd.setCursor(0, 1);
	lcd.print("                 ");

	// Leitura de Data e Hora do RTC
	DateTime now = rtc.now();
	horas = now.hour();
	minutos = now.minute();
	segundos = now.second();

	// Hora
	lcd.setCursor(0, 3);
	if(horas<10){
		lcd.print("0");
	}
	lcd.print(horas, DEC);
	lcd.print(':');
	if(minutos<10){
		lcd.print("0");
	}
	lcd.print(minutos, DEC);
	lcd.print(':');
	if(segundos<10){
		lcd.print("0");
	}
	lcd.print(segundos, DEC);
	if(alarme == 1){
		lcd.write(0);
		lcd.print(" ");
		} else {
		lcd.print("  ");
	}

	// Data
	lcd.setCursor(12, 3);
	int dia = now.day();
	if(dia<10){
		lcd.print("0");
	}
	lcd.print(dia, DEC);
	lcd.print('/');
	int mes = now.month();
	if(mes<10){
		lcd.print("0");
	}
	lcd.print(mes, DEC);
	lcd.print('/');
	int ano = (now.year() % 100);
	if(ano<10){
		lcd.print("0");
	}
	lcd.print(ano, DEC);

	// Tensão
	lcd.setCursor(0, 2);
	lcd.print(tensao);
	lcd.print("V    ");
	if(tensao<10){
		lcd.print("   ");
	}

	// Temperatura
	checkTemp();
	lcd.setCursor(13, 2);
	lcd.print(temperatura);
	lcd.write(1);
	lcd.print("C");

}

float checkTemp(){
	float totalTemp = 0;
	for(int i=0; i<5; i++){
		valorLidoTemp = analogRead(pinoLM35);
		delay(20);
		temperatura = (valorLidoTemp * 0.00488);
		temperatura = temperatura * 100;
		totalTemp+= temperatura;
	}
	return totalTemp/5;
}

void displayVolume(){
	if(volumeTempo>0){
		int sobra = 17;
		lcd.setCursor(0, 1);
		if(volume<10){
			lcd.print("0");
		}
		lcd.print(volume);
		lcd.print(" ");
		if(volume > 0){
			lcd.write(2);
			sobra--;
		}
		if(volume > 3){
			lcd.write(2);
			sobra--;
		}
		if(volume > 10){
			lcd.write(3);
			sobra--;
		}
		if(volume > 15){
			lcd.write(3);
			sobra--;
		}
		if(volume > 20){
			lcd.write(4);
			sobra--;
		}
		if(volume > 25){
			lcd.write(4);
			sobra--;
		}
		if(volume > 30){
			lcd.write(5);
			sobra--;
		}
		if(volume > 35){
			lcd.write(5);
			sobra--;
		}
		if(volume > 40){
			lcd.write(5);
			sobra--;
		}
		if(volume > 45){
			lcd.write(5);
			sobra--;
		}
		if(volume > 50){
			lcd.write(6);
			sobra--;
		}
		if(volume > 55){
			lcd.write(6);
			sobra--;
		}
		if(volume > 60){
			lcd.write(6);
			sobra--;
		}
		if(volume > 65){
			lcd.write(6);
			sobra--;
		}
		if(volume > 70){
			lcd.write(7);
			sobra--;
		}
		if(volume > 75){
			lcd.write(7);
			sobra--;
		}
		if(volume > 80){
			lcd.write(7);
			sobra--;
		}
		if(volume > 90){
			lcd.write(7);
			sobra--;
		}
		for(int i=0; i<sobra; i++){
			lcd.print(" ");
		}
		volumeTempo--;
	}
}


