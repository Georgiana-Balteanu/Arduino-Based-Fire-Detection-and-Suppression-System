#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

const int buzzer = 2;
const int led    = 7;
const int gaspin = A0;

const int PRAG_GAZ = 250;
const float PRAG_TEMP = 35.0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(led, LOW);
  

  Serial.begin(9600);

  lcd.begin();// Inițializarea display-ului LCD
lcd.backlight();// Activarea iluminării de fundal a display-ului


  sensors.begin();

  lcd.setCursor(0,0);
  lcd.print("Sistem pornit");
  delay(1200);
  lcd.clear();
}

void alarmaGaz(int valoareGaz) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DETECTARE GAZ!");
  lcd.setCursor(0,1);
  lcd.print("Gaz=");
  lcd.print(valoareGaz);

  digitalWrite(led, HIGH);

  // beep scurt repetat (mai “vizibil”)
  tone(buzzer, 2000);
  delay(300);
  noTone(buzzer);
  delay(200);
}

void alarmaTemperatura() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEMPERATURA");
  lcd.setCursor(0,1);
  lcd.print("CRESCUTA!");

  digitalWrite(led, HIGH);
  tone(buzzer, 1500);
  delay(800);
  noTone(buzzer);
  delay(200);
}

void loop() {
  int valoareGaz = analogRead(gaspin);

  // DEBUG: vezi sigur daca trece de prag
  Serial.print("Gaz: ");
  Serial.println(valoareGaz);

  // PRIORITATE GAZ: cat timp e peste prag, ramai in alarma
  if (valoareGaz > PRAG_GAZ) {
    alarmaGaz(valoareGaz);
    return;                     // <<< asta e cheia
  }

  // daca nu e gaz, mergi pe temperatura
  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);

  Serial.print("Temp: ");
  Serial.println(temperatura);

  if (temperatura >= PRAG_TEMP) {
    alarmaTemperatura();
    return;
  }

  // afisare normala
  digitalWrite(led, LOW);
  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" C");

  lcd.setCursor(0,1);
  lcd.print("Gaz: ");
  lcd.print(valoareGaz);

  delay(500);
}