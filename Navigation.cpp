#include "Odometrie.h"
#include "Navigation.h"
#include "Motor.h"
#include <Arduino.h>
// Allgemeine Todos
// TODO: Timer für das automatischen Beenden der Fahrt
// TODO: Beacons nicht detektieren.
// TODO:
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
int Navigation::getCurrentQuarter(){
	// TODO: Ausweichverhalten implementieren in Navigation
	if (x_aktuell < AreaWidth/2){
		if (y_aktuell < AreaHigh/2){
			currentQuarter = 1;	// Aktueller Aufenthalt in Quartal 1: x=0, y=0 bis x=999, y = 1499
		}else{
			currentQuarter = 2;	// Aktueller Aufenthalt in Quartal 2: x=0, y=1500 bis x=999, y = 3000
		}
	}else{
		if (y_aktuell < AreaHigh/2){
			currentQuarter = 3;	// Aktueller Aufenthalt in Quartal 3: x=1000, y=0 bis x=2000, y = 1499
		}else{
			currentQuarter = 4;	// Aktueller Aufenthalt in Quartal 4: x=1000, y=1500 bis x=2000, y = 3000
		}
	}
	return currentQuarter;
}
void Navigation::setSpeed(int speed){
	this->speed = speed;
}

void Navigation::setTargetAngle(float angle) {
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


