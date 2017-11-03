#ifndef KOMMUNIKATION_H
#define KOMMUNIKATION_H

#include <Arduino.h>

// TODO: brauchen wir sone structs überhaupt, diese Klasse dient ja nur der Kommunikation
// und nicht dem speichern von werten
struct JSON_Hardware {
	unsigned int LinkesRad;
	unsigned int RechtesRad;
};

struct JSON_Position {
	unsigned int PositionX;
	unsigned int PositionY;
	unsigned int Zeit;
};

//struct JSON_Hardware;
//struct JSON_Position;

// TODO: Die Byte Längen sind alle noch auf mindestens 45 zu setzen
class Kommunikation {
private:

	bool SignalOk = false;
	unsigned int x_position = 0;
	unsigned int y_position = 0;

	int positionAddress = 1;
	int hardwareAddress = 4;

	int stringLength = 60;

	void DataFromHardware(String& comString);
	void DataToHardware(String comString);
	void DataFromPosition(String& comString);

public:

	Kommunikation();

	bool getStopEnemy();
	void getPosition(float& xPos, float& yPos);
	void sendMotorPower(float leftWheelPower, float rightWheelPower);

	// Zum testen Objekt im Robi.cpp anlegen und dann
	// diese Funktion einfach im loop laufen lassen
	void testAsMaster();
	void testAsSlave();

	// von Lennart angelegt wurden
	void DatenVonHardware(bool);
	void DatenAnHardware(int linkesRad, int rechtesRad, bool notHalt);
	void DatenVonPosition();
	bool getNotHalt();
	bool getSignalOk();
	int getX_position();
	int getY_position();
};

#endif /* KOMMUNIKATION_H */

