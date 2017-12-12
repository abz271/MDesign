#include "Motor.h"
#include "Odometrie.h"
#include <Arduino.h>
Motor::Motor() {
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
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
}

void Motor::stoppInstantForward(unsigned char velocity){
	digitalWrite(in1, HIGH);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, HIGH);

	nextVelocityPwmLeft = velocity;
	nextVelocityPwmRight = velocity;
}

void Motor::driveStraight(){
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

	nextVelocityPwmLeft = 150;
	nextVelocityPwmRight = 150;
}

void Motor::driveStraightRegulated(unsigned char velocity, float difference){
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
// Geschwindigkeiten für die Motoren einstellen

	nextVelocityPwmLeft = velocity - char(difference);
	nextVelocityPwmRight = velocity + char(difference);
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

void Motor::driveBack(){
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	nextVelocityPwmLeft = 255;
	nextVelocityPwmRight = 255;
}



