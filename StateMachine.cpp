#include "Statemachine.h"
#include <Arduino.h>
enum states {
	initState,
	nextPoint,
	turnToTargetAngle,
	startUp,
	driveStraight,
	stopMotor,
	avoidCrash,
	finished
};
static enum states currentState = nextPoint;
static unsigned long timeLast = millis();
static unsigned long timeStop = millis();

StateMachine::StateMachine() {
	pinMode(switchPin, INPUT);
}

void StateMachine::UpdateData() {
	Navi.UpdateData();
	timeCur = millis();
}

void StateMachine::evalStateMachine() {
	switch (currentState) {
	case initState: {
//Serial.println("In initState");
	}
		break;
	case nextPoint: {
		//Serial.println("In nextPoint");
	}
		break;
	case turnToTargetAngle: {
		Navi.turnToTargetAngle();
		//Serial.println("In turn to Target Angle");
	}
	break;
	case startUp: {
		//Serial.println("In startUp");
		Navi.driveToTargetPosition();
		Navi.setSpeed(60);
	}
	break;
	case driveStraight: {
		//Serial.println("Drive Straight");
		Serial.println(Navi.getOdometrie().getAngle());
		Navi.driveToTargetPosition();
	}
		break;
	case stopMotor: {
		//Serial.println("Stop Motor");
		Navi.getMotor().stoppInstantForward(speedmax);
		if (timeCur >= timeStop + 40) {
			Navi.setSpeed(speedStop);
			Navi.getMotor().stop();
		}
	}
		break;
	case avoidCrash: {
		//Serial.println("avoid crash");
	}
		break;
	case finished: {
		//Serial.println("finished");
		Navi.setTargetAngle(90);
		Navi.turnToTargetAngle();
		//Navi.setSpeed(speedStop);
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
		if ((timeCur - timeLast) >= interval) {
			Navi.setSpeed(speedmaxturn);
			Navi.setTargetAngle(Navi.getTargetAngle());
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

	case startUp: {
		if (Navi.getJSON().getStopEnemy()) {
			Navi.setSpeed(speedStop);
			currentState = avoidCrash;
		} else if ((timeCur - timeLast) >= interval) {
			Navi.setSpeed(150);
			currentState = driveStraight;
		}
	}
		break;

	case driveStraight: {
		if (Navi.getJSON().getStopEnemy()) {
			Navi.setSpeed(speedStop);
			timeStop = timeCur;
			currentState = stopMotor;
		} else if (Navi.getDeviation() < Navi.getSafetyRadius()) {
			// Zielkreis erreicht?
			Navi.setSpeed(speedmax);
			timeStop = timeCur;
			currentState = stopMotor;
		}
	}
		break;

	case stopMotor: {
		if (Navi.getJSON().getStopEnemy()) {
			currentState = avoidCrash;
		} else if (Navi.getPosition() == Navi.getMaximalPosition()) {
			// Letzte Position erreicht?
			Navi.setSpeed(speedmaxturn);
			currentState = finished;
		} else if (Navi.getSpeed() == 0) {
			timeLast = timeCur;
			Navi.setNextPosition();
			currentState = nextPoint;
		}

	}
		break;
	case avoidCrash: {
		currentQuarter = Navi.getCurrentQuarter();
	}
		break;

	case finished: {

	}
		break;

	}
}
