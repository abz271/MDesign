

#include "Roboter.h"

/*
#include <iostream>

using namespace std;

void Roboter::PunktAnfahren() {
    navi.PositionBestimmen();
    while (navi.weglaenge(navi.getTargetX(), navi.getTargetY()) > navi.getTargetOffset()) {
        navi.PositionBestimmen();
        Zielwinkel = navi.winkelBerechnen(navi.getTargetX(), navi.getTargetY());
        while (Zielwinkel != navi.getWinkelOdo()) {
            navi.DrehungUmPunkt();
            cout << "Dreht um Punkt\n" << endl;
            navi.warte();
        }
        navi.GeradeAusFahren();
        navi.warte();
        cout << "Fährt los\n" << endl;

    }
    navi.naechestePosition();
}


*/
