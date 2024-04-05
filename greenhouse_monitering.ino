#include <Wire.h> 
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <DHT.h>            // Including library for dht

// WiFi credentials
#define WLAN_SSID "iPhone (4)"
#define WLAN_PASS "Henaris8" 

WiFiClient client; 
unsigned long myChannelNumber =2498328;  //thinkspeak channel number 
const char * myWriteAPIKey ="SRQDE2G0GO6WHI7C";  // thinkspeak api key

// Initialize the I2C LCD.
// The constructor arguments are the I2C address (usually 0x27 or 0x3F), columns, and rows of the LCD
#define RS D0
#define E  D8
#define DB4 D5
#define DB5 D4
#define DB6 D3
#define DB7 D2
#define DHTPIN D7    
#define DHT_TYPE DHT11
#define soilMoisturePin A0


DHT dht(DHTPIN, DHT11);
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);  // RS, E, DB4, DB5, DB6, DB7

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  ThingSpeak.begin(client); // Initialize ThingSpeak

}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2); 
  dht.begin();
  connectToWiFi();
}

void loop() {
  delay(15000); 

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(soilMoisturePin);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("soilMoisture: ");
  Serial.print(soilMoisture);
  Serial.println(" %");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  //lcd.print("C");
  lcd.print(" H:");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("soilMoist:");
  lcd.print(soilMoisture);
  lcd.print("%");

  if (isnan(soilMoisture) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, soilMoisture);
  ThingSpeak.setField(3, humidity);
  // ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200) {
    Serial.println("Update successful.");
  } else {
    Serial.println("Update failed.");
}

}