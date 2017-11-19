#include "Odometrie.h"
#include "Navigation.h"
#include "Motor.h"
#include <Arduino.h>

// Allgemeine Todos
// TODO: Timer f¸r das automatischen Beenden der Fahrt
// TODO: Taster implementieren, der die Fahrt beginnt.
// TODO: Offsets f¸r Position -> Wegl‰nge nutzen?
// TODO: Offsets f¸r Winkel
// TODO: Timerfunktion schreiben mit Zeitwert als ‹bergabe.
Navigation::Navigation(){
	Position = 0;
}

Odometrie& Navigation::getOdometrie(){
	return Odo;
}

Motor& Navigation::getMotor(){
	return Moto;
}

Kommunikation& Navigation::getJSON(){
	return JSON;
}


// Die aktuellen Positionsdaten werden mit den Werten aus der Odometrie und dem Positionsteam angepasst
void Navigation::UpdateData() {
	Odo.updateOdometrie();
	Moto.updateVelocity();
	if (JSON.getSignalUsefull()) {
		JSON.getPosition(x_aktuell, y_aktuell);
		Odo.setPosition(x_aktuell, y_aktuell);
	} else {
		x_aktuell = Odo.getX_position();
		y_aktuell = Odo.getY_position();
	}


}

// CalculateAngle gibt den um sich zu drehenden Winkel mit dem
// k√ºrzesten Drehwinkel an ->  auch negative Zahlen

double Navigation::CalculateAngle(int x, int y) {

	double delta_x = x - x_aktuell;
	double delta_y = y - y_aktuell;
	double Winkel = atan2(delta_y, delta_x) * 180 / PI;

	return Winkel;
}

int Navigation::GetYaktuell(){
	return y_aktuell;
}

int Navigation::ConsiderOffset(int Coordinate) {
	int ConsiderdCoordinate = Coordinate - CoordinateOffset;
	return ConsiderdCoordinate;
}

// Berechnet die Vektorl√§nge zwischen dem aktuellem Standort und dem Zielpunkt.
// Wird evtl nicht mehr gebraucht
double Navigation::LengthToPosition(int x, int y) {
	int delta_x = x - x_aktuell;
	int delta_y = y - y_aktuell;
	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}


// Einfache Fahrt gerade aus mit gleicher Beschleunigung auf beiden Motoren
// Sonderfall: Abweichnung um einen kleinen Winkel -> Offset um gegenan zu steuern
void Navigation::DriveStraightForward() {
	TargetAngleNew = Odo.getAngle();	// Eingeschlagenen Winkel aktualisieren
	// Falls das Auto im Toleranzbereich des Ursprungswinkel f‰hrt, einfach gerade aus fahren
	if ((TargetAngleNew <= (ActualTargetAngle + angleTolerance)) || (TargetAngleNew >= (ActualTargetAngle - angleTolerance))) {
		Moto.driveStraight(speed);
	} else if (TargetAngleNew > (ActualTargetAngle + angleTolerance)) {	// falls Drehwinkel aus Odometrie zu groﬂ wird
		Moto.driveStraightLeft(speed);			// Fahrrichtung langsam korrigieren
	} else if (TargetAngleNew < (ActualTargetAngle - angleTolerance)) {	// falls Drehwinkel aus Odometrie zu klein wird
		Moto.driveStraightRight(speed);		// Fahrrichtung langsam korrigieren
	}
}

// Drehung um 90 Grad nach mathematisch neg. 90 ∞
void Navigation::rotateLeft90(){
	// checked
	float actualRotationAngle = Odo.getAngle();
	float aimRotationAngle;
	aimRotationAngle = actualRotationAngle - 90.0;
	Moto.turnLeft();
	while(actualRotationAngle > aimRotationAngle){
		actualRotationAngle = Odo.getAngle();
		Odo.updateOdometrie();
	}
	Serial.println(Odo.getAngle());
	Moto.stop();
}
// Drehung um 90 Grad nach mathematisch pos. 90 ∞
void Navigation::rotateRight90(){
	// checked
	Odo.updateOdometrie();
	float actualRotationAngle = Odo.getAngle();	// Hole dir den aktuellen Fahrtwinkel
	float aimRotationAngle;
	aimRotationAngle = actualRotationAngle + 90.0;	// Addiere Drehwinkel auf
	Moto.turnRight();
	while(actualRotationAngle < (aimRotationAngle)){	// Dreh dich solange, bis du um Drehwinkel gedreht hast
		Odo.updateOdometrie();
		actualRotationAngle = Odo.getAngle();
	}
	Moto.stop();
}
void Navigation::AvoidClash() {
	unsigned long currentState = 0;
	if (x_aktuell < Spielfeldbreite/2){
		if (y_aktuell < Spielfeldhoehe/2){
			currentState = 1;	// Aktueller Aufenthalt in Quartal 1: x=0, y=0 bis x=999, y = 1499
		}else{
			currentState = 2;	// Aktueller Aufenthalt in Quartal 2: x=0, y=1500 bis x=999, y = 3000
		}
	}else{
		if (y_aktuell < Spielfeldbreite *0.5){
			currentState = 3;	// Aktueller Aufenthalt in Quartal 3: x=1000, y=0 bis x=2000, y = 1499
		}else{
			currentState = 4;	// Aktueller Aufenthalt in Quartal 4: x=1000, y=1500 bis x=2000, y = 3000
		}
	}
// TODO: kurzes warten, Gegner noch da ? dann erst drehen
	unsigned int time = 0;
	switch (currentState) {
	// TODO: Lavabereich
	// TODO: Wie lange gerade aus fahren?
	case 1:
		// TODO: Ausweichverhalten f¸r Quartal 1
		if (Odo.getAngle() < 90) {
			rotateRight90();
		} else {
			rotateLeft90();
		}
		time = millis();
		while(time < maxTime){
			Moto.updateVelocity();
		}
		Moto.driveStraight(speed);
		break;
	case 2:
		// TODO: Ausweichverhalten f¸r Quartal 2
		if ((abs(Odo.getAngle()) < 0)) {
			rotateRight90();
		} else {
			rotateLeft90();
		}
		time = millis();
		while(time < maxTime){
			Moto.driveStraight(speed);
		}
		break;
	case 3:
		// TODO: Ausweichverhalten f¸r Quartal 3
		if (Odo.getAngle() > 0) {
			rotateRight90();
		} else {
			rotateLeft90();
		}
		time = millis();
		while(time < maxTime){
			Moto.driveStraight(speed);
		}
		break;
	case 4:
		// TODO: Ausweichverhalten f¸r Quartal 4
		if ((abs(Odo.getAngle()) < 90)) {
			rotateLeft90();
		} else {
			rotateRight90();
		}
		time = millis();
		while(time < maxTime){
			Moto.driveStraight(speed);
		}
		break;
	}
}

// Programm beenden
bool Navigation::finished(){
	return (x_aktuell == X_Koordinaten[MaxPositions])
			&& (y_aktuell == Y_Koordinaten[MaxPositions]);
}

bool Navigation::NotAtPoint() {
	return (x_aktuell != X_Koordinaten[Position])
			&& (y_aktuell != Y_Koordinaten[Position]);
}

void Navigation::drive() {
	// TODO: Zum "Zielbaustein" drehen, danach erst zum n‰chsten Punkt fahren
	// While solange wir nicht am Ende angekommen sind
	while (!finished()) {

		// Zielwinkel  bestimmen
		ActualTargetAngle = CalculateAngle(X_Koordinaten[Position],
				Y_Koordinaten[Position]);
		// Fahrzeug Richtung Ziel drehen
		while (ActualTargetAngle != Odo.getAngle()) {
			if (ActualTargetAngle < 0){
				Moto.turnLeft();
			}else{
				Moto.turnRight();
			}
			UpdateData();
		}
		// Gerade zum Ziel Fahren
		while (NotAtPoint()) {
			unsigned int time = 0;
			// Gegner ausweichen?
			if (JSON.getStopEnemy()) {
				Moto.stop();	// Daten An Motoren = 0;
				if (Master) { // Abfrage, ob eigenes Fahrzeug ausweichen soll oder Gegnerisches
					time = millis();
					while (time < maxTime);		// warte 5 Sekunden
					if (JSON.getStopEnemy()){	// Gegner noch da? Umfahren! TODO: Zeit einstellen
						AvoidClash();
					}
				} else {
					time = millis();
					while (time < maxTimeWait);		// warte 10 Sekunden bevor weitergefahren wird
					DriveStraightForward();
				}
			} else
				// Weiter geradeaus fahren
				DriveStraightForward();
		}
		UpdateData();
	}
	// Drehen zum Arbeitspunkt, mˆglicherweise auslassen
	Position++;	// naechsten Punkt anfahren
}


void Navigation::setTargetAngle(float angle) {
	targetAngle = angle;
}

void Navigation::turnToTargetAngle() {
	e = targetAngle - Odo.getAngle();
	Moto.turn(signum(e) * speed);
	Serial.println(speed);
	Serial.println(e);
	if (abs(e) < 100){
		speed --;
		// speed wieder beim anfahren hochsetzen
		if (speed <= 0){
			Moto.stop();
		}
	}
}
void Navigation::driveToTargetPosition(int PositionX, int PositionY){
	e = LengthToPosition(PositionX, PositionY);
	if (speed > 0){
		Moto.driveStraight(speed);
	}
	if (e < 50){
		Moto.driveStraight(speed);
		speed --;
		if (speed <= 0){
			Moto.stop();
		}
	}

}
// Gibt Vorzeichen des ¸bergebenen Wertes zur¸ck
int Navigation::signum(float sign){
	int NumberSign = 0;
	if (sign > 0){
		NumberSign = 1;
	}
	if (sign < 0){
		NumberSign = -1;
	}
	return NumberSign;
}

void Navigation::TestDriveToPoint(){

	setTargetAngle(180);
	turnToTargetAngle();
	if( abs(e) <= 1){
		speed = 150;
		while (Odo.getY_position() < 1000){
			Odo.updateOdometrie();
			Moto.driveStraight(speed);
		}
		Serial.println(Odo.getY_position());
		Moto.stop();
		while(1);
	}

}
