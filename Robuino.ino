#include "Arduino.h"
#include "StateMachine.h"
#include "Gerade.h"

StateMachine StateM;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
/*
	Vec oha(200, 100);
	//Vec r(1, 1);
	Vec raw(90);
	Gerade test(oha, raw);
	Gerade Vec1(Vec(0, 0), Vec(1, 0));
	Gerade Vec2(Vec(0,2000), Vec(1,0));
	Gerade Vec3(Vec(0,0),Vec(0,1));
	Gerade Vec4(Vec(3000,0), Vec(0,1));


	float a = test.getIntersection(Vec1);
	float b = test.getIntersection(Vec2);
	float c = test.getIntersection(Vec3);
	float d = test.getIntersection(Vec4);

	Vec klaro = test.getDirectVec(c);
	Serial.println(klaro.getX());
	Serial.println(klaro.getY());
*/
}


void loop() {

	//StateM.UpdateData();
	StateM.evalStateMachine();
}
