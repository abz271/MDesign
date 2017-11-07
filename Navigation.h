
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Kommunikation.h"
#include "Odometrie.h"

class Navigation {
private:
    Kommunikation JSON;
    Odometrie Odo;
private:

    // Attribute für die Motoransteuerung
    int stopp = 0;
    float forward = 1;
    float backward = 1;
    float TargetAngle = 0;

    // Attribute für die Positionsbestimmung
    int X_Koordinaten[5] = {0, 10, 15, 20, 25};
    int Y_Koordinaten[5] = {100, 25, 10, 5, 9};
    float x_aktuell = 0; // Startwert in x-Richtung
    float y_aktuell = 0; // Startwert in y-Richtung
    float angle_aktuell = 0; // Startwert Drehwinkel
    int Position = 0;	// Aktueller Anfahrpunkt
    int MaxPositions = 5;	// Maximal anfahrbare Punkt


    // Attribute für das Ausweichverhalten
    bool Master = true;
    int Spielfeldbreite = 3000;
    int Spielfeldhoehe = 2000;
    int autobreite = 200;
    float AvoidAngle = 0;

    
    // Änderungen von Torben
    float angleToleranz = 0.4;			// Winkelabweichung beim fahren

private:

    Navigation();



    double weglaenge(int x, int y);	// wird evtl nicht mehr gebraucht, erstmal dabei gelassen
    double CalculateAngle(int x, int y);

    void UpdateData();
    void drive();
    void DriveStraightForward();
    void StopDriving();
    void Rotate();
    void AvoidClash();
    void nextPosition();

    bool finished();
    bool atPoint();

};

#endif /* NAVIGATION_H */

