#include <Arduino.h>


#ifndef ODOMETRIE_H
#define ODOMETRIE_H

class Odometrie {
private:

	// Geometrie des Fahrzeugs
    //const int breite = 10; // Breite des Autos in "mm"
    //const int hoehe = 10; // Hoehe des Autos in "mm"
    float Achsabstand = 200;	// in mm


	// Position
    float x_odometrie = 0;
    float y_odometrie = 0;
    float alpha_odometrie = 0;

    //float odometrie_offset = 0.5;

    // Rad Geometrie
    const float radDurchmesser = 50.0;
    const float radUmfang = radDurchmesser * PI;

    // Encoder sind global, in der cpp definiert definiert
    // Rotary Encoder Methoden
	void getDiffWheelDistance(float& leftWheelChange, float& rightWheelChange);
	void getDiffWheelAngle(float& leftWheelChange, float& rightWheelChange);

public:

    // Nach auﬂen gebracuhte Methoden
    Odometrie();
    void updateOdometrie();
    float getX_position();
    float getY_position();
    float getAngle();

    // Test Methoden
    void testRotary();
    void testBerechnung(float diffLeft[], float diffRight[], int arraySize);



};

#endif /* ODOMETRIE_H */





