#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Stepper.h>

#define LIS3DH_CLK 18
#define LIS3DH_MISO 19
#define LIS3DH_MOSI 23
#define LIS3DH_CS 5

Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);




const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 21, 2, 4, 22);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  SPI.begin();
  pinMode(LIS3DH_CLK, OUTPUT);
  pinMode(LIS3DH_MISO, INPUT);
  pinMode(LIS3DH_MOSI, OUTPUT);
  pinMode(LIS3DH_CS, OUTPUT);

  pinMode(21, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(22, OUTPUT);

  // Serial.println("LIS3DH test!");

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    while (1) yield();
  }

  myStepper.setSpeed(rolePerMinute);
  
}

void loop() {
  sensors_event_t event;
  lis.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  Serial.println(" m/s^2 ");

  Serial.println("");

  if (event.acceleration.y > 1.0) {
    myStepper.step(128);
  } else if (event.acceleration.y < -1.0) {
    myStepper.step(-128);
  }

  // myStepper.step(stepsPerRevolution);

  // delay(200);

  // myStepper.step(-stepsPerRevolution);

  delay(10);

}

// Have a cube that changes what it displays based on the orientation
// Accelerometer to determine ^
// Room temperature along with weather outside
// time/date
// alarm?
// cute face/anims

// status APA106 leds
// maybe use solar power? 