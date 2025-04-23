#include <Wire.h>
#include <MAX30105.h>           // Library for MAX30102
#include "heartRate.h"          // Heart rate algorithm (comes with MAX3010x library)
#include <LiquidCrystal_I2C.h>  // LCD library
#include <Servo.h>              // Servo motor library

#define BUZZER_PIN 8
#define SERVO_PIN 9
#define OXYGEN_THRESHOLD 90     // Minimum safe oxygen level

MAX30105 particleSensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo bedServo;

float irValue;
int oxygen = 99;  // Initial oxygen level

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  bedServo.attach(SERVO_PIN);
  bedServo.write(0);  // Bed starts in flat position

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 not found");
    lcd.print("Sensor Error!");
    while (1);  // Stop program if sensor not found
  }

  particleSensor.setup();  // Default sensor settings
  particleSensor.setPulseAmplitudeRed(0x0A);  // Turn on Red LED
  particleSensor.setPulseAmplitudeIR(0x0A);   // Turn on IR LED
}

void loop() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue)) {
    float bpm = particleSensor.getIR();  // Simulated heart rate
    oxygen = random(88, 99);  // Simulated oxygen value (use real calculation in real deployment)

    Serial.print("SpO2: ");
    Serial.print(oxygen);
    Serial.println("%");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SpO2: ");
  lcd.print(oxygen);
  lcd.print("%");

  if (oxygen < OXYGEN_THRESHOLD) {
    lcd.setCursor(0, 1);
    lcd.print("LOW OXYGEN!");
    bedServo.write(110);             // Raise bed
    digitalWrite(BUZZER_PIN, HIGH);  // Activate buzzer
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Oxygen Normal");
    bedServo.write(0);               // Return bed to flat
    digitalWrite(BUZZER_PIN, LOW);   // Turn off buzzer
  }

  delay(1000);  // Update every second
}
