#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <functional>
using namespace std;

void functie( std::function<double()> punt, std::function<int(int)> index){
    int aantal=10000;
    vector<double> punten(aantal);
    for (int i=0; i<aantal; i++ ){
        punten[i]=punt();
    }
    for (int i=0; i<aantal; i++ ){
        int ind=index(aantal);
    }
};
//geeft een index terug die voor 99,9% zeker in de laatste 0,01% van de range ligt
int scheveIndex(int aantal){
    int keuze=random()%1000;
    if (keuze==999)
        return random()%(99*aantal/100);
    else
        return (99*aantal/100) + random()%(aantal/100);
};


int main(int argc, char *argv[]){
    int aantal=10000;
    functie([aantal](){return random()%aantal;},[](int a){return random()%a;});
    functie([aantal](){return random()%aantal;},scheveIndex);
    return 0;
}
