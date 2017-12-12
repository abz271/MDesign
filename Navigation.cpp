#include "Odometrie.h"
#include "Navigation.h"
#include "Motor.h"
#include <Arduino.h>


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
	float xFromPosition, yFromPosition;

	// Rohe Positionsdaten
	float xPosiZumPlotten, yPosiZumPlotten;		// nachher wieder weg
	int WertOk = JSON.getPosition(xPosiZumPlotten, yPosiZumPlotten);
	JSON.getPosition(xPosiZumPlotten, yPosiZumPlotten);

	// Positionsdaten
	float xZusammenPlotten, yZusammenPlotten; // nachher weg

	// bei start up schon erlauben, dass Daten überschrieben werden, aber nicht gesetzt werden => Odometrie fahren
	// Grund: Keine Ausreißer beim Fahren 0,0 auf x,x => Falscher Winkel
	if (StateStartUp){
		x_PositionteamOld = int (xFromPosition);
		y_PositionteamOld = int (yFromPosition);
		x_PositionteamNew = int (xFromPosition);
		y_PositionteamNew = int (yFromPosition);
	}

	// Daten von Positionteam sind gut
	if (JSON.getPosition(xFromPosition, yFromPosition)){

		// Daten von Positionteam sind die gleichen? => nur 5 Hz Taktung
		if((x_PositionteamOld == x_PositionteamNew) && (y_PositionteamOld == y_PositionteamNew)) {
			xZusammenPlotten = Odo.getX_position();
			yZusammenPlotten = Odo.getY_position();

			x_aktuell = Odo.getX_position();
			y_aktuell = Odo.getY_position();
		}else{
			xZusammenPlotten = int(xFromPosition);
			yZusammenPlotten = int(yFromPosition);

			x_aktuell = int(xFromPosition);
			y_aktuell = int(yFromPosition);
			Odo.setPosition(x_aktuell, y_aktuell);
			if ((x_PositionteamOld != x_PositionteamNew) && (y_PositionteamOld != y_PositionteamNew)){
				int deltaX = x_PositionteamNew - x_PositionteamOld;
				int deltaY = y_PositionteamNew - y_PositionteamOld;
				float angle = atan2(deltaY, deltaX) * 180/PI;	// evtl ( 0 , 0 ) abfangen
				Odo.setAngle(angle);
			}
		}
		// Falls beide Winkel unterschiedlich sind, alten Winkel zuweisen
		if ((x_PositionteamNew != x_PositionteamOld) && (y_PositionteamNew != y_PositionteamOld)){
			x_PositionteamOld = x_PositionteamNew;
			y_PositionteamOld = x_PositionteamNew;
		}

		// aktualisieren des neuen Winkels
		x_PositionteamNew = int(xFromPosition);
		y_PositionteamNew = int(yFromPosition);



		// Daten von Positionteam sind schlecht
	} else {
		x_aktuell = Odo.getX_position();
		y_aktuell = Odo.getY_position();
		x_PositionteamOld = x_PositionteamNew;
		y_PositionteamOld = y_PositionteamNew;
	}
	Odo.testOdometrie();

	Serial.println(Odo.getX_position());
	Serial.println(" ");
	Serial.println(Odo.getY_position());
	Serial.println(" ");
	Serial.println(xPosiZumPlotten);
	Serial.println(" ");
	Serial.println(yPosiZumPlotten);
	Serial.println(" ");
	Serial.println(WertOk);
	Serial.println(" ");
	Serial.println(xZusammenPlotten);
	Serial.println(" ");
	Serial.println(yZusammenPlotten);
	Serial.println(" ");

}

float Navigation::getCalculateAngle(int x, int y) {

	double delta_x = x - x_aktuell;
	double delta_y = y - y_aktuell;
	double Winkel = atan2(delta_y, delta_x) * 180 / PI;

	return Winkel;
}
// TODO: Evtl nicht mehr genutzt
bool Navigation::PositionInLava(){
	bool Lava = false;
	int Lavabereich = 300;
	if (((x_aktuell >= 0) && (x_aktuell <= Lavabereich)) ){
		if ((y_aktuell >= 0) && (y_aktuell <= 2000)){
			Lava = true;
		}
	}
	if (((x_aktuell >= 3000-Lavabereich) && (x_aktuell <= 3000))){
		if ((y_aktuell >= 0) && (y_aktuell <= 2000)){
			Lava = true;
		}
	}
	if((x_aktuell >= Lavabereich) && (x_aktuell <= 3000-Lavabereich)){
		if ((y_aktuell >= 2000-Lavabereich) && (y_aktuell <= 2000)){
			Lava = true;
		}
	}
	if ((x_aktuell >= Lavabereich) && (x_aktuell <= 3000-Lavabereich)){
		if ((y_aktuell >= 0) && (y_aktuell <= Lavabereich)){
			Lava = true;
		}
	}

	return Lava;
}
// Berechnet die VektorlÃƒÂ¤nge zwischen dem aktuellem Standort und dem Zielpunkt.
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
		speed = speed - 5;
		if (speed <= 0){
			Moto.stop();
		}
	}
}

void Navigation::driveToTargetPosition(){
	e = getLengthToPosition(X_Koordinaten[Position], Y_Koordinaten[Position]);
	// implementierung P-Regler
	Serial.println(e);
	controlDeviation = getTargetAngle();	// Soll Winkel
	actualDeviation = Odo.getAngle();		// Ist Winkel
	//Reglerdifferenz verstÃ¤rken und Ã¼bertragen
	differenceDeviation = (controlDeviation - actualDeviation) * amplifierKp;
	Moto.driveStraightRegulated(speed, differenceDeviation);
}

bool Navigation::DetectedEnemyInArea(){
	int xPositionObject = 0;
	int yPositionObject = 0;
	bool result = false;
	int radius = 270; 		// Radius des Fahrzeugs 150 + 100 Messentfernung + 20 Offset
	xPositionObject = cos(Odo.getAngle()* PI/180)*radius + x_aktuell;
	yPositionObject = sin(Odo.getAngle()* PI/180)*radius + y_aktuell;

	if (((xPositionObject > 0) && (xPositionObject < 3000)) && ((yPositionObject > 0) && (yPositionObject < 2000))){
		result = true;
	}
	Serial.print("x:  ");
	Serial.println(xPositionObject);
	Serial.print("y:  ");
	Serial.println(yPositionObject);
	Serial.print("Ergebnis: Objekt im Gebiet:  ");
	Serial.println(result);
	return result;
}
bool Navigation::CrashIncoming(){
	bool TooClose = false;
	int distance = 25;

	Gerade G1(Vec(0, 0), Vec(1, 0));		// G1 und G2 parallel x-Achse
	Gerade G2(Vec(0, 2000), Vec(1, 0));
	Gerade G3(Vec(0, 0), Vec(0, 1));		// G3 und G4 parallel y-Achse
	Gerade G4(Vec(3000, 0), Vec(0, 1));

	// Vektor des Autos anlegen mit gedrehten Richtungsvektor um 90°
	Vec o(x_aktuell, y_aktuell);
	Vec r(Odo.getAngle()-90);
	// Gerade des Autos erzeugen
	Gerade Intersection(o, r);

	//gedrehter Richtungsvektor: Prüfen ob, Schnittpunkt mit Spielfeldvektoren existieren
	float a = Intersection.getIntersection(G1);
	float b = Intersection.getIntersection(G2);
	float c = Intersection.getIntersection(G3);
	float d = Intersection.getIntersection(G4);

	if (((a >= 0) && (a < distance)) || ((b >= 0) && (b < distance)) || ((c >= 0) && (c < distance)) || ((d >= 0) && (d < distance))){
		TooClose = true;
	}
	return TooClose;
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

void Navigation::setStateInStartUp(bool StateStartUp){
	this->StateStartUp = StateStartUp;
}

void Navigation::setPosition(int Position){
	this->Position = Position;
}
void Navigation::setStartParameters(int x, int y, float angle){
	Odo.setXposition(x);
	Odo.setYposition(y);
	Odo.setAngle(angle);
}

// Gibt Vorzeichen des Ã¼bergebenen Wertes zurÃ¼ck
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


