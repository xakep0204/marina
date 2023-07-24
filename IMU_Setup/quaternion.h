
#ifndef QUATERNION_H

#define QUATERNION_H

#include <Arduino.h>

class Quaternion {
    public:
    
    double p[4];

    Quaternion(): p{1,0,0,0} {}

    Quaternion(double s, double x, double y, double z): p{s,x,y,z} {}

    Quaternion clone(){
        return Quaternion(this->p[0], this->p[1], this->p[2], this->p[3]);
    }

    double norm(){
        return sqrt(sq(this->p[0]) + sq(this->p[1]) + sq(this->p[2]) + sq(this->p[3]));
    }

    Quaternion renormalize(){
        double n = this->norm();
        this->p[0] /= n;
        this->p[1] /= n;
        this->p[2] /= n;
        this->p[3] /= n;

        return *this;
    }

    Quaternion conjugate(){
        this->p[1] *= -1;
        this->p[2] *= -1;
        this->p[3] *= -1;
        this->p[0] *= 1;

        return *this;
    }

    Quaternion invert(){
        this->conjugate();
        this->renormalize();

        return *this;
    }

    Quaternion multiply(Quaternion a, Quaternion b){
        Quaternion c;

        c.p[0] = a.p[0]*b.p[0] - a.p[1]*b.p[1] - a.p[2]*b.p[2] - a.p[3]*b.p[3];
        c.p[1] = a.p[0]*b.p[1] + a.p[1]*b.p[0] + a.p[2]*b.p[3] - a.p[3]*b.p[2];
        c.p[2] = a.p[0]*b.p[2] - a.p[1]*b.p[3] + a.p[2]*b.p[0] + a.p[3]*b.p[1];
        c.p[3] = a.p[0]*b.p[3] + a.p[1]*b.p[2] - a.p[2]*b.p[1] + a.p[3]*b.p[0];
    }

    Quaternion rotate(Quaternion r){
        Quaternion inv_r = r.clone().invert();
        Quaternion rp = Quaternion().multiply(r,*this);
        return Quaternion().multiply(rp,inv_r);
    }
    
    void SerialMeasure(){
        Serial.print(">S: ");
        Serial.println(this->p[0]);
        Serial.print(">X: ");
        Serial.println(this->p[1]);
        Serial.print(">Y: ");
        Serial.println(this->p[2]);
        Serial.print(">Z: ");
        Serial.println(this->p[3]);
    }

};

#endif // QUATERNION_H