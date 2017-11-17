#include "Arduino.h"
//The setup function is called once at startup of the sketch
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Navigation.h"

Navigation Navis;
int a = 1;

void testOdo();
void testdrive();
void testTurnRight90();

void setup() {
// Add your initialization code here
	Serial.begin(9600);
	Serial.println("Hallo, Robuino");

	Navis.setTargetAngle(-180.0);
}

// The loop function is called in an endless loop
void loop() {
	// Ich schreib euch drei test Funktionen
	// ihr müste diese hier dann einfach aufrufen
	// z.B. testOdo();
	//testOdo();
	//testdrive();
	//Navis.getMotor().turnLeft();
	//testTurnRight90();
	Navis.turnToTargetAngle();
}

void testOdo(){

	Navis.getOdometrie().testOdometrie();

}

void testdrive(){

	for (int i = 1; i < 5; i++){
		Navis.getMotor().driveStraight();
		while (Navis.getOdometrie().getY_position() <= Navis.ConsiderOffset(i * 200)) {
			Navis.getOdometrie().updateOdometrie();
		}
		Serial.println("Punkt");
		Serial.println(i);
		Serial.println(Navis.getOdometrie().getY_position());
		Navis.getMotor().stop();
		delay(2000);
		Navis.rotateLeft90();
		delay(2000);
		Navis.rotateLeft90();
		delay(2000);
	}
}


void testTurnRight90(){

	Serial.println(Navis.getOdometrie().getAngle());
	while (a == 1){
		Navis.rotateRight90();
		a = 2;
	}
	testOdo();
/*
	while (a == 2){
		Navis.rotateLeft90();
		a = 1;
	}*/
}



/*		Also was muss alles getan werden:
 * 		- die Odometrie muss kalibriert werden
 * 			- Der Radumdurchmesser muss kalibriert werden
 * 			- Der Achsabstand muss kalibriert werden
 * 		- die Drehrichtungen der Motoren müssen eingestellt werden
 * 			- in driveStraight, turnLeft, turnRight und den gleichen mit vorgegebenen Werten
 * 			- im Moment steht dort nur Mist drin, da müsst ihr austesten was richtig ist
 * 			- tim sollte wissen was zu tun ist
 * 		- der Anhalteweg muss eingestellt werden
 * 			- dieser ist Abhängig von der Maximalen Geschwindigkeit die vorher eingestellt wurde
 * 			- nur wenn dieser genau eingestellt ist, kann auch genau angehalten werden
 * 		- Die Berechnungen von Tim sind noch "falsch", am besten stellt ihr alle so ein, damit
 * 		  es mit dem normalen Koordinatensystem überein stimmt
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
