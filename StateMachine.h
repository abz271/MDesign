#include "Arduino.h"
#include "Navigation.h"
int a = 0;

#ifndef STATEMACHINE_H
#define	STATEMACHINE_H

class StateMachine {
private:
	Navigation Navi;
	int switchPin = 52;
	int speedmax = 150;
	int speedmaxturn = 100;
	int speedStop = 0;
	int safetyDistance = 10;

public:

	StateMachine();
	void evalStateMachine();
	void UpdateData();
	void applyOutputToProcess();



};


#endif	/* STATEMACHINE_H */


