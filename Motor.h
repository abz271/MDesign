/*
 * 		Ansteuerung der Motoren
 * 		Version:
 * 		Datum:
 */

/*
 * 		Erklärung:
 * 		updateVelocity muss jeden Programmzyklus aufgerufen werden, also in der laufenden while,
 * 		dem loop oder sonstigem, sonst wird die Motorgeschwindigkeit nicht mehr verändert
 *
 * 		Um gerade aus zu fahren muss einmalig driveStraight aufgerufen werden
 *
 * 		Dann wird gewartet bis man sich nur noch den Anhalteweg entfernt vom Ziel befindet.
 * 		Z.B. will man 20 cm fahren, ruft man nach 19,5 cm die Methode stop auf
 *
 * 		Stop muss auch nur ein einiges mal aufgerufen werden, ab da wird dann die Geschwindigkeit
 * 		innerhalb von einer halben Sekunde auf 0 reduziert
 *
 * 		Das gleiche beim drehen:
 * 		Einmalig: turnLeft
 * 		Warten: Bis man sich den Anhalteweg in grad vor dem Ziel befindet
 * 		(Dieser Wert muss aus dem testen ermittelt werden)
 * 		Einmalig: stop
 *
 * 		Einstellparameter:
 * 		-startUpTime
 * 		-stopTime
 * 		-stepNumber
 */

#include "Odometrie.h"

#ifndef MOTOR_H
#define	MOTOR_H

class Motor {
private:
	Odometrie Odo;


	// Pins der H Brücke
	// Left Wheel
	unsigned char pwmA = 10;	// Pwm Signal, steuert Motorgeschwindigkeit
	unsigned char in1 = 22;		// Drehrichtung HIGH	LOW		LOW
	unsigned char in2 = 23;		// Drehrichtung	LOW		HIGH	LOW
	// Right Wheel
	unsigned char in3 = 24;		// Drehrichtung
	unsigned char in4 = 25;		// Drehrichtung
	unsigned char pwmB = 11;	// Pwm Signal, steuert Motorgeschwindigkeit

	// Geschwindigkeiten der Räder
	unsigned int driveOffset = 10;				// Offset zur Korrektur bei Zickzackfahrt
    unsigned char maxVelocityPwm = 150;			// Wert zwischen 0 und 255, maximale Geschwindigkeit
    unsigned char nextVelocityPwmLeft = 0;		// Ziel Geschwindigkeit
    float currentVelocityPwmLeft = 0;			// Aktuelle Geschwindigkeit
    unsigned char nextVelocityPwmRight = 0;		// Ziel Geschwindigkeit
	float currentVelocityPwmRight = 0;			// Aktuelle Geschwindigkeit

	// Anfahr- und Anhaltekurvenparameter
	float startUpTime = 750;									// Zeit bis die maximale Geschwindigkeit erreicht wird
	float stopTime = 250;										// Zeit bis das Fahrzeug die Geschwindigkeit 0 hat
	int stepNumber = 20;										// Anzahl der Schritte zum Beschleunigen/ Abbremsen
	float startStepTime = (float)startUpTime / stepNumber;		// Zeit eines Startschrittes (Stufenzeit)
	float startStepPwm = (float)maxVelocityPwm /stepNumber;		// Wert eines Starschrites (Stufenwert)
	float stopStepTime = (float)stopTime / stepNumber;			// Zeit eines Stopschrittes
	float stoptStepPwm = (float)maxVelocityPwm /stepNumber;		// Wert eines Stopschrittes

public:

    Motor();
    ~Motor();


    void updateVelocity();

    bool isStoped();

    void driveStraight();
    void driveStraight(unsigned char nextVelocityPwm);
    void driveStraightLeft(unsigned char nextVelocityPwm);
    void driveStraightRight(unsigned char nextVelocityPwm);

    void turn(float speed);
    void turnLeft();
    void turnLeft(unsigned char nextVelocityPwm);
    void turnRight();
    void turnRight(unsigned char nextVelocityPwm);

    //void brake();
    void stop();
    void stopInstant();

    // Testen
    void testAnfahren();
    void exampleDriveStraight();


};

#endif	/* MOTOR_H */

