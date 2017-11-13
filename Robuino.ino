#include "Arduino.h"
//The setup function is called once at startup of the sketch
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Navigation.h"

Odometrie odotest;

void setup()
{
// Add your initialization code here




	Serial.begin(9600);
		Serial.println("Hallo, Robuino");


}

// The loop function is called in an endless loop
void loop()
{

	odotest.testOdometrie();

}
