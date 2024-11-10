#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Stepper.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// LIS3DH (SPI)
#define LIS3DH_CLK 18
#define LIS3DH_MISO 19
#define LIS3DH_MOSI 23
#define LIS3DH_CS 5

Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

// DS18B20 
#define ONE_WIRE_BUS 15

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Stepper motor 
const int stepsPerRevolution = 2048;  // Steps per full revolution for 28BYJ-48
const int rolePerMinute = 15;         // Stepper motor speed (0 to 17 rpm)
Stepper myStepper(stepsPerRevolution, 21, 2, 4, 22); 

void setup() {
  // Serial monitor setup
  Serial.begin(9600);
  while (!Serial) delay(10); 
  

  SPI.begin();
  sensors.begin();
  myStepper.setSpeed(rolePerMinute);

  // LIS3DH and stepper pins
  pinMode(LIS3DH_CLK, OUTPUT);
  pinMode(LIS3DH_MISO, INPUT);
  pinMode(LIS3DH_MOSI, OUTPUT);
  pinMode(LIS3DH_CS, OUTPUT);

  pinMode(21, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(22, OUTPUT);

  pinMode(ONE_WIRE_BUS, INPUT);

  // Check LIS3DH connection
  if (!lis.begin(0x18)) {
    while (1) yield();  
  }
}

void loop() {
  // Read accelerometer data
  sensors_event_t event;
  lis.getEvent(&event);
  
  Serial.print("X: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  Serial.println(" m/s^2");

  if (event.acceleration.y > 1.0) {
    myStepper.step(-128);
  } else if (event.acceleration.y < -1.0) {
    myStepper.step(128);
  }

  // Readtemperature
  sensors.requestTemperatures();  

  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  }

  delay(1000);  
}

// WIP:
// - Display based on orientation
// - Room and outside temperature
// - Time/date display
// - Alarm feature
// - Cute face animations
// - Status display using APA106 LEDs
// - Possible solar power use
