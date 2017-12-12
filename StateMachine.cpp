#include "Statemachine.h"
#include "Navigation.h"
#include "Gerade.h"
#include <Arduino.h>

// Allgemeine Todos
// TODO: Testen: Testen der Odometrie ohne Mopped
// TODO: Testen: Testen der Odometrie mit Mopped
// TODO: Testen: Ausweichverhalten ; nach Umbau noch nicht getestet, neu: Lava
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

static enum states currentState = initState;
static unsigned long timeLast = millis();
static unsigned long timeStop = millis();
static unsigned long timeToPlay = millis();

Navigation& StateMachine::getNavigation(){
	return Navi;
}

StateMachine::StateMachine() {
	pinMode(switchPin, INPUT_PULLUP);
}


// Aktualisiert alle Daten aus Odometrie und Kommunikation
void StateMachine::UpdateData() {
	Navi.UpdateData();
	timeCur = millis();
	playTime = millis();
}

void StateMachine::evalStateMachine() {
	if ((playTime - timeToPlay) >= intervalPlaytime){
		if (currentState != initState){
			Navi.setSpeed(speedStop);
			currentState = finishedOutOfTime;
		}
	}
	Navi.setPrePositionteams(true);
	Navi.setPositionteam(false);
	switch (currentState) {
		case initState: {

			Navi.setPositionteam(false);
			timeToPlay = playTime;
		}
			break;
		case nextPoint: {
			playTime = millis();
			break;
		}
		case turnToTargetAngle: {
			Navi.turnToTargetAngle();
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
			Navi.driveToTargetPosition();
			Navi.setSpeed(60);
		}
			break;
		case driveStraightRegulated: {
			Navi.setPrePositionteams(false);
			Navi.setPositionteam(true);
			Navi.driveToTargetPosition();
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
			if (!digitalRead(switchPin)) {
				timeLast = timeCur;
				timeToPlay = playTime;
				currentState = nextPoint;
				Serial.println("Von Init nach next Point");
			}
		}
			break;

		case nextPoint: {
			if ((timeCur - timeLast) >= interval) {
				Navi.setSpeed(speedmaxturn);
				Navi.setTargetAngle(Navi.getTargetAngle());
				currentState = turnToTargetAngle;
				Serial.println("von nextPoint nach turnToTargetAngle");
			}
		}
			break;

		case turnToTargetAngle: {
			if (Navi.getSpeed() == speedStop) {
				// Drehung fertig?
				Navi.setSpeed(speedStartUp);
				timeLast = timeCur;
				currentState = startUp;
				Serial.println("von turnToTargetAngle nach startUp");
			}
		}
			break;

		case turnToAvoidTargetAngle: {
			if (Navi.getSpeed() == speedStop) {
				// Drehung fertig?
				Navi.setSpeed(speedStartUp);
				timeLast = timeCur;
				currentState = driveShortlyStraight;
				Serial.println("von turnToAvoidTargetAngle nach driveShortlyStraight");
			}
		}
			break;
		case startUp: {
			if (Navi.getJSON().getStopEnemy() && Navi.DetectedEnemyInArea()) {
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("von startUp nach stopMotor");
			} else if ((timeCur - timeLast) >= interval) {
				Navi.setSpeed(150);
				currentState = driveStraightRegulated;
				Serial.println("von startUp nach driveStraightRegulated");
			}
		}
			break;

		case driveShortlyStraight: {
			if (Navi.getJSON().getStopEnemy()&& Navi.DetectedEnemyInArea()) {
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("von driveShortlyStraight nach stopMotor (Gegner erkannt)");
			}else if ((timeCur - timeLast) >= intervalDrive) {
				Navi.setSpeed(speedmaxturn);
				timeLast = timeCur;
				currentState = nextPoint;
				Serial.println("von driveShortlyStraight nach nextPoint");
			}
		}
			break;
		case driveStraightRegulated: {
			if (Navi.getJSON().getStopEnemy()&& Navi.DetectedEnemyInArea()) {
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("von driveStraightRegulated nach stopMotor (Gegner erkannt)");
			} else if (Navi.getDeviation() < Navi.getSafetyRadius()) {
				// Zielkreis erreicht?
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("von driveStraightRegulated nach stopMotor (Ziel erreicht)");
			} else if (Navi.CrashIncoming() && 0){
				// Zu nah an den Spielplanken
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("von driveStraightRegulated nach stopMotor (Planken zu nah)");
			}
		}
			break;

		case stopMotor: {
			// Weiterschalten in AvoidCrash
			if (Navi.getJSON().getStopEnemy()) {
				currentState = avoidCrash;
				Serial.println("von stopMotor nach avoidCrash");
			// Letzte Position erreicht?
			} else if (Navi.getPosition() == Navi.getMaximalPosition()) {
				Navi.setSpeed(speedmaxturn);
				currentState = finished;
				Serial.println("von stopMotor nach finished");
			// Wenn an Position gebremst worden ist, nächste Position anfahren
			} else if ((Navi.getSpeed() == 0) && /*(!digitalRead(switchPin))*/1)  {
				timeLast = timeCur;
				Navi.setNextPosition();
				currentState = nextPoint;
				Serial.println("von stopMotor nach nextPoint");
			}

		}
			break;
		case avoidCrash: {
			// Spielfeldgeraden anlegen

			/*************G2*************
			 * 							*
			 * 	  						*
			 * 							*
			 G3 	  					G4
			 * 		 					*
			 * 							*
			 * 							*
			 *************G1*************/
			Gerade G1(Vec(0, 0), Vec(1, 0));		// G1 und G2 parallel x-Achse
			Gerade G2(Vec(0, 2000), Vec(1, 0));
			Gerade G3(Vec(0, 0), Vec(0, 1));		// G3 und G4 parallel y-Achse
			Gerade G4(Vec(3000, 0), Vec(0, 1));

			// Vektor des Autos anlegen mit gedrehten Richtungsvektor um 90°
			Vec o(Navi.getX(), Navi.getY());
			Vec r(Navi.getOdometrie().getAngle());
			// Gerade des Autos erzeugen
			Gerade Intersection(o, r);

			//gedrehter Richtungsvektor: Prüfen ob, Schnittpunkt mit Spielfeldvektoren existieren
			a = Intersection.getIntersection(G1);
			b = Intersection.getIntersection(G2);
			c = Intersection.getIntersection(G3);
			d = Intersection.getIntersection(G4);

			// Schnittpunkte in Vektoren einspeichern
			Vec IntersectionG1 = Intersection.getDirectVec(a);
			Vec IntersectionG2 = Intersection.getDirectVec(b);
			Vec IntersectionG3 = Intersection.getDirectVec(c);
			Vec IntersectionG4 = Intersection.getDirectVec(d);

			//Länge zu Schnittpunkten berechnen
			float aimLength = 0;
			float lengthG1 = Navi.getLengthToPosition(IntersectionG1.getX(),
					IntersectionG1.getY());
			float lengthG2 = Navi.getLengthToPosition(IntersectionG2.getX(),
					IntersectionG2.getY());
			float lengthG3 = Navi.getLengthToPosition(IntersectionG3.getX(),
					IntersectionG3.getY());
			float lengthG4 = Navi.getLengthToPosition(IntersectionG4.getX(),
					IntersectionG4.getY());

			// Überprüfen, welcher Schnittpunkt Sinn ergibt und welcher am weitesten entfernt ist
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
			// Ausweichwinkel abspeichern
			Navi.setTargetAngle(actualAvoidAngle);

			// Ausweichsignal war nur kurz da?
			if (!Navi.getJSON().getStopEnemy()){
				timeLast = timeCur;
				Navi.setSpeed(speedStartUp);
				currentState = startUp;
			// Warte kurz und handel entsprechend deiner Bestimmung:
			}else if ((timeCur - timeLast) >= intervalStop) {
				// Masterfahrzeug: Nach Zeitablauf zu neuem Winkel drehen
				if (Master) {
					timeLast = timeCur;
					Navi.setSpeed(speedmaxturn);
					currentState = turnToAvoidTargetAngle;
					Serial.println("von AvoidCrash nach turnToAvoidTargetAngle (Master)");
				// Betafahrzeug: Wenn Zeit abgelaufen ist und das andere Fahrzeug verschwunden ist weiterfahren
				} else {
					if (!Navi.getJSON().getStopEnemy()) {
						timeLast = timeCur;
						Navi.setSpeed(speedStartUp);
						currentState = startUp;
						Serial.println("von avoidCrash nach nextPoint (Slave)");
					}
				}
			}

		}
			break;

		case finished: {
			if (!digitalRead(switchPin)) {
				timeLast = timeCur;
				timeToPlay = playTime;
				Navi.setPosition(0);
				currentState = nextPoint;
				Serial.println("von finished nach nextPoint");
			}
		}
			break;

		case finishedOutOfTime: {
			if ((!digitalRead(switchPin) && (Navi.getSpeed() == 0))) {
				timeLast = timeCur;
				timeToPlay = playTime;
				Navi.setPosition(0);
				currentState = nextPoint;
				Serial.println("von finishedOutOfTime nach nextPoint");
			}
		}
			break;
	}
}
