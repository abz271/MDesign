#include <Arduino.h>
#include "Kommunikation.h"


#ifndef ODOMETRIE_H
#define ODOMETRIE_H

class Odometrie {
private:

	// Geometrie des Fahrzeugs
    float Achsabstand = 276.5;	// in mm

	// Position
    float x_odometrie;
    float y_odometrie;
    float alpha_odometrie;


    // Rad Geometrie
    const float radDurchmesser = 49.4 * (1000.0/1009.0);
    const float radUmfang = radDurchmesser * PI;

    // Encoder sind global, in der cpp definiert definiert
    // Rotary Encoder Methoden
	void getDiffWheelDistance(float& leftWheelChange, float& rightWheelChange);
	void getDiffWheelAngle(float& leftWheelChange, float& rightWheelChange);

public:

    // Nach auﬂen gebrauchte Methoden
    Odometrie();
    void updateOdometrie();
    void setPosition(int x, int y);
    int getX_position();
    int getY_position();
    float getAngle();


    // Test Methoden
    void testOdometrie();
    void testRotary();
    void testBerechnung(float diffLeft[], float diffRight[], int arraySize);



};

#endif /* ODOMETRIE_H */






