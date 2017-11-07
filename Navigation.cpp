#include "Odometrie.h"
#include "Navigation.h"

#include <Arduino.h>

Navigation::Navigation() {
	Position = 0;
}

// Die aktuellen Positionsdaten werden mit den Werten aus der Odometrie und dem Positionsteam angepasst
void Navigation::UpdateData() {
	Odo.updateOdometrie();
	if (JSON.getSignalUsefull()) {
		JSON.getPosition(x_aktuell, y_aktuell);
		Odo.setPosition(x_aktuell, y_aktuell);
	} else {
		x_aktuell = Odo.getX_position();
		y_aktuell = Odo.getY_position();
	}

	//TODO: timer
}

// CalculateAngle gibt den um sich zu drehenden Winkel mit dem
// kÃ¼rzesten Drehwinkel an ->  auch negative Zahlen
// mit 90Â° Koordinatensystem dem des Fahrzeugs angepasst!
double Navigation::CalculateAngle(int x, int y) {

	double delta_x = x - x_aktuell;
	double delta_y = y - y_aktuell;
	double Winkel = atan2(delta_y, delta_x) * 180 / PI;

	// TODO: Quadranten korrektur möglicherweise nötig

	if ((Winkel < 180) && (Winkel > 0)) {

		Winkel = (int) (((Winkel * 100 + 0.5) / 100.0) - 90);
	} else {
		Winkel = (int) (((Winkel * 100 + 0.5) / 100.0) + 90);
	}
	return Winkel;
}

// Berechnet die VektorlÃ¤nge zwischen dem aktuellem Standort und dem Zielpunkt.
// TODO: Wird evtl nicht mehr gebraucht
// TODO: automatisch von nächsten Ziel ausrechnen, aber nicht wirklich nötig
double Navigation::weglaenge(int x, int y) {
	int delta_x = x - x_aktuell;
	int delta_y = y - y_aktuell;
	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

// Rotate sorgt fÃ¼r eine Drehung auf der Stelle um den Zielwinkel
void Navigation::Rotate() {

	if (CalculateAngle(X_Koordinaten[Position], Y_Koordinaten[Position]) < 0) {
		// Rechts rum drehen!!!
		Moto.sendMotorPower(forward, backward);
	} else {
		// Links rum drehen!!!
		Moto.sendMotorPower(backward, forward);
	}
}
// Einfache Fahrt gerade aus mit gleicher Beschleunigung auf beiden Motoren
void Navigation::DriveStraightForward() {
	TargetAngleNew = Odo.getAngle();	// Eingeschlagenen Winkel aktualisieren
	// Falls das Auto im Toleranzbereich des Ursprungswinkel fährt, einfach gerade aus fahren
	if ((TargetAngleNew <= (TargetAngleOld + angleTolerance)) || (TargetAngleNew >= (TargetAngleOld - angleTolerance))) {
		Moto.sendMotorPower(forward, forward);
	} else if (TargetAngleNew > (TargetAngleOld + angleTolerance)) {	// falls Drehwinkel aus Odometrie zu groß wird
		Moto.sendMotorPower((forward + driveOffset), forward);		// Fahrrichtung langsam korrigieren
	} else if (TargetAngleNew < (TargetAngleOld - angleTolerance)) {	// falls Drehwinkel aus Odometrie zu klein wird
		Moto.sendMotorPower(forward, (forward + driveOffset));		// Fahrrichtung langsam korrigieren
	}
}

void Navigation::StopDriving() {
	Moto.sendMotorPower(stopp, stopp);
}

void Navigation::AvoidClash() {
// TODO: Ausweichwinkel, Ausweichfahrt
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

	switch(currentState){
	case 1:
		// TODO: Ausweichverhalten für Quartal 1
		break;
	case 2:
		// TODO: Ausweichverhalten für Quartal 2
		break;
	case 3:
		// TODO: Ausweichverhalten für Quartal 3
		break;
	case 4:
		// TODO: Ausweichverhalten für Quartal 4
		break;
	}
}

// Programm beenden
bool Navigation::finished() {

}

bool Navigation::NotAtPoint() {
	return (x_aktuell != X_Koordinaten[Position])
			&& (y_aktuell != Y_Koordinaten[Position]);
}

void Navigation::drive() {

	// While solange wir fahren
	while (finished()) {
		// Ziel bestimmen
		TargetAngleOld = CalculateAngle(X_Koordinaten[Position],
				Y_Koordinaten[Position]);
		// Fahrzeug richtung Ziel drehen
		while (TargetAngleOld != Odo.getAngle()) {
			Rotate();
			UpdateData();
		}
		// Gerade zum Ziel Fahren
		while (NotAtPoint()) {
			// Gegner ausweichen?
			if (JSON.getStopEnemy()) {
				Moto.sendMotorPower(stopp, stopp);	// Daten An Motoren = 0;


				if (Master) { // Abfrage, ob eigenes Fahrzeug ausweichen soll oder Gegnerisches
					AvoidClash();
				} else {
					// Warte 10 Sekunden
					// TODO: Daleay nicht gut möglich: eher while mit timer delay(10000);
					DriveStraightForward();
				}
			} else
				// Weiter geradeaus fahren
				DriveStraightForward();
		}
		UpdateData();
	}
	// Drehen zum Arbeitspunkt, möglicherweise auslassen
	Position++;	// naechsten Punkt anfahren
}

