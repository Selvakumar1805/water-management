#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "eQMoYkugvaUIaTWNH8Q9zbE9TFPE8xeI";
char ssid[] = "Airtel_MARCELLO";
char pass[] = "MTECHTRICHY1";

LiquidCrystal_I2C lcd(0x3F,16, 2);

const int trigPin = D7;
const int echoPin = D8;

const int Red = D3; // 1st LED
const int Yellow = D5; // 2nd LED
const int Green = D6; // 3rd LED
const int relayPin = D0;

// Define the water level thresholds in cm
const int fullLevel = 3.50;
const int lowLevel = 8.18;
// Define variables for duration, distance, and water level
long duration ;
float distance ;
int waterLevel ;

void setup() {
  Serial.begin(9600);           // Initialize the serial communication and pins
  digitalWrite(relayPin, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(Red, OUTPUT); // 1st LED
  pinMode(Yellow, OUTPUT); // 2nd LED
  pinMode(Green, OUTPUT); // 3rd LED

  lcd.begin();
  lcd.backlight();
  lcd.clear();

  Blynk.begin(auth,ssid,pass);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!= WL_CONNECTED){
    Serial.print("......");
    lcd.setCursor(0,0);
    lcd.print(".................");
    delay(500);
    lcd.clear();

  }
  Serial.print("WiFi Connected");

  lcd.setCursor(0,0);
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("Water Management");
  lcd.setCursor(5,1);
  lcd.print("System");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Send a 10 microsecond pulse to the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse and calculate the distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Determine the water level based on the distance measured
  if (distance <= fullLevel) {
    waterLevel = 100;
  } else if (distance >= lowLevel) {
    waterLevel = 0;
  } else{
   waterLevel = map(distance, fullLevel, lowLevel, 0, 100);

  }
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Water Level:");
   lcd.print(waterLevel);
  

  // Control the relay based on the water level
  if ( waterLevel < 20) {
    digitalWrite(relayPin, LOW);
    Blynk.virtualWrite(V4,"MOTOR ON");
    lcd.setCursor(0,1);
    lcd.print("MOTOR ON ");
  } else if(waterLevel >19 && waterLevel <=90  ) {
    Blynk.virtualWrite(V4,"MOTOR ON");
    lcd.setCursor(0,1);
    lcd.print("MOTOR ON ");

  }else if(waterLevel > 90  ) {
    digitalWrite(relayPin, HIGH);
    Blynk.virtualWrite(V4,"MOTOR OFF");
    lcd.setCursor(0,1);
    lcd.print("MOTOR OFF");
  }

  // Control the LEDs based on the water level
  if (waterLevel < 25) {
    digitalWrite(Red, HIGH); // 1st LED ON
    digitalWrite(Yellow, LOW);  // 2nd LED OFF
    digitalWrite(Green, LOW);  // 3rd LED OFF
    Blynk.virtualWrite(V1,HIGH);
    Blynk.virtualWrite(V2,LOW);
    Blynk.virtualWrite(V3,LOW);
    
  } else if (waterLevel >= 25 && waterLevel <= 80) {
    digitalWrite(Red, LOW);   // 1st LED OFF
    digitalWrite(Yellow, HIGH);  // 2nd LED ON
    digitalWrite(Green, LOW);   // 3rd LED OFF
    Blynk.virtualWrite(V2,HIGH);
    Blynk.virtualWrite(V1,LOW);
    Blynk.virtualWrite(V3,LOW);
    
  } else if(waterLevel > 80) {

    digitalWrite(Red, LOW);   // 1st LED OFF
    digitalWrite(Yellow, LOW);   // 2nd LED OFF
    digitalWrite(Green, HIGH);  // 3rd LED ON
    Blynk.virtualWrite(V3,HIGH);
    Blynk.virtualWrite(V1,LOW); 
    Blynk.virtualWrite(V2,LOW);
  }

  // Print the distance and water level to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Water Level: ");
  Serial.print(waterLevel);
  Serial.println("%");
 
}

