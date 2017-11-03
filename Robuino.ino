#include "Arduino.h"
//The setup function is called once at startup of the sketch
#include "Kommunikation.h"
#include "Odometrie.h"

Odometrie odotest;
Kommunikation komTest;

void setup()
{
// Add your initialization code here

	Serial.begin(9600);
		Serial.println("hallo, geht endlich");

		pinMode(13, OUTPUT);

		float arr1[] = {-30, -25, 20, -20};
		float arr2[] = {30, 25, 20, 20};
		int size = 3;

		odotest.testBerechnung(arr1, arr2, size);
		delay(1000);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
}
