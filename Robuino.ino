#include "Arduino.h"
#include "StateMachine.h"
#include "Gerade.h"

StateMachine StateM;

void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	StateM.getNavigation().setStartParameters(250, 250, 90);
}


void loop() {
	StateM.UpdateData();
	StateM.evalStateMachine();
}
