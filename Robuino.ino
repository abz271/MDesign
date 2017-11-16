#include "Arduino.h"
//The setup function is called once at startup of the sketch
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Navigation.h"

Odometrie odo;
Motor moto;

void testOdo();
void testdrive();
void testTurnRight90();

void setup() {
// Add your initialization code here

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");

}

// The loop function is called in an endless loop
void loop() {

	// Ich schreib euch drei test Funktionen
	// ihr müste diese hier dann einfach aufrufen
	// z.B. testOdo();

	testOdo();

}

void testOdo(){

	odo.testOdometrie();

}

void testdrive(){

	moto.driveStraight();

	while(1){
		moto.updateVelocity();

		if(odo.getY_position() >= 200){
			moto.stop();
		}
	}


}

void testTurnRight90(){

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
