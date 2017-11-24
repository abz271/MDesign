#include "Gerade.h"
#include "Vec.h"


Gerade::Gerade(){

}

Gerade::Gerade(Vec o, Vec r):
	o(o),
	r(r) {
k = 0;
}

float Gerade::getIntersection(Gerade g){
	float Intersection = 0;
	Intersection = (g.o.x*g.r.y+o.y*g.r.x-g.o.y*g.r.x-g.r.y*o.x)/(g.r.y*r.x-r.y*g.r.x);

	return Intersection;
}


