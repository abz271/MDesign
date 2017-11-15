#include "Motor.h"
#include <Arduino.h>

Motor::Motor() {
}

Motor::~Motor() {
}

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

			/*	Wenn die Geschwindigkeit die maximale Geschwindigkeit übersteigt
			 * 	wird diese auf das Maximun gesetzt. Dies wird zusätzlich benötigt, damit
			 * 	das System nicht wieder langsamer wird
			 */
			if (currentVelocityPwmLeft >= nextVelocityPwmLeft) {
				currentVelocityPwmLeft = nextVelocityPwmLeft;
			}

		}
		// Rechts
		if (currentVelocityPwmRight < nextVelocityPwmRight) {

			// Beschleunigen
			currentVelocityPwmRight += startStepPwm;

			/*	Wenn die Geschwindigkeit die maximale Geschwindigkeit übersteigt
			 * 	wird diese auf das Maximun gesetzt. Dies wird zusätzlich benötigt, damit
			 * 	das System nicht wieder langsamer wird
			 */
			if (currentVelocityPwmRight >= nextVelocityPwmRight) {
				currentVelocityPwmRight = nextVelocityPwmRight;
			}

		}

		// Geschwindigkeiten updaten
		analogWrite(pwmA, currentVelocityPwmLeft);
		analogWrite(pwmB, currentVelocityPwmRight);
	}

	if (millis() >= stopTimer + stopStepTime) {

		stopTimer = millis();
		// Links
		if (currentVelocityPwmLeft > nextVelocityPwmLeft) {

			// Abbremsen
			currentVelocityPwmLeft -= stoptStepPwm;

			/*	Wenn die Geschwindigkeit kleiner als die minimale Geschwindigkeit 0 ist,
			 * 	wird diese auf das Minimum gesetzt
			 */
			if (currentVelocityPwmLeft <= 0) {
				currentVelocityPwmLeft = 0;
			}

		}

		// Rechts
		if (currentVelocityPwmRight > nextVelocityPwmRight) {

			// Abbremsen
			currentVelocityPwmRight -= stoptStepPwm;

			/*	Wenn die Geschwindigkeit kleiner als die minimale Geschwindigkeit 0 ist,
			 * 	wird diese auf das Minimum gesetzt
			 */
			if (currentVelocityPwmRight <= 0) {
				currentVelocityPwmRight = 0;
			}

		}

		// Geschwindigkeiten updaten
		analogWrite(pwmA, currentVelocityPwmLeft);
		analogWrite(pwmB, currentVelocityPwmRight);
	}
}

void Motor::driveStraight() {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = maxVelocityPwm;
	nextVelocityPwmRight = maxVelocityPwm;

}
void Motor::driveStraight(unsigned char nextVelocityPwm) {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = nextVelocityPwm;
	nextVelocityPwmRight = nextVelocityPwm;

}

void Motor::turnLeft() {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = maxVelocityPwm;
	nextVelocityPwmRight = maxVelocityPwm;

}

void Motor::turnLeft(unsigned char nextVelocityPwm) {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = nextVelocityPwm;
	nextVelocityPwmRight = nextVelocityPwm;

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

}
void Motor::turnRight(unsigned char nextVelocityPwm) {

// Pins einstellen um vorwärts zu fahren
// TODO: Richtung einstellen
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = nextVelocityPwm;
	nextVelocityPwmRight = nextVelocityPwm;

}

void Motor::stop() {

// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = 0;
	nextVelocityPwmRight = 0;

}
void Motor::stopInstant() {

}

void Motor::testAnfahren() {

	static int time = millis();

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
