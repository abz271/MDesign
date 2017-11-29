#include "Arduino.h"
#include "StateMachine.h"
#include "Gerade.h"

StateMachine StateM;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	/*
	Vec o(100, 100);
	//Vec r(-1, -1);
	Vec r(90);
	Gerade test(o, r);
	Gerade Vec1(Vec(0, 0), Vec(1, 0));
	Gerade Vec2(Vec(0,2000), Vec(1,0));
	Gerade Vec3(Vec(0,0),Vec(0,1));
	Gerade Vec4(Vec(3000,0), Vec(0,1));


	Serial.println(Vec1.getIntersection(test));
	Serial.println(Vec2.getIntersection(test));
	Serial.println(Vec3.getIntersection(test));
	Serial.println(Vec4.getIntersection(test));
	*/
}


void loop() {

	//StateM.UpdateData();
	StateM.evalStateMachine();
}
