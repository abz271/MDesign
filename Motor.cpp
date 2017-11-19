#include "Motor.h"
#include "Odometrie.h"
#include <Arduino.h>
int a = 0;
Motor::Motor() {
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
}

Motor::~Motor() {
}

void Motor::updateVelocity(){
	analogWrite(pwmA, nextVelocityPwmLeft);
	analogWrite(pwmB, nextVelocityPwmRight);
}

void Motor::stop() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = 0;
	nextVelocityPwmRight = 0;

	updateVelocity();

}

void Motor::driveStraight(unsigned char velocity){
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
// Geschwindigkeiten für die Motoren einstellen

	nextVelocityPwmLeft = velocity;
	nextVelocityPwmRight = velocity;
	updateVelocity();

}

// bei Vorwärtsfahrt leichte Korrektur bei Abweichung vom Weg
void Motor::driveStraightLeft(unsigned char velocity){
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

	// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = velocity - driveOffset;
	nextVelocityPwmRight = velocity;

	updateVelocity();

}
// bei Vorwärtsfahrt leichte Korrektur bei Abweichung vom Weg
void Motor::driveStraightRight(unsigned char velocity){
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

	// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = velocity;
	nextVelocityPwmRight = velocity - driveOffset;

	updateVelocity();
}

void Motor::turn(float velocity) {
	if(velocity <= 0) {
		digitalWrite(in1, LOW);
		digitalWrite(in2, HIGH);
		digitalWrite(in3, HIGH);
		digitalWrite(in4, LOW);
	} else {
		digitalWrite(in1, HIGH);
		digitalWrite(in2, LOW);
		digitalWrite(in3, LOW);
		digitalWrite(in4, HIGH);
	}

	nextVelocityPwmLeft = abs(velocity);
	nextVelocityPwmRight = abs(velocity);

}


// TODO: Löschen?
void Motor::turnLeft() {

	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = maxVelocity;
	nextVelocityPwmRight = maxVelocity;

	updateVelocity();

}

// TODO: Löschen?
void Motor::turnRight() {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = maxVelocity;
	nextVelocityPwmRight = maxVelocity;

	updateVelocity();
}



