#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "graaf.h"
//functie om random vlottendekommagetallen te geven.
double frand(double maxf){
    return (random()%1000000000)*(maxf/1000000000.0);
}

class Stroomnetwerkgenerator{
public:
virtual GraafMetTakdata<GERICHT,int>* newStroomnet(int aantalKnopen, double takkans, int maxGewicht){
    GraafMetTakdata<GERICHT,int>* nieuweGraaf=new GraafMetTakdata<GERICHT,int>(aantalKnopen);
    for (int i=0; i<aantalKnopen; i++ ){
        for (int j=0; j<aantalKnopen ;j++ ){
            if (j!=i && frand(1) <= takkans)
                nieuweGraaf->voegVerbindingToe(i,j,rand()%maxGewicht);
        }
    }
    return nieuweGraaf;
}
protected:
};
