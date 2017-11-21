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
	int speedStartUp = 45;
	int speedmax = 150;
	int speedStop = 0;
    bool Master = true;
    unsigned long currentQuarter = 0;

    unsigned long timeCur = millis();
    const unsigned long interval = 1000;

    float targetAngle = 0;


public:

	StateMachine();
	void evalStateMachine();
	void UpdateData();
	void applyOutputToProcess();

};


#endif	/* STATEMACHINE_H */


