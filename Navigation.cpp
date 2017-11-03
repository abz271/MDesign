
#include "Odometrie.h"
#include "Navigation.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>


Navigation::Navigation() {
    //KarteErzeugen();
    KoordinatenAnfahren = 0;
}

void Navigation::naechestePosition() {
    KoordinatenAnfahren++;
}

double Navigation::winkelBerechnen(int x, int y) {

    // WinkelBerechnen gibt den um sich zu drehenden Winkel mit dem 
    // kürzesten Drehwinkel an ->  auch negative Zahlen
    // -90, um Koordinatensystem anzupassen!
    double delta_x = x - x_aktuell;
    double delta_y = y - y_aktuell;
    double Winkel = atan2(delta_y, delta_x) * 180 / M_PI;
    Winkel = (int) (((Winkel * 100 + 0.5) / 100.0) -90);

    return Winkel;
}

double Navigation::weglaenge(int x, int y) {
    int delta_x = x - x_aktuell;
    int delta_y = y - y_aktuell;
    return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

int Navigation::getTargetX() {
    return X_Koordinaten[KoordinatenAnfahren];
}

int Navigation::getTargetY() {
    return Y_Koordinaten[KoordinatenAnfahren];
}

int Navigation::getAktuellesX() {
    return x_aktuell;
}

int Navigation::getAktuellesY() {
    return y_aktuell;
}

float Navigation::getTargetOffset(){
    return ZielpunktOffset;
}
void Navigation::PositionBestimmen() {
    if (JSON.getSignalOk()) {
        x_aktuell = JSON.getX_position();
        y_aktuell = JSON.getY_position();
    } else {
        x_aktuell = Odo.getX_position();
        y_aktuell = Odo.getY_position();
    }
}

void Navigation::DrehungUmPunkt(){
    if (abs(winkelBerechnen(X_Koordinaten[KoordinatenAnfahren],Y_Koordinaten[KoordinatenAnfahren])) < 90){
        JSON.DatenAnHardware(Beschleunigung, -Beschleunigung, JSON.getNotHalt());
    }else{
        JSON.DatenAnHardware(-Beschleunigung, Beschleunigung, JSON.getNotHalt());
    }
}

void Navigation::GeradeAusFahren(){
    JSON.DatenAnHardware(Beschleunigung, Beschleunigung, JSON.getNotHalt());
}
void Navigation::warte(){
    for (int i = 0; i < 50000; i++);
}
float Navigation::getWinkelOdo(){
    float test1 = 1;
    float test2 = 2;
    //Odo.getRotaryChange(test1, test2)
    return 0;
}

