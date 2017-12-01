#include "Statemachine.h"
#include "Navigation.h"
#include "Gerade.h"
#include <Arduino.h>

// Allgemeine Todos
// TODO: Überhaupt nötig? Beacons nicht detektieren.
// TODO: Testen: Testen der Odometrie ohne Mopped
// TODO: Testen: Ausweichverhalten
// TODO: Testen: Wenn Zeit: Drehung auf Stelle optimieren
// TODO: Testen: Kommunikation

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
	finished,
	finishedOutOfTime
};
static enum states currentState = avoidCrash;
static unsigned long timeLast = millis();
static unsigned long timeStop = millis();
static unsigned long timeToPlay = millis();

Navigation& StateMachine::getNavigation(){
	return Navi;
}


StateMachine::StateMachine() {
	pinMode(switchPin, INPUT);
}

void StateMachine::UpdateData() {
	Navi.UpdateData();
	timeCur = millis();
	playTime = millis();
}

void StateMachine::evalStateMachine() {
	if ((playTime - timeToPlay) >= intervalPlaytime){
		Navi.setSpeed(speedStop);
		currentState = finishedOutOfTime;
		// TODO: Testen
	}
	switch (currentState) {
		case initState: {
		}
			break;
		case nextPoint: {
		}
			break;
		case turnToTargetAngle: {
			Navi.turnToTargetAngle();
			Serial.println("turnToTargetAngle");
		}
			break;
		case turnToAvoidTargetAngle: {
			Navi.turnToTargetAngle();
			Serial.println("turnToAvoidTargetAngle");
		}
			break;
		case driveShortlyStraight: {
			Navi.getMotor().driveStraight();
			Serial.println("driveShortlyStraight");
		}
			break;

		case startUp: {
			Navi.driveToTargetPosition();
			Navi.setSpeed(60);
			Serial.println("startUp");
		}
			break;
		case driveStraightRegulated: {
			Serial.println(Navi.getOdometrie().getAngle());
			Navi.driveToTargetPosition();
			Serial.println("driveStraightRegulated");
		}
			break;
		case stopMotor: {
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
			Navi.setTargetAngle(90);
			Navi.turnToTargetAngle();
		}
			break;
		case finishedOutOfTime: {
			Navi.getMotor().stop();

		}
			break;
	}
	switch (currentState) {
		case initState: {
			if (switchPin == 1) {
				currentState = nextPoint;
				timeLast = timeCur;
				timeToPlay = playTime;
				Navi.setNextPosition();
			}
		}
			break;

		case nextPoint: {
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
			//Vec o(Navi.getX(), Navi.getY());
			Vec o(1501, 100);
			Vec r(90);
			Serial.println(Navi.getOdometrie().getAngle());
			//Vec r(Navi.getOdometrie().getAngle());
			Gerade Intersection(o, r);
			//gedrehter Richtungsvektor: Schnittpunkt mit Spielfeldvektoren prüfen

			a = Intersection.getIntersection(G1);
			b = Intersection.getIntersection(G2);
			c = Intersection.getIntersection(G3);
			d = Intersection.getIntersection(G4);

			Serial.print("a  :");
			Serial.println(a);
			Serial.print("b  :");
			Serial.println(b);
			Serial.print("c :");
			Serial.println(c);
			Serial.print("d :");
			Serial.println(d);

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
			float lengthG1 = Navi.getLengthToPosition(IntersectionG1.getX(),
					IntersectionG1.getY());
			float lengthG2 = Navi.getLengthToPosition(IntersectionG2.getX(),
					IntersectionG2.getY());
			float lengthG3 = Navi.getLengthToPosition(IntersectionG3.getX(),
					IntersectionG3.getY());
			float lengthG4 = Navi.getLengthToPosition(3000,
					100);

			if ((IntersectionG1.getY() >= 0) && (IntersectionG2.getY() <= 2000)) {
				if ((IntersectionG1.getX() >= 0)
						&& (IntersectionG1.getX() <= 3000)) {
					if (lengthG1 > aimLength) {
						aimLength = lengthG1;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG1.getX(), IntersectionG1.getY());
					}
				}
			}
			if ((IntersectionG2.getY() >= 0) && (IntersectionG2.getY() <= 2000)) {
				if ((IntersectionG2.getX() >= 0)
						&& (IntersectionG2.getX() <= 3000)) {
					if (lengthG2 > aimLength) {
						aimLength = lengthG2;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG2.getX(), IntersectionG2.getY());
					}
				}
			}
			if ((IntersectionG3.getY() >= 0) && (IntersectionG3.getY() <= 2000)) {
				if ((IntersectionG3.getX() >= 0)
						&& (IntersectionG3.getX() <= 3000)) {
					if (lengthG3 > aimLength) {
						aimLength = lengthG3;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG3.getX(), IntersectionG3.getY());
					}
				}
			}
			if ((IntersectionG4.getY() >= 0) && (IntersectionG4.getY() <= 2000)) {
				if ((IntersectionG4.getX() >= 0)
						&& (IntersectionG4.getX() <= 3000)) {
					if (lengthG4 > aimLength) {
						aimLength = lengthG4;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG4.getX(), IntersectionG4.getY());
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
			delay(3000);

			if (((timeCur - timeLast) >= intervalStop)) {
				if (Master && Navi.getJSON().getStopEnemy()) {
					timeLast = timeCur;
					Navi.setSpeed(speedmaxturn);
					currentState = turnToAvoidTargetAngle;
				} else {
					if (!Navi.getJSON().getStopEnemy()) {
						timeLast = timeCur;
						Navi.setSpeed(speedStartUp);
						currentState = startUp;
					}

				}
			}

		}
			break;

		case finished: {

		}
			break;
		case finishedOutOfTime: {

		}
			break;
	}
}
