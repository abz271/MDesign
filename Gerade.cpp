#include "Arduino.h"
#include "Gerade.h"
#include "Vec.h"

Gerade::Gerade() {

}

Gerade::Gerade(Vec o, Vec r) :
		o(o), r(r) {
	k = 0;
}

float Gerade::getIntersection(Gerade g) {
	float Intersection = 0;
	Intersection = (g.o.x * g.r.y + o.y * g.r.x - g.o.y * g.r.x - g.r.y * o.x)
			/ (g.r.y * r.x - r.y * g.r.x);

	return Intersection;
}

void Gerade::testSchnittpunkt() {
	Vec o(1500, 1000);					//
	Vec r(1, 0);							//		x->			^
	Gerade test(o, r);					//					I
	Gerade Vec1(Vec(0, 0), Vec(2000, 0));	//					x
	int a = 1;
	if (a == 1) {
		Serial.println(test.getIntersection(Vec1));
		a = 2;
	}
}
