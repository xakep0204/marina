#include "quaternion_math.h"

// updating quaternion value using gyro values

void updateQuaternionGyro(Quaternion q, double gyr[3], double dt){
    // create infinitesmal rotation quaternion

    // normalise gyro values
    double gyro_norm = sqrt(sq(gyr[0]) + sq(gyr[1]) + sq(gyr[2]));


    Quaternion q_d = q.clone();

    if (gyro_norm >= 1e-8){
        q_d = Quaternion(gyro_norm*dt,sin(gyro_norm*dt/2)*(gyr[0]/gyro_norm),sin(gyro_norm*dt/2)*(gyr[1]/gyro_norm),sin(gyro_norm*dt/2)*(gyr[2]/gyro_norm));
        
    }

    q  = q.multiply(q, q_d);
}


// Complementary filter for accelerometer and gyroscope

void updateQuaternion(Quaternion q, double gyr[3], double acc[3], double dt, double alpha){

    // gyro to rotation quaternion
    Quaternion q_d = q.clone();
    updateQuaternionGyro(q_d, gyr, dt);

    // accelerometer to rotation quaternion
    Quaternion q_acc = Quaternion(0,acc[0],acc[1],acc[2]).renormalize();
    q_acc = q_acc.rotate(q_d).renormalize();

    double phi = 180/PI*acos(q_acc.p[2]);

    double norm_m = sqrt(sq(q_acc.p[1])+sq(q_acc.p[3]));

    Quaternion q_tilt;
    
    if (norm_m >= 1e-8){
        q_tilt = Quaternion().setFromAngleAxis((1-alpha)*phi, -q_acc.p[3]/norm_m, 0.0, q_acc.p[1]/norm_m).renormalize();

    }

    q = q.multiply(q_tilt,q_d).renormalize();
}