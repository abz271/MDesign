#include "Statemachine.h"
enum states {
    initState, turnToTargetAngle, driveStraight, driveStraightRight, driveStraightLeft, avoidCrash
};
static enum states currentState  = initState;

StateMachine::StateMachine(){
   	pinMode(switchPin,INPUT);
}

void StateMachine::UpdateData(){
	Navi.UpdateData();
}

//TODO: resetOutputs(), überprüfen, ob funktionalität noch gegeben ist

void StateMachine::evalStateMachine() {
    switch (currentState) {
        case initState:
        	// zum Testen
        	Navi.getOdometrie().testOdometrie();
            break;
        case turnToTargetAngle:
            break;
        case driveStraight:
        	Navi.setSpeed(speedmax);
            break;

        case driveStraightLeft:
        	Navi.setSpeed(speedmax);
            break;
        case driveStraightRight:
        	Navi.setSpeed(speedmax);
            break;
        case avoidCrash:
        	Navi.setSpeed(speedStop);
            break;
    }
    //TODO: Taster einbauen lassen
    switch (currentState) {
        case initState:
        	if(switchPin){
        		currentState = turnToTargetAngle;
        		Navi.setSpeed(speedmax);
        	}
            break;
        case turnToTargetAngle:
        	if (Navi.getSpeed() == speedStop){
        		currentState = driveStraight;
        	}
            break;
        case driveStraight:
            break;

        case driveStraightRight:

            break;

        case driveStraightLeft:

            break;

        case avoidCrash:

            break;
    }
}
