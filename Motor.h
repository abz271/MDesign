
/*
 * File:   motorsteuerung.h
 * Author: ugen
 *
 * Created on November 7, 2017, 10:16 AM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include <stdio.h>
#include "Config.h"

class Motor {
private:
    unsigned char samplerate; // Samplerate des Hauptprogramms.
    unsigned char formfactor; // Bestimmt die Form der Beschleunigungsrampe

    unsigned char v0; // alte Geschwindigkeit (PWM))
    unsigned char v1; // geplante Geschwindigkeit (PWM))
    unsigned char v_delta; // Geschwindigkeitsdifferenz (PWM))
    unsigned char v_delta_half; // halbe Geschwindigkeitsdifferenz (PWM)
    unsigned char v; // aktuelle Geschwindigkeit (PWM)

    unsigned char a[SAMPLERATE / 2]; // halbe Beschleunigungsrampe
    unsigned char a_max; // Maximale Beschleunigung in Bit/Sample
    unsigned char index; // Für PWM-Berechnung

    unsigned char pin;  // Analog-Out-Pin
    unsigned char dirpin; // Digital-Out-Pin

    bool v1greaterv0;   // Beschleunigung?
    bool ramp_started;    // pos. Beschleunigungsrampe gefahren
    bool ramp_symmetry; // Rampe muss symmetrisch sein
    bool compensation;  // Kompensation, um Ausgabefehler gering zu halten

public:
    Motor();
    Motor(unsigned char pin, unsigned char dirpin);
    ~Motor(); // leerer Destruktor

    void calcMovement(unsigned char v1); // Vorberechnungen für Beschleunigung auf geplante Geschwindigkeit
    unsigned char calcPWM(bool emergencyOverride); // Berechnet anhand Beschleunigungstabelle Geschwindigkeit
    void setPwM(unsigned char newPWM, bool dir); // Gibt PWM und Drehrichtung an Pins aus
    void dumpData(); // Data Dump für Debugging
    void sendMotorPower(float leftWheelPower, float rightWheelPower);
};

#endif	/* MOTOR_H */

