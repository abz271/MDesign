
#include "Kommunikation.h"

#include <Wire.h>
#include <ArduinoJson.h>
#include <Arduino.h>

/*
 * Konstruktor zum initialisieren der Verbindung
 * Dieser Arduino dient als Master
 */
Kommunikation::Kommunikation() {

	// Verbindung als Master aufbauen
	Wire.begin();

}

/*
 * Fragt die Daten Ã¼ber den i^2c Bus von der Hardware Gruppe an
 * und gibt sie per Reference zurÃ¼ck
 * i^2c Bus Methode
 */
void Kommunikation::DataFromHardware(String& comString) {
	// Kommunikation starten, Daten anfragen

	Wire.requestFrom(hardwareAddress, stringLength);

	// Char array anlegen
	char c = 0;

	// Daten einlesen
	while (Wire.available()) {

		// Chars nacheinander empfangen und in ein char array schreiben
		c = Wire.read();
		comString += c;
	}
}

/*
 * Fragt die Daten über den i2c Bus von der Positions Gruppe an
 * i2c Methode
 */
void Kommunikation::DataFromPosition(byte* comString) {
	// Kommunikation starten, Daten anfragen

	Wire.requestFrom(positionAddress, 6);

	int counter = 0;
	// Daten einlesen
	while (Wire.available()) {
		comString[counter] = Wire.read();
		counter++;
	}

}
/*
 * Gibt die aktuelle Position der Positionsgruppe per Referenze zurück
 * Außerdem wird ein bool zurück gegeben ob die Information verwendbar ist oder nicht
 */
bool Kommunikation::getPosition(float& xPos, float& yPos, float& angle) {
	unsigned long message = 0;
	unsigned long tmp = 0;
	unsigned short uangle = 0;

	byte comString[6];

	DataFromPosition(comString);
// Bitverkn�pfung, um Daten von Positionsteam zu rekonstruieren
	tmp = comString[0];
	message |= (tmp << 24) & 0xFF000000;
	tmp = comString[1];
	message |= (tmp << 16) & 0x00FF0000;
	tmp = comString[2];
	message |= (tmp <<  8) & 0x0000FF00;
	tmp = comString[3];
	message |= (tmp & 0x000000FF);

	tmp = comString[4];
	uangle |= (tmp << 8) & 0xFF00;
	tmp = comString[5];
	uangle |= (tmp & 0x00FF);

	xPos = ((message & 0xFFFE0000) >> 17) / 10.0;
	yPos = ((message & 0x0001FFFC) >>  2) / 10.0;

	angle = (uangle / 100.0) - 180.0;

	return message & 0x03;
}

/*
 * Gibt einen bool zurÃ¼ck wenn sich ein Gegner in Reichweise der Sensoren befindet
 * Diese Methode kÃ¼mmert sich um das Ãœbersetzen des JSONs
 */
bool Kommunikation::getStopEnemy() {

	// Buffer fÃ¼r den Json String
	StaticJsonBuffer<100> jsonBuffer;

	// Variablen
	bool result = false;
	String comString;

	// Daten von dem Hardware Team abfragen
	DataFromHardware(comString);

	// Einen Json String erstellen
	JsonObject& root = jsonBuffer.parseObject(comString);

	//Daten in den Json String schreiben
	result = root["enDet"];

	// Return des Ergebnisses
	return result;

}

void Kommunikation::testKommunikation(){
	static unsigned long time = millis();

	bool signalOk = false;
	bool enemyDet = false;
	float x, y = -1000;
	float angle;

	signalOk = getPosition(x, y, angle);


	if(millis() > time + 100){
		time = millis();
		 signalOk = getPosition(x, y, angle);
		 enemyDet = getStopEnemy();
		 Serial.print("SigOk:  ");
		 Serial.print(signalOk);
		 Serial.print("   xPos:  ");
		 Serial.print(x);
		 Serial.print("   yPos:   ");
		 Serial.print(y);
		 Serial.print("   Genger vorraus:   ");
		 Serial.print(enemyDet);
		 Serial.println();
	}


}


// Master testen
// Der Master empfÃ¤ngt etwas

void Kommunikation::testAsMaster() {

	static unsigned long mili = millis();

	if (millis() > (mili + 1500)) {

		static int number = 0;
		Serial.print("Abfrage ");
		Serial.print(number);
		Serial.print(":  ");

		number++;

		String comString;

		Wire.requestFrom(5, 50);

		// Char array anlegen
		char c = 0;

		// Daten einlesen
		while (Wire.available()) {

			// Chars nacheinander empfangen und in ein char array schreiben
			c = Wire.read();
			comString += c;
		}

		// Buffer fÃ¼r den Json String
		StaticJsonBuffer<100> jsonBuffer;

		// Json Object aus dem Ã¼bergebenen string erstellen
		JsonObject& root = jsonBuffer.parseObject(comString);
		// Werte aus dem Json Objekt auslesen und den Ã¼bergebenen Werten zuweisen

		float left = root["left"];
		float right = root["right"];

		Serial.print(left);
		Serial.print("   ");
		Serial.print(right);
		Serial.println();

		mili = millis();
	}

}

void send() {
	Wire.write("hello i^2c");
}

  //Slave testen
  //Der Slave sendet etwas
void Kommunikation::testAsSlave() {

	Wire.begin(3);
	Wire.onRequest(send);

}
*/
