
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Kommunikation.h"
#include "Odometrie.h"

class Navigation {
private:
    Kommunikation JSON;
    Odometrie Odo;
private:
    int Beschleunigung = 10000;
    bool nothalt;
    
    bool master = true;
    
    int X_Koordinaten[10] = {0, 10, 15, 20, 25};
    int Y_Koordinaten[10] = {100, 25, 10, 5, 9};
    int x_aktuell = 200; // Startwert in x-Richtung
    int y_aktuell = 200; // Startwert in y-Richtung

    int KoordinatenAnfahren;
    float ZielpunktOffset = 10;
    
public:
    Navigation();
    void warte();
    void DrehungUmPunkt();
    void GeradeAusFahren();
    void naechestePosition();
    double weglaenge(int x, int y);
    double winkelBerechnen(int x, int y);
    void PositionBestimmen();
    int getAktuellesX();
    int getAktuellesY();
    int getTargetX();
    int getTargetY();
    float getWinkelOdo();
    float getTargetOffset();
};

#endif /* NAVIGATION_H */

