#include <Arduino.h>
#include "Kommunikation.h"

#ifndef ODOMETRIE_H
#define ODOMETRIE_H

class Odometrie {
private:

	// Geometrie des Fahrzeugs
    float Achsabstand = 277.3;	// in mm

	// Position
    float x_odometrie;
    float y_odometrie;
    float alpha_odometrie;


    // Rad Geometrie
    const float radDurchmesserLinks = 48.83 * (1000.0/1009.0);
    const float radUmfangLinks = radDurchmesserLinks * PI;

    const float radDurchmesserRechts = 49.06 * (1000.0/1009.0);
    const float radUmfangRechts = radDurchmesserRechts * PI;

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






