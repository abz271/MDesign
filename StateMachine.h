#include "Arduino.h"
#include "Navigation.h"
#include "Vec.h"

#ifndef STATEMACHINE_H
#define	STATEMACHINE_H

class StateMachine {
private:
	Navigation Navi;
	int switchPin = 52;				// Einschalter

	// Fahrparameter
	int speedmaxturn = 100;
	int speedStartUp = 45;
	int speedmax = 255;
	int speedStop = 0;

	// Ausweichverhalten
    bool Master = true;
    unsigned long currentQuarter = 0;
    float actualAvoidAngle = 0;

    // Zeitverhalten
    unsigned long timeCur = millis();
    const unsigned long interval = 1000;

public:

	StateMachine();
	void evalStateMachine();
	void UpdateData();
	void applyOutputToProcess();

};


#endif	/* STATEMACHINE_H */


