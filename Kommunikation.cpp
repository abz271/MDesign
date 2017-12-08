
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
	// TODO: fÃ¼r Testzwecke auch als Slave, Adresse einfÃ¼gen
	Wire.begin();

}

/*
 * Fragt die Daten Ã¼ber den i^2c Bus von der Hardware Gruppe an
 * und gibt sie per Reference zurÃ¼ck
 * i^2c Bus Methode
 */
void Kommunikation::DataFromHardware(String& comString) {

	// TODO: String grÃ¶ÃŸe ermitteln aus funktion
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
 * Sendet sie Daten fÃ¼r dei Moteren an die Hardware Gruppe
 * i^2c Bus Methode
 */

// TODO: Methode wird nicht mehr genutzt. Kann gelÃ¶scht werden?
void Kommunikation::DataToHardware(String comString) {

	// Char Array anlegen
	char charComString[stringLength];
	comString.toCharArray(charComString, stringLength);

	// Kommunikation beginnen
	Wire.beginTransmission(hardwareAddress);

	// Daten Ã¼bertragen, als normales Char Array Ã¼bertragen
	Wire.write(charComString);

	// Kommunikation beenden
	Wire.endTransmission();

}

/*
 * Fragt die Daten über den i2c Bus von der Positions Gruppe an
 * i2c Methode
 */
void Kommunikation::DataFromPosition(String& comString) {

	// TODO: String größe ermitteln aus funktion
	// Kommunikation starten, Daten anfragen

	Wire.requestFrom(positionAddress, stringLength);

	// Char array anlegen
	char c;

	// Daten einlesen
	while (Wire.available()) {

		// Chars nacheinander empfangen und in ein char array schreiben
		c = Wire.read();
		comString += c;
	}

	Serial.println(comString);

}

/*
 * Gibt die aktuelle Position der Positionsgruppe per Referenze zurück
 * Außerdem wird ein bool zurück gegeben ob die Information verwendbar ist oder nicht
 * Diese Methode kümmert sich um das Übersetzen des JSONs
 */
bool Kommunikation::getPosition(float& xPos, float& yPos) {

	// Buffer fÃ¼r den Json String
	StaticJsonBuffer<100> jsonBuffer;
	String comString;

	// Daten von dem Positionsteam abfragen
	DataFromPosition(comString);

	// Json Object aus dem Ã¼bergebenen string erstellen
	JsonObject& root = jsonBuffer.parseObject(comString);

	// TODO: Nach Ãœbersetzungsfehler prÃ¼fen

	// Werte aus dem Json Objekt auslesen und den Ã¼bergebenen Werten zuweisen
	xPos = root["x"];
	yPos = root["y"];
	return root["e"];

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

/*
 *	Gibt einen Bool zurÃ¼ck, wenn das Signal des Positionsbestimmungsteams gut ist.
 *  Wird in den Funktionen genutzt, um Daten zu aktualisieren und zu kalibrieren
 */
 /*
bool Kommunikation::getSignalUsefull() {
	// Buffer fÃ¼r den Json String
	StaticJsonBuffer<100> jsonBuffer;
	// Variablen
	bool result = false;
	String comString;
	// Daten von dem Positions-Team abfragen
	DataFromPosition(comString);
	// Einen Json String erstellen
	JsonObject& root = jsonBuffer.parseObject(comString);
	//Daten in den Json String schreiben
	result = root["sigOk"];
	// Return des Ergebnisses
	return result;
}


void Kommunikation::testKommunikation(){
	static unsigned long time = millis();

	bool signalOk = false;
	bool enemyDet = false;
	float x, y = -1000;

	signalOk = getPosition(x, y);


	if(millis() > time + 100){
		time = millis();
		 signalOk = getPosition(x, y);
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
