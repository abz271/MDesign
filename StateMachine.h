#include "Arduino.h"
#include "Navigation.h"

#ifndef STATEMACHINE_H
#define	STATEMACHINE_H

class StateMachine {
private:
	Navigation Navi;
	int safetyDistance = 5;
	int speedmaxturn = 100;
	int maxPosition = 1;
	int switchPin = 52;
	int speedmax = 150;
	int speedStop = 0;
    bool Master = true;

    float targetAngle = 0;


public:

	StateMachine();
	void evalStateMachine();
	void UpdateData();
	void applyOutputToProcess();

};


#endif	/* STATEMACHINE_H */


