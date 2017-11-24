#ifndef GERADE_H
#define GERADE_H

#include "Vec.h"

class Gerade {
public:
	Vec o;
	Vec r;
	float k;
public:
	Gerade();
	Gerade(Vec o, Vec r);
	float getIntersection(Gerade g);

};

#endif /* GERADE_H */
