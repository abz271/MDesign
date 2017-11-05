#include <stdio.h>

#include "Odometrie.h"
#include "Navigation.h"

#include <stdlib.h>

Navigation::Navigation() {
	Position = 0;
}

// Die aktuellen Positionsdaten werden mit den Werten aus der Odometrie und dem Positionsteam angepasst
void Navigation::UpdateData() {
	Odo.updateOdometrie();
	if (JSON.getSignalUsefull()) {
		JSON.getPosition(x_aktuell, y_aktuell);
		Odo.setPosition();
	} else {
		x_aktuell = Odo.getX_position();
		y_aktuell = Odo.getY_position();
	}
}

double Navigation::CalculateAngle(int x, int y) {

	// CalculateAngle gibt den um sich zu drehenden Winkel mit dem
	// kürzesten Drehwinkel an ->  auch negative Zahlen
	// mit 90° Koordinatensystem dem des Fahrzeugs angepasst!
	double delta_x = x - x_aktuell;
	double delta_y = y - y_aktuell;
	double Winkel = atan2(delta_y, delta_x) * 180 / PI;
	if ((Winkel < 180) && (Winkel > 0)) {

		Winkel = (int) (((Winkel * 100 + 0.5) / 100.0) - 90);
	} else {
		Winkel = (int) (((Winkel * 100 + 0.5) / 100.0) + 90);
	}
	return Winkel;
}

// Berechnet die Vektorlänge zwischen dem aktuellem Standort und dem Zielpunkt. Wird evtl nicht mehr gebraucht
double Navigation::weglaenge(int x, int y) {
	int delta_x = x - x_aktuell;
	int delta_y = y - y_aktuell;
	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}
// Rotate sorgt für eine Drehung auf der Stelle um den Zielwinkel
void Navigation::Rotate() {

	if (CalculateAngle(X_Koordinaten[Position], Y_Koordinaten[Position]) < 0) {
		JSON.sendMotorPower(forward, backward);
	} else {
		JSON.sendMotorPower(backward, forward);
	}
}
// Einfache fahrt gerade aus mit gleicher Beschleunigung auf beiden Motoren
void Navigation::DriveStraightForward() {
	JSON.sendMotorPower(forward, forward);
}

void Navigation::StopDriving() {
	JSON.sendMotorPower(stopp, stopp);
}

void Navigation::AvoidClash() {
// TODO: Ausweichwinkel, Ausweichfahrt,
}

void Navigation::StartDriving() {
	// Solange der aktuelle Position nicht der Zielpunkt ist...
	while ((x_aktuell != X_Koordinaten[Position])
			&& (y_aktuell != Y_Koordinaten[Position])) {
		TargetAngle = CalculateAngle(X_Koordinaten[Position],
				Y_Koordinaten[Position]);
		// Ist der Zielwinkel ungleich des Drehwinkels des Fahrzeuges?
		while (TargetAngle != Odo.getAngle()) {
			Rotate();
			// Falls während der Drehung ein Fahrzeug entdeckt wird
			// Ausweichalgorithmus einleiten, danach Positionsdaten aktualisieren
			if (JSON.getStopEnemy()) {
				AvoidClash();
				UpdateData();
				TargetAngle = CalculateAngle(X_Koordinaten[Position],
						Y_Koordinaten[Position]);
			}
		}
		// Gegnerfahrzeug nach der Drehung detektiert?
		if (JSON.getStopEnemy()) {
			// Daten An Motoren = 0;

			if (Master) {// Abfrage, ob eigenes Fahrzeug ausweichen soll oder Gegnerisches
				AvoidClash();
			} else {
				// Warte 10 Sekunden
				delay(10000);
				DriveStraightForward();
			}
		} else {
			DriveStraightForward();
		}
		delay(10);	// 10 ms Verzögerung vor neuer Positionsbestimmung
		UpdateData();	// Aktuelle Position erfahren
	}
	Position++;	// naechsten Punkt anfahren
}

