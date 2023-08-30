#include <basicMPU6050.h>
#include <MadgwickAHRS.h>


// Constants

#define RAD_TO_DEG 180.00

// IMU object setup

basicMPU6050<> imu;

// Madgwick filter object setup

Madgwick filter;



double acc[3] = {0,0,0};
double gy[3] = {0,0,0};

void setup() {
  // setting up registers
  imu.setup();

  imu.setBias();

  filter.begin(1);
  
  // Serial Monitor

  Serial.begin(38400);

 

}

void loop() {

  imu.updateBias();

  gy[0] = (double) imu.gx();
  gy[1] = (double) imu.gy();
  gy[2] = (double) imu.gz();

  acc[0] = (double) imu.ax();
  acc[1] = (double) imu.ay();
  acc[2] = (double) imu.az();




  
  
  


}
