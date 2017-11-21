#include "Statemachine.h"
#include <Arduino.h>
enum states {
	initState,
	nextPoint,
	turnToTargetAngle,
	startUp,
	driveStraight,
	avoidCrash,
	finished
};
static enum states currentState = nextPoint;
static unsigned long timeLast = millis();

StateMachine::StateMachine() {
	pinMode(switchPin, INPUT);
}

void StateMachine::UpdateData() {
	Navi.UpdateData();
}

void StateMachine::evalStateMachine() {
	switch (currentState) {
	case initState: {
		//Serial.println("In initState");
	}
		break;
	case nextPoint: {
		//Serial.println("In next Point");
	}
		break;
	case turnToTargetAngle: {
		Navi.turnToTargetAngle();
		//Serial.println("In turnToTargetAngle");
	}
		break;
	case startUp:{
		Navi.driveToTargetPosition();
		//Serial.println("In startUp")
	}
	case driveStraight: {
		Navi.driveToTargetPosition();
		//Serial.println("In driveStraight");
	}
		break;
	case avoidCrash: {
		//Serial.println("In avoidCrash");
	}
		break;
	case finished: {
		Navi.setSpeed(speedStop);
		Serial.println("In finished");
	}
		break;
	}
	//TODO: Taster einbauen lassen
	switch (currentState) {
	case initState: {
		if (switchPin == 1) {
			currentState = nextPoint;
			timeLast = timeCur;
			Navi.setNextPosition();
		}
	}
		break;

	case nextPoint: {
		// Kleiner Wartezustand
		// danach weiter drehen
		// kann unter Umständen entfernt werden
		// zur Verzögerung des Init-Zustandes nutzen?
		if ((timeCur - timeLast) >= interval ) {
			Navi.setSpeed(speedmaxturn);
			float targetAngle = Navi.getTargetAngle();
			Navi.setTargetAngle(targetAngle);
			currentState = turnToTargetAngle;
		}
	}
		break;

	case turnToTargetAngle: {
		if (Navi.getSpeed() == speedStop) {
			// Drehung fertig?
			Navi.setSpeed(speedStartUp);
			timeLast = timeCur;
			currentState = startUp;
		}
	}
		break;

	case startUp:{
		if (Navi.getJSON().getStopEnemy()){
			Navi.setSpeed(speedStop);
			currentState = avoidCrash;
		}else if((timeCur - timeLast) >= interval){
			Navi.setSpeed(speedmax);
			currentState = driveStraight;
		}
	}
		break;
	case driveStraight: {
		// TODO: Wenn e < Schutzradius und dann wieder größer wird. Neu drehen ohne Positon++
		if (Navi.getJSON().getStopEnemy()){
			Navi.setSpeed(speedStop);
			currentState = avoidCrash;
		}else if(Navi.getDeviation() < safetyDistance){
			// Zielkreis erreicht?
			Navi.setSpeed(speedStop);
			if (Navi.getPosition() == maxPosition){
				// Letzte Position erreicht?
				currentState = finished;
			}else{
				// Nächste Position anfahren?
				Navi.setNextPosition();
				timeLast = timeCur;
				currentState = nextPoint;
			}

		}
	}
		break;

	case avoidCrash: {
		currentQuarter = Navi.getCurrentQuarter();

		break;
	}
	case finished: {

	}
		break;
	}
}
