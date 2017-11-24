
#ifndef VEC_H
#define	VEC_H

class Vec {

public:
    float x;
    float y;
    Vec();
    Vec(float x, float y);
    Vec operator+(Vec v);
    float getLength();
    float getAngle();
    void setX(float x);
    void setY(float y);

};

#endif	/* VEC_H */
