/*
 * 		Ansteuerung der Motoren
 * 		Version:
 * 		Datum:
 */

#include "Odometrie.h"

#ifndef MOTOR_H
#define	MOTOR_H

class Motor {
private:
	Odometrie Odo;
	// Neue Version

	// Pins der H Brücke
	// Left Wheel
	unsigned char pwmA = 10;
	unsigned char in1 = 8;
	unsigned char in2 = 9;
	// Right Wheel
	unsigned char in3 = 12;
	unsigned char in4 = 13;
	unsigned char pwmB = 11;

	// Geschwindigkeiten der Räder
    unsigned char driveOffset = 1;				// Offset-Signal zum Gegensteuern bei (ZickZack-Kurs)
    unsigned char maxVelocityPwm = 150;			// Wert zwischen 0 und 255, maximale Geschwindigkeit
    unsigned char nextVelocityPwmLeft = 0;		//
    float currentVelocityPwmLeft = 0;			//
    unsigned nextVelocityPwmRight = 0;			//
	float currentVelocityPwmRight = 0;			//

	// Anfahr- und Anhaltekurvenparameter
	float startUpTime = 1000;
	float stopTime = 500;
	int stepNumber = 20;
	float startStepTime = startUpTime / stepNumber;
	float startStepPwm = (float)maxVelocityPwm /stepNumber;
	float stopStepTime = stopTime / stepNumber;
	float stoptStepPwm = (float)maxVelocityPwm /stepNumber;

public:

    Motor();
    ~Motor(); // leerer Destruktor


    void updateVelocity();

    bool isStoped();

    void driveStraight();
    void driveStraight(unsigned char nextVelocityPwm);
    void driveStraightLeft(unsigned char nextVelocityPwm);
    void driveStraightRight(unsigned char nextVelocityPwm);
    void rotateRight90();
    void rotateLeft90();
    void turnLeft();
    void turnLeft(unsigned char nextVelocityPwm);
    void turnRight();
    void turnRight(unsigned char nextVelocityPwm);

    //void brake();
    void stop();
    void stopInstant();

    // Testen
    void testAnfahren();


};

#endif	/* MOTOR_H */

