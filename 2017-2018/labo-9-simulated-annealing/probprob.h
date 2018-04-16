#ifndef PROBPROB_H
#define PROBPROB_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

//
// Probabilistic Probing
//
// De abstracte klasse ProbProb zoekt een vector van knopen die een optimale oplossing biedt voor een gegeven probleem
// waarvoor de zoekruimte te groot is om ze met backtracking helemaal af te gaan. Optimaal betekent met de beste waarde
// voor een evaluatiefunctie f. Hiervoor onderzoekt ze de zoekruimte op probabilistische wijze gebruik makend van een
// heuristiek h.
//
//
// Een oplossing bestaat uit een reeks waarden W_0, W_1, ..., W_k (waarbij k niet vooraf gegeven hoeft te zijn) die aan
// bepaalde voorwaarden voldoet en waarvoor f kan berekend worden.
//
// Oplossingen worden probabilistisch opgebouwd, Gegeven een deeloplossing W_0, ... W_j wordt de verzameling V_j+1 van
// mogelijke waarden bepaald voor W_j+1. Tevens wordt de heuristische waarde h(W_0, ... W_j, v) berekend voor alle v in
// V_j+1. het algoritme kiest dan probabilistisch de volgende waarde W_j+1; de probabiliteit om v te kiezen is evenredig
// met h(W_0, ... W_j, v).
//
// Dit proces wordt n keren herhaald door de lidfunctie
//
//      vector<T> ProbProb<T>::geefOptimum(int n)
//
// die de best gevonden oplossing teruggeeft.
//
// Er wordt van uitgegaan dat een deeloplossing die een oplossing is nooit kan uitgebreid worden tot een betere
// oplossing en dat een lege deeloplossing geen oplossing kan zijn. bijgevolg mag de code nooit f evalueren op een lege
// deeloplossing.
//

template <class T>
class ProbProb
{
public:
    // Indien geefOptimum er niet in slaagt ook maar 1 oplossing te vinden geeft ze een lege vector terug
    std::vector<T> geefOptimum(int n);

protected:
    // De functie geefProbOplossing probeert op probabilistische manier 1 oplossing te vinden door constructief steeds
    // verder te zoeken. Merk op: het kan zijn dat ze op dood spoor belandt en een deeloplossing vindt die niet verder
    // kan uitgebreid worden maar ook geen oplossing is. In dat geval geeft ze een lege vector terug.

    std::vector<T> geefProbOplossing();

    ////////////////////////////////////////////////////////////////////////////
    // De onderstaande functies moeten ingevuld worden voor specifieke problemen.
    ////////////////////////////////////////////////////////////////////////////

    virtual bool isOplossing(const std::vector<T>& deeloplossing) = 0;
    virtual double f(const std::vector<T>& oplossing) = 0;

    // Gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen samen met hun heuristische
    // waarde
    virtual std::vector<std::pair<T, double>> geefVoortzetting(const std::vector<T>& deeloplossing) = 0;
};

//
// Stel h0 = 8, h1 = 4, h2 = 12, h3 = 6
// som = 30
//
// We willen dat de grootste h het meeste kans heeft om geselecteerd te worden. De vector kan visueel voorgesteld worden
// als volgt:
//
// <----8---><--4-><-----12-----><---6-->
//
// --------------------^ = de geselecteerde waarde door kans: s = a * som = 0.667 * 30 = 20
//
// lot0 = 20: 8 < 20 dus we gaan door en lot1 = (lot0 - 8)
// lot1 = 12: 4 < 12 dus we gaan door en lot2 = (lot1 - 4)
// lot2 = 8 : 12 > 8 dus het geselecteerde heuristisch gewicht is gevonden
//
template <class T>
std::vector<T> ProbProb<T>::geefProbOplossing()
{
    std::vector<T> deeloplossing;
    bool deeloplossingIsOplossing = false;
    std::vector<std::pair<T, double>> voortzetting = geefVoortzetting(deeloplossing);

    while (!deeloplossingIsOplossing && (voortzetting.size() > 0))
    {
        double totaleHeuristiek = 0;

        for (auto&& p : voortzetting)
        {
            totaleHeuristiek += p.second;
        }

        double lot = rand() * (totaleHeuristiek / RAND_MAX);

        int i = 0;
        while (i < voortzetting.size() - 1 && lot > voortzetting[i].second)
        {
            lot -= voortzetting[i].second;
            i++;
        }

        deeloplossing.push_back(voortzetting[i].first);
        deeloplossingIsOplossing = isOplossing(deeloplossing);

        if (!deeloplossingIsOplossing)
        {
            voortzetting = geefVoortzetting(deeloplossing);
        }
    }

    if (!deeloplossingIsOplossing)
    {
        deeloplossing.clear();
    }

    return deeloplossing;
}

template <class T>
std::vector<T> ProbProb<T>::geefOptimum(int n)
{
    std::vector<T> optimum;
    double optimumF;

    for (int i = 0; i < n; i++)
    {
        std::vector<T> poging = geefProbOplossing();

        if (poging.size() > 0)
        { // poging leverde geldige oplossing
            double pogingF = f(poging);

            if (optimum.size() == 0 || pogingF < optimumF)
            {
                std::cout << "Beste oplossing: " << pogingF << std::endl;
                optimum = move(poging);
                optimumF = pogingF;
            }
        }
    }

    return optimum;
}

#endif
