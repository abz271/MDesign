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
		Vec r(Navi.getOdometrie().getAngle());
		Gerade Intersection(o, r);
		//gedrehter Richtungsvektor: Schnittpunkt mit Spielfeldvektoren prüfen
		float a = G1.getIntersection(Intersection);		// x-Wert g1 : y-Wert 0
		float b = G2.getIntersection(Intersection);		// x-Wert g2 : y-Wert 2000
		float c = G3.getIntersection(Intersection);		// y-Wert g3 : x-Wert 0
		float d = G4.getIntersection(Intersection);		// y-Wert g4 : x-Wert 3000
		float aimLength = 0;
		float lengthA = 0;
		float lengthB = 0;
		float lengthC = 0;
		float lengthD = 0;

		if ((a >= 0) && (a <= 3000)) {
			float PositionA[2] = {a, 0};
			lengthA = Navi.getLengthToPosition(PositionA[0], PositionA[1]);
			if (lengthA > aimLength){
				aimLength = lengthA;
				actualAvoidAngle = Navi.getCalculateAngle(PositionA[0], PositionA[1]);
			}
		}
		if ((b >= 0) && (b <= 3000)) {
			float PositionB[2] = {b, 2000};
			lengthB = Navi.getLengthToPosition(PositionB[0], PositionB[1]);
			if (lengthB > aimLength){
				aimLength = lengthB;
				actualAvoidAngle = Navi.getCalculateAngle(PositionB[0], PositionB[1]);
			}
		}
		if ((c >= 0) && (c <= 2000)) {
			float PositionC[2] = { 0, c};
			lengthC = Navi.getLengthToPosition(PositionC[0], PositionC[1]);
			if (lengthC > aimLength){
				aimLength = lengthC;
				actualAvoidAngle = Navi.getCalculateAngle(PositionC[0], PositionC[1]);
			}
		}
		if ((d >= 0) && (d <= 2000)) {
			float PositionD[2] = { 3000, d};
			lengthD = Navi.getLengthToPosition(PositionD[0], PositionD[1]);
			if (lengthD > aimLength){
				aimLength = lengthD;
				actualAvoidAngle = Navi.getCalculateAngle(PositionD[0], PositionD[1]);
			}
		}
		Serial.print("aimLength  :");
		Serial.println(aimLength);
		Serial.print("lengthA  :");
		Serial.println(lengthA);
		Serial.print("lengthB  :");
		Serial.println(lengthB);
		Serial.print("lengthC  :");
		Serial.println(lengthC);
		Serial.print("lengthD  :");
		Serial.println(lengthD);

		Navi.setTargetAngle(actualAvoidAngle);
		Serial.print("actualAvoidAngle  :");
		Serial.println(actualAvoidAngle);
		Serial.print("a  :");
		Serial.println(a);
		Serial.print("b  :");
		Serial.println(b);
		Serial.print("c  :");
		Serial.println(c);
		Serial.print("d  :");
		Serial.println(d);

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
