#include "Arduino.h"
#include "Vec.h"

Vec::Vec() {
    x = 0;
    y = 0;

}
Vec::Vec(float angle){
	x = cos ((angle+90)*M_PI/180);
	y = sin ((angle+90)*M_PI/180);
}
Vec::Vec(float x, float y) {
    this->x = x;
    this->y = y;
}

Vec Vec::operator+(Vec v) {
    Vec r(x + v.x, y + v.y);
    return r;
}

float Vec::getLength() {
    return sqrt(x*x + y*y);
}

float Vec::getAngle() {
    return (atan2(y, x) *180 / M_PI);
}

float Vec::getX(){
	return x;
}

float Vec::getY(){
	return y;
}

void Vec::setX(float x) {
    this->x = x;
}

void Vec::setY(float y) {
    this->y = y;
}
