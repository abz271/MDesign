#include "Arduino.h"
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Navigation.h"
#include "StateMachine.h"
StateMachine StateM;
Navigation Navis;	// ist nur ein Dummy

void setup() {
	Serial.begin(9600);
	Serial.println("Hallo, Robuino");

}

void loop() {
	//StateM.UpdateData();
	//StateM.evalStateMachine();
	Navis.getOdometrie().testOdometrie();
	float a = 90;
	float b = Navis.getOdometrie().getAngle();
	float c = 0.5;
	float d = (a-b)*c;
	Serial.println(a);
	Serial.println(b);
	Serial.println(d);
	//testdrive();


}

void testdrive(){
	for (int i = 1; i < 2; i++){
		Navis.getMotor().updateVelocity();
		float a = Navis.getCalculateAngle(0, 2000);
		float b = Navis.getOdometrie().getAngle();
		float c = 0.5;
		float d = 0;
		while (Navis.getyPosition() <= 2000){
			a = Navis.getCalculateAngle(0, 2000);
			b = Navis.getOdometrie().getAngle();
			d = (a-b)*c;
			Navis.getMotor().driveStraightRegulated(150, d);
			Navis.UpdateData();
			Navis.getOdometrie().updateOdometrie();
		}
		Navis.getMotor().stop();
		delay(2000);
		if (i == 1){
			Navis.getOdometrie().updateOdometrie();
			Serial.println("Punkt");
			Serial.println(i);
			Serial.println(Navis.getOdometrie().getY_position());
			while(1){
				Navis.getOdometrie().testOdometrie();
			}
		}
		delay(2000);
	}
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
