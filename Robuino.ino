#include "Arduino.h"
#include "StateMachine.h"
#include "Gerade.h"

StateMachine StateM;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
}


void loop() {

	StateM.UpdateData();
	StateM.evalStateMachine();
}
