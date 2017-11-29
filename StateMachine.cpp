#include "Statemachine.h"
#include "Navigation.h"
#include "Gerade.h"
#include <Arduino.h>
enum states {
	initState,
	nextPoint,
	turnToTargetAngle,
	turnToAvoidTargetAngle,
	startUp,
	driveShortlyStraight,
	driveStraightRegulated,
	stopMotor,
	avoidCrash,
	finished
};
static enum states currentState = avoidCrash;
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
	case turnToAvoidTargetAngle: {
		Navi.turnToTargetAngle();
	}
		break;
	case driveShortlyStraight: {
		Navi.getMotor().driveStraight();
	}
		break;

	case startUp: {
		//Serial.println("In startUp");
		Navi.driveToTargetPosition();
		Navi.setSpeed(60);
	}
		break;
	case driveStraightRegulated: {
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

	case turnToAvoidTargetAngle: {
		if (Navi.getSpeed() == speedStop) {
			Navi.setSpeed(speedStartUp);
			timeLast = timeCur;
			currentState = driveShortlyStraight;
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
			currentState = driveStraightRegulated;
		}
	}
		break;

	case driveShortlyStraight: {
		if ((timeCur - timeLast) >= intervalDrive) {
			Navi.setSpeed(speedmaxturn);
			timeLast = timeCur;
			currentState = nextPoint;
		}
	}
		break;
	case driveStraightRegulated: {
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
		Gerade G1(Vec(0, 0), Vec(1, 0));		// G1 und G2 parallel x-Achse
		Gerade G2(Vec(0, 2000), Vec(1, 0));
		Gerade G3(Vec(0, 0), Vec(0, 1));		// G3 und G4 parallel y-Achse
		Gerade G4(Vec(3000, 0), Vec(0, 1));
		Vec o(Navi.getX(), Navi.getY());
		Serial.println(Navi.getOdometrie().getAngle());
		Vec r(Navi.getOdometrie().getAngle());
		Gerade Intersection(o, r);
		//gedrehter Richtungsvektor: Schnittpunkt mit Spielfeldvektoren prüfen
		float a = Intersection.getIntersection(G1);
		float b = Intersection.getIntersection(G2);
		float c = Intersection.getIntersection(G3);
		float d = Intersection.getIntersection(G4);

		Vec IntersectionG1 = Intersection.getDirectVec(a);
		Vec IntersectionG2 = Intersection.getDirectVec(b);
		Vec IntersectionG3 = Intersection.getDirectVec(c);
		Vec IntersectionG4 = Intersection.getDirectVec(d);

		Serial.print("Schnittpunkt G1x :");
		Serial.println(IntersectionG1.getX());
		Serial.print("Schnittpunkt G1y  :");
		Serial.println(IntersectionG1.getY());
		Serial.print("Schnittpunkt G2x :");
		Serial.println(IntersectionG2.getX());
		Serial.print("Schnittpunkt G2y  :");
		Serial.println(IntersectionG2.getY());
		Serial.print("Schnittpunkt G3x :");
		Serial.println(IntersectionG3.getX());
		Serial.print("Schnittpunkt G3y  :");
		Serial.println(IntersectionG3.getY());
		Serial.print("Schnittpunkt G4x :");
		Serial.println(IntersectionG4.getX());
		Serial.print("Schnittpunkt G4y  :");
		Serial.println(IntersectionG4.getY());

		float aimLength = 0;
		float lengthG1 = IntersectionG1.getLength();
		float lengthG2 = IntersectionG2.getLength();
		float lengthG3 = IntersectionG3.getLength();
		float lengthG4 = IntersectionG4.getLength();

		if ((IntersectionG1.getY() >= 0) && (IntersectionG2.getY() <= 3000)) {
			if ((IntersectionG1.getX() >= 0)
					&& (IntersectionG1.getX() <= 2000)) {
				if (lengthG1 > aimLength) {
					aimLength = lengthG1;
					actualAvoidAngle = IntersectionG1.getAngle();
				}
			}
		}
		if ((IntersectionG2.getY() >= 0) && (IntersectionG2.getY() <= 3000)) {
			if ((IntersectionG2.getX() >= 0)
					&& (IntersectionG2.getX() <= 2000)) {
				if (lengthG2 > aimLength) {
					aimLength = lengthG2;
					actualAvoidAngle = IntersectionG2.getAngle();
				}
			}
		}
		if ((IntersectionG3.getY() >= 0) && (IntersectionG3.getY() <= 3000)) {
			if ((IntersectionG3.getX() >= 0)
					&& (IntersectionG3.getX() <= 2000)) {
				if (lengthG3 > aimLength) {
					aimLength = lengthG3;
					actualAvoidAngle = IntersectionG3.getAngle();
				}
			}
		}
		if ((IntersectionG4.getY() >= 0) && (IntersectionG4.getY() <= 3000)) {
			if ((IntersectionG4.getX() >= 0)
					&& (IntersectionG4.getX() <= 2000)) {
				if (lengthG4 > aimLength) {
					aimLength = lengthG4;
					actualAvoidAngle = IntersectionG4.getAngle();
				}
			}
		}
		Serial.print("aimLength  :");
		Serial.println(aimLength);
		Serial.print("lengthG1  :");
		Serial.println(lengthG1);
		Serial.print("lengthG2  :");
		Serial.println(lengthG2);
		Serial.print("lengthG3  :");
		Serial.println(lengthG3);
		Serial.print("lengthG4  :");
		Serial.println(lengthG4);

		Navi.setTargetAngle(actualAvoidAngle);
		Serial.print("actualAvoidAngle  :");
		Serial.println(actualAvoidAngle);

		if (((timeCur - timeLast) >= intervalStop)) {
			if (Master && Navi.getJSON().getStopEnemy()) {
				timeLast = timeCur;
				currentState = turnToAvoidTargetAngle;
			} else {
				if (!Navi.getJSON().getStopEnemy()) {
					timeLast = timeCur;
					currentState = driveStraightRegulated;
				}

			}
		}

	}
		break;

	case finished: {

	}
		break;
	}
}
