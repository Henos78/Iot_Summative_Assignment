#include <LiquidCrystal.h>
LiquidCrystal lcd = LiquidCrystal(8, 7, 13, 12, 11, 10); // Create an LCD object. Parameters: (RS, E, D4, D5, D6, D7):
const int FRONT_TRIG_PIN = 4;
const int FRONT_ECHO_PIN = 9;
const int REAR_TRIG_PIN = 2;
const int REAR_ECHO_PIN = 3;
const int BUZZER_PIN = 1; 
const int RED_LED_PIN = 6;
const int GREEN_LED_PIN=5;



void setup() {
  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  pinMode(REAR_TRIG_PIN, OUTPUT);
  pinMode(REAR_ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Home Security");
  lcd.setCursor(0, 1);
  lcd.print("System");
  delay(2000);
  lcd.clear();
}

void loop() {
  int frontDistance = getDistance(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
  int rearDistance = getDistance(REAR_TRIG_PIN, REAR_ECHO_PIN);
  
  if (frontDistance <= 200) {
    displaySuspectDistance("Front", frontDistance);
    alert();
  } else if (rearDistance <= 200) {
    displaySuspectDistance("Rear", rearDistance);
    alert();
  } else {
    displaySuspectDistance("Safe", 0); // Displays "Safe" when no suspects are detected
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) / 58;
}

void displaySuspectDistance(String location, int distance) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(location + ": ");
  if (distance > 0) {
    lcd.print(distance);
    lcd.print(" cm");
  } else {
    lcd.print("Safe");
  }
}

void alert() {
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
  noTone(BUZZER_PIN);
  delay(1000);
}
