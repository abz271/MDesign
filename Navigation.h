
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
    int X_Koordinaten[5] = {0, 1000, 0, 0, 25};
    int Y_Koordinaten[5] = {1000, 1000, 0, 0, 9};
    float x_aktuell = 0;
    float y_aktuell = 0;
    int maxPosition = 2;	// Maximal 5 Positionen
    int Position;				// Aktueller Anfahrpunkt
    // Reglungswerkzeuge
	float controlDeviation = 0;
	float actualDeviation = 0;
	float differenceDeviation = 0;
	float amplifierKp = 10;
	float safetyRadius = 30;
    float e = 0.0;

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
    void setTargetAngle(float angle);
    void setSpeed(int speed);
    void setNextPosition();


private:
    void AvoidCrash();				// kommt in die State

};

#endif /* NAVIGATION_H */

