#pragma once
#include "Quaternion.h"

void updateQuaternionGyro(Quaternion q, double gyr[3], double dt);

void updateQuaternion(Quaternion q, double gyr[3], double acc[3], double dt, double alpha);

