#include "Motor.h"
#include <math.h>
#include <stdlib.h>

Motor::Motor(){


}

Motor::Motor(unsigned char pin, unsigned char dirpin) {
    /* Konstruktor berechnet zum Anfang des Programms die Lookup-Table, um spätere Berechnungen zu beschleunigen.
     * Pins für Geschwindigkeit und Richtung werden ebenfalls hier gesetzt
     */

    // Pin-Zuweisung
    this->pin = pin;
    this->dirpin = dirpin;

    // Erstellung der Beschleunigungs-Lookup-Table
    int ramp_size;
    int max_a_size;
    int adj_formfactor;

    float conversion_factor;
    float ramp_incline;

    this->samplerate = SAMPLERATE;
    this->formfactor = FORMFACTOR;
    this->a_max = AMAX;

    // Berechnung der Beschleunigungsrampe: Umrechnungen
    conversion_factor = (float) samplerate / 100;
    adj_formfactor = (int) (conversion_factor * formfactor);
    ramp_size = samplerate - adj_formfactor;
    max_a_size = adj_formfactor - ramp_size;

    // Berechnung der Beschleunigungsrampe: Diskretisierung
    ramp_incline = (float) a_max / (float) ramp_size;
    for (int i = 0; i < ramp_size; i++) {
        a[i] = (int) round(i * ramp_incline);
    }

    // Plateau maximaler Beschleunigung
    for (int i = ramp_size - 1; i < SAMPLERATE / 2; i++) {
        a[i] = a_max;
    }

    // Initialisiere Werte
    v = 0;
    index = 0;
}

Motor::~Motor() {
    // nothing to see here~~~
}

void Motor::calcMovement(unsigned char v1) {
    /* Vorbereitung der Beschleunigungsberechnung. Ermittlung der PWM-Differenz.
     * Wird bei jeder neuen Bewegung ausgeführt (jedoch nicht bei kleinen Kurskorrekturen).
     */
    this->v1 = v1;
    v0 = v;
    v_delta = abs(this->v1 - v0);
    v_delta_half = v_delta / 2;
    v1greaterv0 = this->v1 > v0 ? true : false;
    index = 0;
    ramp_started = false;
    ramp_symmetry = false;
    compensation = false;
}

unsigned char Motor::calcPWM(bool emergencyOverride) {
    /* Berechnung der Beschleunigung im Taktzyklus des Kurskorrektur-Algorithmus.
     * emergencyOverride schaltet die Beschleunigungsberechnung aus und gibt direkt den gewünschten PWM-Wert aus.
     * Falls der Unterschied zwischen altem und neuen PWM-Wert < JMAX, beschleunigt calcPWM direkt auf neuen PWM-Wert
     * Gibt neuen PWM-Wert zurück.
     */
    unsigned char lut_sigma = a_max / 2 + 1;
    unsigned char v_delta_to_half = abs(abs(v1 - v) - v_delta_half);
    //printf("v_current_delta: %u\n", v_delta_to_half);

    if (!emergencyOverride) {
        if (abs(v1 - v0) < JMAX) {
            // Springt zu neuem PWM-Wert, wenn Unterschied klein ist
            index = 0;
            v = v1;
        } else if (v < 255) {
            // Beschleunigungsberechnung anhand Lookup-Table
            v = v1greaterv0 ? v + a[index] : v - a[index];
            //printf("index: %u v: ", index);
            if (v_delta_to_half > lut_sigma && !ramp_started && index < SAMPLERATE / 2 - 1) {
                index++;
                //            } else if (!ramp_started && (v1 - 2 * v) > 2) {
                //                // pass through
                //                printf("Compensation\n");
            } else if (!ramp_started) {
                // höchster Wert muss zur korrekten Berechnung 1 Mal wiederholt werden
                ramp_started = true;
            } else if (ramp_started && index > 0) {
                index--;
            }
        }
    } else {
        // emergencyOverride! Potentiell schädlich für Mechanik. Nur im Notfall einsetzen!
        index = 0;
        v = v1;
    }
    return v;
}

void Motor::setPwM(unsigned char newPWM, bool dir) {
    //analogOut(pin, newPWM);
    //digitalOut(dirpin, dir);
    printf("setPWM: %u, %u, %i, %u", newPWM, pin, dir, dirpin);
}

void Motor::dumpData() {
    /* Debug-Methode.
     * Gibt Daten und Ergebnisse von Berechnungen aus
     */

    int a_sum = 0;
    // Debug

    // Constructor:
    printf("CONSTRUCTOR:\n");
    printf("pin number: %u\n", pin);
    printf("acceleration lookup table:\n");
    for (int i = 0; i < sizeof (a) / sizeof (char); i++) {
        printf("%3i: %i\n", i, a[i]);
    }
    printf("/acceleration lookup table\n");
    for (int i = 0; i < sizeof (a) / sizeof (char); i++) {
        a_sum += a[i];
    }
    printf("sum of a: %i\n", a_sum);
    printf("/CONSTRUCTOR\n\n");

    // calcMovement:
    printf("CALCMOVEMENT:\n");
    printf("v0:\t\t%3u\nv1:\t\t%3u\nv_delta:\t%3u\nv1greaterv0:\t%3i\n", v0, v1, v_delta, v1greaterv0);
    printf("/CALCMOVEMENT\n\n");

    // calcPWM:
    printf("CALCPWM:\n");
    for (unsigned char i = 0; i < 100; i++) {
        printf("v %3u:\t%3u\n", i, this->calcPWM(false));
    };
    printf("/CALCPWM\n\n");
}


// Methode zur Ansteuerung der Motoren
void Motor::sendMotorPower(float leftWheelPower, float rightWheelPower) {
	// TODO: Motoransteuerung realisieren.
}
