#include "Motor.h"
#include "Odometrie.h"
#include <Arduino.h>

Motor::Motor() {
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
}

Motor::~Motor() {
}
/*
void Motor::updateVelocity() {

	// Zeitliches Verhalten, nur alle 0,05s den nächsten Schritt machen
	static int startTimer = millis();
	static int stopTimer = millis();

	if (millis() >= startTimer + startStepTime) {

		startTimer = millis();
		// Links
		if (currentVelocityPwmLeft < nextVelocityPwmLeft) {

			// Beschleunigen
			currentVelocityPwmLeft += startStepPwm;

			//	Wenn die Geschwindigkeit die maximale Geschwindigkeit übersteigt
			//	wird diese auf das Maximun gesetzt. Dies wird zusätzlich benötigt, damit
			// 	das System nicht wieder langsamer wird
			//
			if (currentVelocityPwmLeft >= nextVelocityPwmLeft) {
				currentVelocityPwmLeft = nextVelocityPwmLeft;
			}

		}
		// Rechts
		if (currentVelocityPwmRight < nextVelocityPwmRight) {

			// Beschleunigen
			currentVelocityPwmRight += startStepPwm;

			 //Wenn die Geschwindigkeit die maximale Geschwindigkeit übersteigt
			 //	wird diese auf das Maximun gesetzt. Dies wird zusätzlich benötigt, damit
			 //	das System nicht wieder langsamer wird
			 //
			if (currentVelocityPwmRight >= nextVelocityPwmRight) {
				currentVelocityPwmRight = nextVelocityPwmRight;
			}

		}

		// Geschwindigkeiten updaten
		analogWrite(pwmA, currentVelocityPwmLeft);
		analogWrite(pwmB, currentVelocityPwmRight);
	}
/*
	if (millis() >= stopTimer + stopStepTime) {

		stopTimer = millis();
		// Links
		if (currentVelocityPwmLeft > nextVelocityPwmLeft) {

			// Abbremsen
			currentVelocityPwmLeft -= stoptStepPwm;

			//	Wenn die Geschwindigkeit kleiner als die minimale Geschwindigkeit 0 ist,
			//	wird diese auf das Minimum gesetzt

			if (currentVelocityPwmLeft <= 0) {
				currentVelocityPwmLeft = 0;
			}

		}

		// Rechts
		if (currentVelocityPwmRight > nextVelocityPwmRight) {

			// Abbremsen
			currentVelocityPwmRight -= stoptStepPwm;

			//	Wenn die Geschwindigkeit kleiner als die minimale Geschwindigkeit 0 ist,
			// 	wird diese auf das Minimum gesetzt
			if (currentVelocityPwmRight <= 0) {
				currentVelocityPwmRight = 0;
			}

		}

		// Geschwindigkeiten updaten
		analogWrite(pwmA, currentVelocityPwmLeft);
		analogWrite(pwmB, currentVelocityPwmRight);
	}

}
*/
void Motor::updateVelocity(){
	analogWrite(pwmA, nextVelocityPwmLeft);
	analogWrite(pwmB, nextVelocityPwmRight);
}


void Motor::driveStraight() {
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
// Geschwindigkeiten für die Motoren einstellen

	nextVelocityPwmLeft = 150;
	nextVelocityPwmRight = 150;
	updateVelocity();

}
void Motor::driveStraight(unsigned char nextVelocityPwm) {
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = nextVelocityPwm;
	nextVelocityPwmRight = nextVelocityPwm;
	updateVelocity();
}

// bei Vorwärtsfahrt leichte Korrektur bei Abweichung vom Weg
void Motor::driveStraightLeft(unsigned char nextVelocityPwm){
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

	// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = nextVelocityPwm - driveOffset;
	nextVelocityPwmRight = nextVelocityPwm;

	updateVelocity();

}
// bei Vorwärtsfahrt leichte Korrektur bei Abweichung vom Weg
void Motor::driveStraightRight(unsigned char nextVelocityPwm){
	// checked
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

	// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = nextVelocityPwm;
	nextVelocityPwmRight = nextVelocityPwm - driveOffset;

	updateVelocity();
}

void Motor::turn(float speed) {
	if(speed <= 0) {
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



	nextVelocityPwmLeft = abs(speed) * 255;
	nextVelocityPwmRight = abs(speed) * 255;

	updateVelocity();
}

void Motor::turnLeft() {

	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = maxVelocityPwm;
	nextVelocityPwmRight = maxVelocityPwm;
	updateVelocity();

}


void Motor::turnRight() {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = maxVelocityPwm;
	nextVelocityPwmRight = maxVelocityPwm;

	updateVelocity();
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
void Motor::stopInstant() {

}

void Motor::testAnfahren() {

	static unsigned int time = millis();

	if(millis() >= time + 50){

		time = millis();

		Serial.print("links: ");
		Serial.print(currentVelocityPwmLeft);
		Serial.print(" ");
		Serial.print("rechts: ");
		Serial.print(currentVelocityPwmRight);
		Serial.println(" ");

	}

}
