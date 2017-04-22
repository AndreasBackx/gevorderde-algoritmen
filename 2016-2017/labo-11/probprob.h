#ifndef __PROBPROB_H
#define __PROBPROB_H

#include <iostream>
#include <fstream>
#include <stdio.h>

using std::ostream;
using std::cerr;
using std::cout;

#include <vector>

using std::vector;
using std::pair;

//
// Probabilistic Probing
//
//De abstracte klasse ProbProb zoekt een vector van knopen die een optimale oplossing biedt voor een gegeven
//probleem waarvoor de zoekruimte te groot is om ze met backtracking helemaal af te gaan.
//Optimaal betekent met de beste waarde voor een evaluatiefunctie f.
//
//Hiervoor onderzoekt ze de zoekruimte op probabilistische wijze gebruik makend van een heuristiek h.
//
//
//Een oplossing bestaat uit een reeks waarden W_0, W_1, ..., W_k (waarbij  k niet vooraf gegeven hoeft te zijn)
//die aan bepaalde voorwaarden voldoet en waarvoor f kan berekend worden.
//
//Oplossingen worden probabilistisch opgebouwd, Gegeven een deeloplossing W_0, ... W_j wordt de verzameling
//V_j+1 van mogelijke waarden bepaald voor W_j+1. Tevens wordt de heuristische waarde
//h(W_0, ... W_j, v) berekend voor alle v in V_j+1. het algoritme kiest dan probabilistisch de volgende waarde
//W_j+1; de probabiliteit om v te kiezen is evenredig met h(W_0, ... W_j, v).
//
//Dit proces wordt n keren herhaald door de lidfunctie
//
//vector<Waarde> ProbProb<Waarde>::geefOptimum(int n)
//
//die de best gevonden oplossing teruggeeft.
//
//Er wordt van uitgegaan dat een deeloplossing die een oplossing is nooit kan uitgebreid worden tot een betere
//oplossing en dat een lege deeloplossing geen oplossing kan zijn.
//bijgevolg mag de code nooit f evalueren op een lege deeloplossing.

template <class Waarde>
class ProbProb
{
public:
    //indien geefOptimum er niet in slaagt ook maar 1 oplossing te vinden geeft ze een lege vector terug
    vector<Waarde> geefOptimum(int n);
protected:
    //de functie geefProbOplossing probeert op probabilistische manier 1 oplossing te vinden door
    //constructief steeds verder te zoeken.
    //Merk op: het kan zijn dat ze op dood spoor belandt
    //en een deeloplossing vindt die niet verder kan uitgebreid worden maar ook geen oplossing is.
    //In dat geval geeft ze een lege vector terug.
    vector<Waarde> geefProbOplossing();

    /**********************************************************************
    De onderstaande functies moeten ingevuld worden voor specifieke problemen.
     ***************************************************************************/
    virtual bool isOplossing(const vector<Waarde>& deeloplossing) = 0;
    virtual double f(const vector<Waarde>& oplossing) = 0;

    //gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
    //samen met hun heuristische waarde
    virtual vector<pair<Waarde, double> > geefVoortzetting(const vector<Waarde>& deeloplossing) = 0;
};

template <class Waarde>
vector<Waarde> ProbProb<Waarde>::geefProbOplossing()
{
    vector<Waarde> deeloplossing;
    bool deeloplossingIsOplossing = false;
    vector<pair<Waarde, double> > voortzetting = geefVoortzetting(deeloplossing); // Geef me alle mogelijke waarden om verder aan te vullen

    while (!deeloplossingIsOplossing && voortzetting.size() > 0) // Zolang oplossing niet gevonden en er nog mogelijke verdere waarden zijn
    {
        double totaleHeuristiek = 0;
        for (auto&& p : voortzetting)
        {
            std::cout << " (" << p.first << ", " << p.second << ")";
            totaleHeuristiek += p.second;
        }

        double lot = (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * totaleHeuristiek;

        std::cout << " | lot = " << lot;
        int i = 0;
        while (i < voortzetting.size() - 1 && lot > voortzetting[i].second)
        {

            lot -= voortzetting[i].second; // Aanpassen random pivot
            std::cout << ", " << lot;

            i++;
        };

        // Er is dus 1 / 5 kans dat hij vanaf de eerste keer 10 pakt, alvorens lot herberekent wordt en er naar de volgende waarden wordt gekeken:
        // (0, 10) (1, 9) (2, 8) (3, 7) (4, 6) (5, 5) (6, 4) (7, 3) (8, 2) (9, 1)
        // 10 / 54 = 0.185

        std::cout << " | i = " << i << " | select = (" << voortzetting[i].first << ", " << voortzetting[i].second << ")";

        deeloplossing.push_back(voortzetting[i].first); // De eerste wiens heuristiek boven de random pivot zit (indien geen enkelm het allerlaatste element)
        deeloplossingIsOplossing = isOplossing(deeloplossing);

        if (!deeloplossingIsOplossing) // Indien de deeloplossing geen oplossing, zoek nieuwe volgende waarden
        {
            voortzetting = geefVoortzetting(deeloplossing);
        }

        std::cout << std::endl;
    };

    if (!deeloplossingIsOplossing)
    {
        deeloplossing.clear();
    }

    return deeloplossing;
};

template <class Waarde>
vector<Waarde> ProbProb<Waarde>::geefOptimum(int n)
{
    std::ofstream out("out.csv");

    vector<Waarde> optimum;
    double optimumF;
    for (int i = 0; i < n; i++)
    {
        vector<Waarde> poging = geefProbOplossing();

        cout << i << " |";
        for (auto&& w : poging)
        {
            cout << " " << w;
        }

        if (poging.size() > 0) //poging leverde geldige oplossing
        {
            double pogingF = f(poging);
            cout << " | f = " << pogingF;
            out << i << "," << pogingF << "\n";
            if (optimum.size() == 0 || pogingF < optimumF)
            {

                cout << " | Beste oplossing";

                optimum = move(poging);
                optimumF = pogingF;
            };
        };

        cout << "\n";
    }
    return optimum;
};

#endif
