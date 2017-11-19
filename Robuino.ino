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
	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	Navis.setTargetAngle(-90);

}

void loop() {
	//testOdo();
	//testdrive();
	// Navis.getMotor().turnLeft();
	// testTurnRight90();
	//Navis.TestDriveToPoint();
	Navis.UpdateData();		// Immer ganz oben, danach Verarbeitung, danach Ausgabe!
	//Navis.turnToTargetAngle();
	//Navis.driveToTargetPosition(0, 200);
	//Serial.println("Wert in X: ");
	//Serial.println(Navis.getOdometrie().getX_position());
	//Serial.println("Wert in Y: ");
	//Serial.println(Navis.getOdometrie().getY_position());

	//delay(3000);
	//testOdo();
	Navis.turnToTargetAngle();
}

void testOdo(){

	Navis.getOdometrie().testOdometrie();

}

void testdrive(){
	for (int i = 1; i < 2; i++){
		Navis.getMotor().driveStraight(150);
		while (Navis.getOdometrie().getY_position() <= Navis.ConsiderOffset(200)) {
			Navis.getOdometrie().updateOdometrie();
		}
		Navis.getMotor().stop();
		delay(2000);
		if (i == 1){
			Navis.getOdometrie().updateOdometrie();
			Serial.println("Punkt");
			Serial.println(i);
			Serial.println(Navis.getOdometrie().getY_position());
			while(1);
		}
		delay(2000);/*
		Navis.rotateLeft90();
		delay(2000);
		Navis.rotateLeft90();
		delay(2000);*/
	}
}


void testTurnRight90(){

	Serial.println(Navis.getOdometrie().getAngle());
	while (a == 1){
		Navis.rotateRight90();
		a = 2;
	}
	/*testOdo();

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
