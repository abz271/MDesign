
#ifndef NAVIGATION_H
#define NAVIGATION_H
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Motor.h"
//#include "StateMachine.h"


class Navigation {
private:
    Kommunikation JSON;
    Odometrie Odo;
    Motor Moto;
private:

    // Attribute für die Motoransteuerung
    float speed = 100;		// MotorPWM-Signal für die Vorwärtsfahrt eines Rades
    float targetAngle = 0;	// zum Aktualisieren des Eingeschlagenen Winkel
    // Attribute für die Positionsbestimmung
<<<<<<< HEAD
    int X_Koordinaten[5] = {300, 850, 2150, 2790, 300};			// Alpha Fahrzeug Koordinaten
    int Y_Koordinaten[5] = {1700, 1460, 1460, 200, 1700};		// Alpha Fahrzeug Koordinaten

    //int X_Koordinaten[5] = {2700, 2800, 2700, 1500, 710};		// Beta Fahrzeug Koordinaten
    //int Y_Koordinaten[5] = {1700, 1300, 200, 800, 200};		// Beta Fahrzeug Koordinaten
    int x_aktuell = 0;
    int y_aktuell = 0;
    int maxPosition = 4;		// Maximal 5 Positionen; Zählweise ab 0
=======
    int X_Koordinaten[5] = {0, 1000, 0, 0, 25};
    int Y_Koordinaten[5] = {1000, 1000, 0, 0, 9};
    float x_aktuell = 0;
    float y_aktuell = 0;
    int maxPosition = 2;	// Maximal 5 Positionen
>>>>>>> branch 'master' of https://github.com/TorbenMa/mdesign
    int Position;				// Aktueller Anfahrpunkt
    // Reglungswerkzeuge
	float controlDeviation = 0;		// Sollwert Geradeausfahrt (Drehwinkel)
	float actualDeviation = 0;		// Istwert Geradeausfahrt (Drehwinkel)
	float differenceDeviation = 0;	// Differenz Soll-Ist-Winkel Geradeausfahrt
	float amplifierKp = 10;			// Verstärkungsfaktor
	float safetyRadius = 30;		// Sicherheitsradius bei anfahren von Punkten
    float e = 0.0;					// Differenz Soll-Ist-Winkel
    float direction = 0;			// Kürzeste Richtung fürs drehen

public:	// Allgemeines
    Navigation();
    bool PositionInLava();
    void UpdateData();
    void turnToTargetAngle();
    void driveToTargetPosition();
    Odometrie& getOdometrie();
    Kommunikation& getJSON();
    Motor& getMotor();
private:	// Allgemeines
    int signum(float sign);
public: // Getter
    float getLengthToPosition(int x, int y);
    float getCalculateAngle(int x, int y);
    float getSafetyRadius();
    float getTargetAngle();
    float getDeviation();
    float getSpeed();
    int getTargetCoordinateX();
    int getTargetCoordinateY();
    int getCurrentQuarter();
    int getMaximalPosition();
    int getPosition();
    int getY();
    int getX();

public:	// Setter
    void setStartParameters(int x, int y, float angle);
    void setDirection(float odo, float ziel);
    void setTargetAngle(float angle);
    void setSpeed(int speed);
    void setPosition(int Position);
    void setNextPosition();


    void testAngle(float odo, float ziel);
};

#endif /* NAVIGATION_H */

