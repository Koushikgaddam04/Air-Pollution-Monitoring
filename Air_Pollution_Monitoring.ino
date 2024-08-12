#define BLYNK_TEMPLATE_ID "<Template - ID>"
#define BLYNK_TEMPLATE_NAME "<Template - Name>"
#define BLYNK_AUTH_TOKEN "<Token>"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <MQ135.h>
// #include <SoftwareSerial.h>

#define SENSOR_PIN 34 // GPIO36 (VP)

char ssid[] = "<Wifi-Username>";
char pass[] = "<Wifi-password>";

BlynkTimer timer;

void myTimerEvent() {
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);

  Serial.println("Calibrating MQ135...");
  for (int i = 0; i < 10; i++) {
    int sensorValue = analogRead(SENSOR_PIN);
    Serial.print("Initial reading ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorValue);
    delay(1000);
  }
  Serial.println("Calibration done.");
}

void loop() {
  Blynk.run();
  timer.run();

  int sensorValue = analogRead(SENSOR_PIN);

  int NO_PPM = analogRead(SENSOR_PIN);
  int SO2_PPM = analogRead(SENSOR_PIN);
  int CO_PPM = analogRead(SENSOR_PIN);
  int NH3_PPM = analogRead(SENSOR_PIN);
  float CO2_PPM = analogRead(SENSOR_PIN);

  NO_PPM = map(NO_PPM, 0, 1023, 0, 200);
  SO2_PPM = map(SO2_PPM, 0, 1023, 0, 500);
  NH3_PPM = map(NH3_PPM, 0, 1023, 0, 100);
  CO2_PPM = map(CO2_PPM, 0, 1023, 0, 5000);
  CO_PPM = map(CO_PPM, 0, 1023, 0, 50);

  Blynk.virtualWrite(V0, NO_PPM);
  Blynk.virtualWrite(V1, SO2_PPM);
  Blynk.virtualWrite(V2, CO_PPM);
  Blynk.virtualWrite(V3, NH3_PPM);
  Blynk.virtualWrite(V4, CO2_PPM);

  Serial.print("NO: ");
  Serial.print(NO_PPM);
  Serial.print(" PPM, SO2: ");
  Serial.print(SO2_PPM);
  Serial.print(" PPM, CO: ");
  Serial.print(CO_PPM);
  Serial.print(" PPM, NH3: ");
  Serial.print(NH3_PPM);
  Serial.print(" PPM, CO2: ");
  Serial.print(CO2_PPM);
  Serial.println(" PPM");

  // Check if any gas concentration exceeds its threshold. If it exceeds send the data to user through email.
  if (NO_PPM > 150 || SO2_PPM > 400 || CO_PPM > 40 || NH3_PPM > 80 || CO2_PPM > 4500) {
    Blynk.logEvent("gas_detected", "Gas Leakage Detected");
  }

  delay(1000);
}
