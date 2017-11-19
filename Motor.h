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
    unsigned char maxVelocity = 255;			// Wert zwischen 0 und 255, maximale Geschwindigkeit

    unsigned char nextVelocityPwmLeft = 0;		// Ziel Geschwindigkeit
    unsigned char nextVelocityPwmRight = 0;	// Ziel Geschwindigkeit



public:

    Motor();
    ~Motor();


    void updateVelocity();

    void driveStraight(unsigned char velocity);
    void driveStraightLeft(unsigned char velocity);
    void driveStraightRight(unsigned char velocity);

    void turn(float velocity);
    void turnLeft();
    void turnRight();

    void stop();

    // Testen
};

#endif	/* MOTOR_H */

