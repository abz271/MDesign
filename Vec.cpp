#include <math.h>
#include "Vec.h"

Vec::Vec() {
    x = 0.0;
    y = 0.0;

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
    return (atan2(y, x) *180 / M_PI)+90;
}

void Vec::setX(float x) {
    this->x = x;
}

void Vec::setY(float y) {
    this->y = y;
}
