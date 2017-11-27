#include "Statemachine.h"
#include "Gerade.h"
#include "Vec.h"
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
		break;
	}
	case avoidCrash: {
		//Serial.println("avoid crash");
		// TODO: kurzes warten, Gegner noch da ? dann erst drehen
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
			timeStop = timeCur;
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
			currentQuarter = Navi.getCurrentQuarter();
			actualAvoidAngle = Navi.getOdometrie().getAngle();
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
		Gerade Vec1(Vec(0, 0), Vec(3000, 0).MakeUnit());	// Vec 1 und Vec 2 parallel
		Gerade Vec2(Vec(0, 2000), Vec(3000, 2000).MakeUnit());	// Vec 3 und Vec 4 parallel
		Gerade Vec3(Vec(0, 0), Vec(0, 2000).MakeUnit());
		Gerade Vec4(Vec(3000, 0), Vec(3000, 2000).MakeUnit());
		Vec o(Navi.getX(), Navi.getY());
		Vec r(Navi.getOdometrie().getAngle());
		Gerade Intersection(o, r);
		//gedrehte Richtungsvektoren: Schnittpunkt mit Spielfeldvektoren prüfen
		float a = Intersection.getIntersection(Vec1);
		float b = Intersection.getIntersection(Vec2);
		float c = Intersection.getIntersection(Vec3);
		float d = Intersection.getIntersection(Vec4);
		if (((timeCur - timeLast) >= intervalStop)){
			if (Master && Navi.getJSON().getStopEnemy() ) {
				if ((a >= 0) && (a <= 3000)){
					if (a > b){
						Navi.setTargetAngle(Navi.getOdometrie().getAngle() - 90);
					}else{
						Navi.setTargetAngle(Navi.getOdometrie().getAngle() + 90);
					}
				}else{
					if (c > d){
						Navi.setTargetAngle(Navi.getOdometrie().getAngle() + 90);
					}else{
						Navi.setTargetAngle(Navi.getOdometrie().getAngle() - 90);
					}
				}
				currentState = turnToTargetAngle;
			} else {
				if (!Navi.getJSON().getStopEnemy()) {
					currentState = driveStraight;
				}

			}
		timeLast = timeCur;
		}
	}
		break;

	case finished: {

	}
		break;
	}
}
