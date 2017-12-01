#include "Odometrie.h"
#include "Navigation.h"
#include "Motor.h"
#include <Arduino.h>


Navigation::Navigation(){
	Position = 0;
	Odo.setXposition(x_aktuell);
	Odo.setYposition(y_aktuell);
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

void Navigation::UpdateData() {
	Odo.updateOdometrie();
	Moto.updateVelocity();
	int xFromPosition, yFromPosition;
	if (JSON.getPosition(xFromPosition, yFromPosition) ) {
		x_aktuell = xFromPosition;
		y_aktuell = yFromPosition;
		Odo.setPosition(x_aktuell, y_aktuell);
	} else {
		x_aktuell = Odo.getX_position();
		y_aktuell = Odo.getY_position();
	}
	Odo.testOdometrie();
}

float Navigation::getCalculateAngle(int x, int y) {

	double delta_x = x - x_aktuell;
	double delta_y = y - y_aktuell;
	double Winkel = atan2(delta_y, delta_x) * 180 / PI;

	return Winkel;
}

// Berechnet die VektorlÃ¤nge zwischen dem aktuellem Standort und dem Zielpunkt.
// Wird evtl nicht mehr gebraucht
float Navigation::getLengthToPosition(int x, int y) {
	int delta_x = x - x_aktuell;
	int delta_y = y - y_aktuell;
	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

void Navigation::turnToTargetAngle() {
	e = targetAngle - Odo.getAngle();
	Moto.turn(signum(e) * speed);
	Serial.print("  targetAngle: ");
	Serial.print(targetAngle);
	Serial.print("  OdoAngle: ");
	Serial.print(Odo.getAngle());
	Serial.print(" speed: ");
	Serial.print(speed);
	Serial.print(" e: ");
	Serial.println(e);
	if (abs(e) < 50){
		//speed --;		//Originalversion
		speed = speed - 2;
		if (speed <= 0){
			Moto.stop();
		}
	}
}
void Navigation::driveToTargetPosition(){
	e = getLengthToPosition(X_Koordinaten[Position], Y_Koordinaten[Position]);
	// implementierung P-Regler
	controlDeviation = getTargetAngle();	// Soll Winkel
	actualDeviation = Odo.getAngle();		// Ist Winkel
	//Reglerdifferenz verstärken und übertragen
	differenceDeviation = (controlDeviation - actualDeviation) * amplifierKp;
	Moto.driveStraightRegulated(speed, differenceDeviation);
}

int Navigation::getTargetCoordinateX(){
	return X_Koordinaten[Position];
}

int Navigation::getTargetCoordinateY(){
	return Y_Koordinaten[Position];
}

float Navigation::getDeviation(){
	return e;
}

float Navigation::getSpeed(){
	return speed;
}
float Navigation::getTargetAngle(){
	float targetAngle = getCalculateAngle(X_Koordinaten[Position], Y_Koordinaten[Position]);
	return targetAngle;
}

int Navigation::getPosition(){
	return Position;
}

int Navigation::getY(){
	return y_aktuell;
}

int Navigation::getX(){
	return x_aktuell;
}

float Navigation::getSafetyRadius(){
	return safetyRadius;
}

int Navigation::getMaximalPosition(){
	return maxPosition;
}

void Navigation::setSpeed(int speed){
	this->speed = speed;
}

void Navigation::setTargetAngle(float angle) {
	if (angle >= 360.0){
		angle -= 360.0;
	}
	targetAngle = angle;
}

void Navigation::setNextPosition(){
	Position ++;
}

// Gibt Vorzeichen des übergebenen Wertes zurück
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


