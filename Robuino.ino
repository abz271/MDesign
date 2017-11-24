#include "Arduino.h"
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Navigation.h"
#include "StateMachine.h"
#include "Gerade.h"
//StateMachine StateM;
Gerade MaxA(Vec(8,2),Vec(-2,4)),MaxB(Vec(9,5),Vec(-4,4));
void setup() {
	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
}

void loop() {
	Serial.println(MaxA.getIntersection(MaxB));
	//StateM.UpdateData();
	//StateM.evalStateMachine();
}
