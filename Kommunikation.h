#ifndef KOMMUNIKATION_H
#define KOMMUNIKATION_H

#include <Arduino.h>

// TODO: Die Byte Längen sind alle noch auf mindestens 45 zu setzen
class Kommunikation {
private:

	int positionAddress = 1;
	int hardwareAddress = 4;

	int stringLength = 60;

	void DataFromHardware(String& comString);
	void DataToHardware(String comString);
	void DataFromPosition(String& comString);

public:

	Kommunikation();
	bool getSignalUsefull();
	bool getStopEnemy();
	void getPosition(float& xPos, float& yPos);
	void sendMotorPower(float leftWheelPower, float rightWheelPower);

	// Zum testen Objekt im Robi.cpp anlegen und dann
	// diese Funktion einfach im loop laufen lassen
	void testAsMaster();
	void testAsSlave();



};

#endif /* KOMMUNIKATION_H */

