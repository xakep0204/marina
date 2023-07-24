#include <basicMPU6050.h>



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



}

void loop() {

  imu.updateBias();

  float Ax = imu.ax();

  Serial.print( Ax );
  Serial.print( " " );
  Serial.print( imu.ay() );
  Serial.print( " " );
  Serial.print( imu.az() );
  Serial.print( "    " );

}
