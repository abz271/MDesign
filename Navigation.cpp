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
		// Links rum drehen???
		JSON.sendMotorPower(forward, backward);
	} else {
		// Rechts rum drehen???
		JSON.sendMotorPower(backward, forward);
	}
}
// Einfache Fahrt gerade aus mit gleicher Beschleunigung auf beiden Motoren
void Navigation::DriveStraightForward() {

	// TODO: zick zack korrektur
	JSON.sendMotorPower(forward, forward);
}

void Navigation::StopDriving() {
	JSON.sendMotorPower(stopp, stopp);
}

void Navigation::AvoidClash() {
// TODO: Ausweichwinkel, Ausweichfahrt,
}

// Programm beeden
bool Navigation::finished(){


}

bool Navigation::atPoint(){

	return (x_aktuell != X_Koordinaten[Position]) && (y_aktuell != Y_Koordinaten[Position]);

}


void Navigation::drive() {


	// While solange wir fahren
	while (finished() ) {


		// Ziel bestimmen
		TargetAngle = CalculateAngle(X_Koordinaten[Position], Y_Koordinaten[Position]);


		// Fahrzeug richtung Ziel drehen
		while (TargetAngle != Odo.getAngle()) {

			Rotate();

			UpdateData();


		}


		// Gerade zum Ziel Fahren
		while(atPoint() ){

			// Gegner ausweichen?
			if (JSON.getStopEnemy()) {
				// Daten An Motoren = 0;

				if (Master) {// Abfrage, ob eigenes Fahrzeug ausweichen soll oder Gegnerisches
					AvoidClash();
				} else {
					// Warte 10 Sekunden
					// TODO: Daleay nicht gut möglich: eher while mit timer delay(10000);
					DriveStraightForward();
				}
			} else {

				// Weiter geradeaus fahren
				DriveStraightForward();
			}

			UpdateData();

		}

		// Drehen zum Arbeitspunkt, möglicherweise auslassen


		Position++;	// naechsten Punkt anfahren
	}
}

