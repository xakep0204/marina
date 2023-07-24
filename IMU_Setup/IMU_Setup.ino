#include <basicMPU6050.h>
#include "quaternion.h"

// Constants

#define RAD_TO_DEG 180.00

// IMU object setup

basicMPU6050<> imu;


float bAx;



void setup() {
  // setting up registers
  imu.setup();

  imu.setBias();
  
  // Serial Monitor

  Serial.begin(38400);

  Quaternion q = Quaternion();

}

void loop() {

  imu.updateBias();

  Serial.print(">AX: ");
  Serial.println(imu.ax()-1);

  Serial.print(">AY: ");
  Serial.println(imu.ay());

  Serial.print(">AZ: ");
  Serial.println(imu.az());

  Serial.print(">GX: ");
  Serial.println(imu.gx());

  Serial.print(">GY: ");
  Serial.println(imu.gy());

  Serial.print(">GZ: ");
  Serial.println(imu.gz());

}
