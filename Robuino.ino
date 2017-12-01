#include "Arduino.h"
#include "StateMachine.h"
#include "Gerade.h"

StateMachine StateM;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	StateM.getNavigation().setXaktuell(1501);
	StateM.getNavigation().setYaktuell(100);
}


void loop() {

	StateM.UpdateData();
	StateM.evalStateMachine();
}
