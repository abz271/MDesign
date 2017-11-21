#include "Arduino.h"
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Navigation.h"
#include "StateMachine.h"
StateMachine StateM;

void setup() {
	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
}

void loop() {
	StateM.UpdateData();
	StateM.evalStateMachine();
}
