#include "Arduino.h"
#include "StateMachine.h"
//#include "Gerade.h
#include "Kommunikation.h"

Kommunikation komi;

StateMachine StateM;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
}


void loop() {

	komi.testKommunikation();
	//StateM.UpdateData();
	//StateM.evalStateMachine();
}
