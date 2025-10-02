#include <basicMPU6050.h> 

// === Create IMU instance ===
basicMPU6050<> imu;

// === Kalman Filter Class ===
class KalmanFilter {
private:
  float Q_angle;   // Process noise variance for the accelerometer
  float Q_bias;    // Process noise variance for the gyro bias
  float R_measure; // Measurement noise variance
  
  float angle; // The angle calculated by the Kalman filter
  float bias;  // The gyro bias calculated by the Kalman filter
  float rate;  // Unbiased rate
  
  float P[2][2]; // Error covariance matrix

public:
  KalmanFilter() {
    Q_angle = 0.001f;
    Q_bias = 0.003f;
    R_measure = 0.03f;
    angle = 0.0f;
    bias = 0.0f;
    P[0][0] = 0.0f; P[0][1] = 0.0f;
    P[1][0] = 0.0f; P[1][1] = 0.0f;
  }
  
  float getAngle(float newAngle, float newRate, float dt) {
    // Predict
    rate = newRate - bias;
    angle += dt * rate;
    
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;
    
    // Update
    float S = P[0][0] + R_measure;
    float K[2];
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;
    float y = newAngle - angle;
    angle += K[0] * y;
    bias += K[1] * y;
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];
    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;
    return angle;
  }
  
  void setAngle(float angle) { this->angle = angle; }
  float getRate() { return this->rate; }
};

// === Kalman filters for each axis ===
KalmanFilter kalmanRoll;
KalmanFilter kalmanPitch;
KalmanFilter kalmanYaw;

// === Variables for angle calculations ===
float accAngleRoll, accAnglePitch;
float kalmanAngleRoll, kalmanAnglePitch, kalmanAngleYaw;
float gyroRateRoll, gyroRatePitch, gyroRateYaw;

// Timing
uint32_t timer;
float dt;

// === Compute roll & pitch from accelerometer ===
void calculateAccAngles() {
  accAngleRoll = atan2(imu.ay(), sqrt(imu.ax() * imu.ax() + imu.az() * imu.az())) * 180.0f / PI;
  accAnglePitch = atan2(-imu.ax(), sqrt(imu.ay() * imu.ay() + imu.az() * imu.az())) * 180.0f / PI;
}

void setup() {
  imu.setup();
  imu.setBias();

  Serial.begin(38400);
  delay(100);

  calculateAccAngles();
  kalmanRoll.setAngle(accAngleRoll);
  kalmanPitch.setAngle(accAnglePitch);
  kalmanYaw.setAngle(0.0f); // no absolute reference for yaw, start at 0

  timer = micros();
}

void loop() {
  dt = (float)(micros() - timer) / 1000000.0f;
  timer = micros();

  imu.updateBias();

  // Gyroscope rates (deg/s)
  gyroRateRoll = imu.gx();
  gyroRatePitch = imu.gy();
  gyroRateYaw = imu.gz();

  calculateAccAngles();

  // Apply Kalman filters
  kalmanAngleRoll = kalmanRoll.getAngle(accAngleRoll, gyroRateRoll, dt);
  kalmanAnglePitch = kalmanPitch.getAngle(accAnglePitch, gyroRatePitch, dt);

  // Yaw: integrate only
  kalmanAngleYaw = kalmanYaw.getAngle(kalmanAngleYaw, gyroRateYaw, dt);

  // Output format: Roll, Pitch, Yaw, Raw Accel X Y Z, Raw Gyro X Y Z, Temp
  Serial.print("Roll: ");
  Serial.print(kalmanAngleRoll);
  Serial.print(" Pitch: ");
  Serial.print(kalmanAnglePitch);
  Serial.print(" Yaw: ");
  Serial.print(kalmanAngleYaw);
  Serial.print(" | Raw: ");
  Serial.print(imu.ax()); Serial.print(" ");
  Serial.print(imu.ay()); Serial.print(" ");
  Serial.print(imu.az()); Serial.print(" ");
  Serial.print(imu.gx()); Serial.print(" ");
  Serial.print(imu.gy()); Serial.print(" ");
  Serial.print(imu.gz());
  Serial.print(" Temp: ");
  Serial.print(imu.temp());
  Serial.println();

  delay(10);
}
