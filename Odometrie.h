#include <Arduino.h>
#include "Kommunikation.h"

#ifndef ODOMETRIE_H
#define ODOMETRIE_H

class Odometrie {
private:

	// Geometrie des Fahrzeugs
    float Achsabstand = 275.2;	// in mm

	// Position
    float x_odometrie;
    float y_odometrie;
    float alpha_odometrie = 90;


    // Rad Geometrie
    const float radDurchmesserLinks = 49.35;
    const float radUmfangLinks = radDurchmesserLinks * PI;

    const float radDurchmesserRechts = 49.25;
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
    void setXposition (int x_odometrie);
    void setYposition (int y_odometrie);

    // Test Methoden
    void testOdometrie();
    void testRotary();
    void testBerechnung(float diffLeft[], float diffRight[], int arraySize);



};

#endif /* ODOMETRIE_H */






