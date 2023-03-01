#define BLYNK_TEMPLATE_ID "TMPL3BqawD3C"
#define BLYNK_DEVICE_NAME "Plant Tree"
#define BLYNK_AUTH_TOKEN "yayHY0jk4TtYB4yM_sPSP_ZxqmYUsKcc"
#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>         
#include <SimpleTimer.h>
const int DHTPIN = 13;
DHT dht(DHTPIN, DHT11);
#define pumpState V0
#define TEMP V1
#define HUM V2
#define MOISTURE V3

SimpleTimer timer;

char auth[] = BLYNK_AUTH_TOKEN; //Auth Token
char ssid[] = "Loc Vung"; //Tên Wifi
char pass[] = "88888888"; //Mật khẩu Wifi

const int moisturePin = A0;
const int motorPin = 26;
int moisturePercentage; //moisture reading
float t;
float h;

void setup() {
  // put your setup code here, to run once:
  Blynk.begin(auth, ssid, pass); 
  pinMode(motorPin,OUTPUT);
  digitalWrite(motorPin, LOW);
  dht.begin();
  timer.setInterval(500, sendDhtSensor);
  timer.setInterval(1000, plant);
}

void sendDhtSensor(){
  h = dht.readHumidity(); //Đọc độ ẩm
  t = dht.readTemperature(); //Đọc nhiệt độ
  Blynk.virtualWrite(TEMP, t); //Hiển thị nhiệt độ lên Blynk
  Blynk.virtualWrite(HUM, h); //Hiển thị độ ẩm lên Blynk
}

void plant(){
  moisturePercentage = analogRead(moisturePin);
  moisturePercentage = map(moisturePercentage, 0, 1024, 0, 100);
  moisturePercentage = (moisturePercentage - 100) * -1;
  Blynk.virtualWrite(MOISTURE,moisturePercentage);
  if (moisturePercentage <= 40) {
    digitalWrite(motorPin, HIGH);         // turn on motor
    Blynk.virtualWrite(pumpState,HIGH);
  }
  if (moisturePercentage > 40) {
    digitalWrite(motorPin, LOW);          // turn off mottor
    Blynk.virtualWrite(pumpState,LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run(); //Khởi chạy app Blynk
  timer.run(); //Khởi chạy timer
}
