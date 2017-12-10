
#include "Arduino.h"
#include "StateMachine.h"
#include "Gerade.h"

StateMachine StateM;
unsigned long lastTime = 0;
unsigned long Ts = 1/10;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	StateM.getNavigation().setStartParameters(250, 250, 90);
	lastTime = millis();
}


void loop() {
	unsigned long test1 = millis();
	//if (millis() - lastTime >= 5){
		StateM.UpdateData();
		//lastTime = millis();
		StateM.evalStateMachine();
	//}
	Serial.println( millis()- test1);
}
