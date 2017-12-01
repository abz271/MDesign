#include "Arduino.h"
#include "Navigation.h"

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
	float actualAvoidAngle = 0;
	float a = 0; float b = 0; float c = 0; float d = 0;

	// Zeitverhalten
	unsigned long timeCur = millis();
	unsigned long playTime = millis();
	const unsigned long interval = 1000;
	const unsigned long intervalStop = 5000;
	const unsigned long intervalDrive = 1000;
	const unsigned long intervalPlaytime = 25000;

public:

	StateMachine();
	void evalStateMachine();
	void UpdateData();
	void applyOutputToProcess();

	Navigation& StateMachine::getNavigation();

};

#endif	/* STATEMACHINE_H */

