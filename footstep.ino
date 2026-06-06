#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int sensorPin = A0;
int ledPin = 7;

float voltage = 0.0;
float power = 0.0;
const float R = 1000.0;   // assume 1kΩ load resistor
int baseline = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("smart steps");
  delay(2000);
  lcd.clear();

  // 🔧 Baseline calibration
  long sum = 0;
  for(int i=0; i<50; i++) {
    sum += analogRead(sensorPin);
    delay(10);
  }
  baseline = sum / 50;   // average idle value
}

void loop() {
  int rawValue = analogRead(sensorPin);

  // Adjust with baseline
  int adjusted = rawValue - baseline;
  if(adjusted < 0) adjusted = 0;

  // Convert to voltage (assuming 5V reference)
  voltage = (adjusted / 1023.0) * 5.0;

  // Calculate power (P = V^2 / R)
  power = (voltage * voltage) / R;

  // Display values
  lcd.setCursor(0,0);
  lcd.print("V: ");
  lcd.print(voltage,2);   // 2 decimal places
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("P: ");
  lcd.print(power,4);     // 4 decimal places
  lcd.print("W   ");

  // Vibration detection: if voltage > 0.5V
  if(voltage > 0.2) {
    digitalWrite(ledPin, HIGH);   // LED strip ON
  } else {
    digitalWrite(ledPin, LOW);    // LED strip OFF
  }

  delay(200);
}