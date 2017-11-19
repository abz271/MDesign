#include "Statemachine.h"
#include <Arduino.h>
enum states {
	initState,
	nextPoint,
	turnToTargetAngle,
	driveStraight,
	driveStraightRight,
	driveStraightLeft,
	avoidCrash,
	finished
};
static enum states currentState = nextPoint;
static unsigned long timeLastly = millis();

StateMachine::StateMachine() {
	pinMode(switchPin, INPUT);
}

void StateMachine::UpdateData() {
	Navi.UpdateData();
}

void StateMachine::evalStateMachine() {
	switch (currentState) {
	case initState: {
		// zum Testen
		Navi.getOdometrie().testOdometrie();
		//Serial.println("In initState");
	}
		break;
	case nextPoint: {

	}
		break;
	case turnToTargetAngle: {
		Navi.turnToTargetAngle();
		//Serial.println("In turnToTargetAngle");
	}
		break;
	case driveStraight: {
		Serial.println("In driveStraight");
	}
		break;

	case driveStraightLeft: {
		//Serial.println("In driveStraightLeft");
	}
		break;
	case driveStraightRight: {
		//Serial.println("In driveStraightRight");
	}
		break;
	case avoidCrash: {
		//Serial.println("In avoidCrash");
	}
		break;
	case finished: {
		//Serial.println("In finished");
	}
		break;
	}
	//TODO: Taster einbauen lassen
	switch (currentState) {
	case initState: {
		if (switchPin == 1) {
			currentState = nextPoint;
		}
	}
		break;

	case nextPoint: {
		// Kleiner Wartezustand
		// danach weiter drehen
		unsigned int timeCurrently = millis();
		if (timeCurrently >= timeLastly + 4000) {
			timeLastly = timeCurrently;
			Navi.setSpeed(speedmax);
			float targetAngle = Navi.getTargetAngle();
			Navi.setTargetAngle(targetAngle);
			currentState = turnToTargetAngle;
		}
	}
		break;

	case turnToTargetAngle: {
		if (Navi.getSpeed() == speedStop) {
			Serial.println("Navi.getSpeed()");
			Navi.setSpeed(speedmax);
			currentState = driveStraight;
		}
	}
		break;

	case driveStraight: {
	}
		break;

	case driveStraightRight: {
	}

		break;

	case driveStraightLeft: {
	}

		break;

	case avoidCrash: {

	}

		break;
	case finished: {

	}

		break;
	}
}
