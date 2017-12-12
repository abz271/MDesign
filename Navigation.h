
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
    bool StateStartUp = false;

    // Attribute für die Motoransteuerung
    float speed = 100;		// MotorPWM-Signal für die Vorwärtsfahrt eines Rades
    float targetAngle = 0;	// zum Aktualisieren des Eingeschlagenen Winkel
    // Attribute für die Positionsbestimmung
    int X_Koordinaten[5] = {300, 850, 2150, 2790, 300};			// Alpha Fahrzeug Koordinaten
    int Y_Koordinaten[5] = {1700, 1460, 1460, 200, 1700};		// Alpha Fahrzeug Koordinaten

    //int X_Koordinaten[6] = {2700, 2800, 2700, 1500, 710, 2700};		// Beta Fahrzeug Koordinaten
    //int Y_Koordinaten[6] = {1700, 1300, 200, 800, 300, 300};		// Beta Fahrzeug Koordinaten
    int x_aktuell = 0;
    int y_aktuell = 0;
    int maxPosition = 4;		// Maximal 5 Positionen; Zählweise ab 0
    int Position;				// Aktueller Anfahrpunkt

    int x_PositionteamOld = 0;
    int x_PositionteamNew = 0;
    int y_PositionteamOld = 0;
    int y_PositionteamNew = 0;

    bool takePosition = false;

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
    bool DetectedEnemyInArea();
    bool CrashIncoming();
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
    void setPrePositionteams(bool StateStartup);
    void setDirection(float odo, float ziel);
    void setPositionteam(bool Calculate);
    void setTargetAngle(float angle);
    void setPosition(int Position);
    void setSpeed(int speed);
    void setNextPosition();


    void testAngle(float odo, float ziel);
};

#endif /* NAVIGATION_H */

