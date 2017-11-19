
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Kommunikation.h"
#include "Odometrie.h"
#include "Motor.h"

class Navigation {
private:
    Kommunikation JSON;
    Odometrie Odo;
    Motor Moto;
private:

    // Attribute für die Motoransteuerung
    unsigned int maxTime = 5000; // Maximal 5s
    unsigned int maxTimeWait = 10000; // maximal 10s warten
    float speed = 150;		// MotorPWM-Signal für die Vorwärtsfahrt eines Rades
    float speedturn = 50;
    char driveOffset = 1;		// Offset-Signal zum Gegensteuern der Geschwindigkeit bei (ZickZack-Kurs)
    float ActualTargetAngle = 0;	// aktueller Ziel-Winkel von aktueller Position zu Zielpunkt
    float TargetAngleNew = 0;	// zum Aktualisieren des Eingeschlagenen Winkel
    int CoordinateOffset = 35;	// Offset für den Winkel (in eine Richtung)
    // Attribute für die Positionsbestimmung
    int X_Koordinaten[5] = {0, 10, 15, 20, 25};
    int Y_Koordinaten[5] = {100, 25, 10, 5, 9};
    int x_aktuell = 0; 		// Startwert in x-Richtung
    int y_aktuell = 0; 		// Startwert in y-Richtung
    int Position;				// Aktueller Anfahrpunkt
    int MaxPositions = 5;		// Maximal anfahrbare Punkt


    // Attribute für das Ausweichverhalten
    bool Master = true;
    int Spielfeldbreite = 2000;
    int Spielfeldhoehe = 3000;

    float angleTolerance = 0.4;			// Winkelabweichung beim fahren
    float targetAngle = 0.0;

    float e = 0.0;
public:

    Navigation();
    int ConsiderOffset(int Coordinate);
    int GetYaktuell();
    int getSpeed();
    void rotateRight90();
    void rotateLeft90();
    void TestDriveToPoint();
    Odometrie& getOdometrie();
    Motor& getMotor();
    Kommunikation& getJSON();

    void turnToTargetAngle();
    void driveToTargetPosition(int PositionX, int PositionY);
    void setTargetAngle(float angle);
    void UpdateData();
private:

    double LengthToPosition(int x, int y);	// wird evtl nicht mehr gebraucht, erstmal dabei gelassen
    double CalculateAngle(int x, int y);



    void drive();
    void DriveStraightForward();
    void StopDriving();
    void AvoidClash();
    void nextPosition();


    int signum(float sign);
    bool finished();
    bool NotAtPoint();



};

#endif /* NAVIGATION_H */

