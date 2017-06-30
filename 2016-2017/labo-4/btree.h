
#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include "bknoop.h"

#include <stack>
#include <cmath>
#include <string>
#include <sstream>

// Betekenis m: zie cursus

// Om het overzicht te bewaren hou ik hier een zo kleine mogelijke koppeling tussen BTree en BKnoop.
// Dit houdt in dat ik niet aan de private variabelen van BKnoop kom vanuit BTree en enkel de public methodes gebruik.
// Dit heeft als voordeel dat BKnoop kan geimplementeerd en aangepast worden, zonder dat BTree dit ooit merkt (zolang
// BKnoop de unittests respecteert). Ook maakt dit de code veel leesbaarder. Het nadeel is dat er een
// performantieverlies doordat er meerdere keren door de BKnoop wordt gelopen (bv is_aanwezig() gevolg door
// update_data() enz ...). Omdat de focus op de BTree ligt en het aantal lementen per knoop vaak in de orde van
// 0 tot hondertallen, heb ik besloten om leesbaarheid en losse koppeling te laten primeren op de (vermoedelijk geringe)
// performantieverlies.

template<class Sleutel, class Data, blokindex m>
class BKnoop;

template<class Sleutel, class Data, blokindex m>
class BTree
{
    public:

        BTree(Schijf<BKnoop<Sleutel, Data, m>>& schijf);
        ~BTree() = default;

        BTree(const BTree<Sleutel, Data, m>& other) = default;
        BTree& operator=(const BTree<Sleutel, Data, m>& other) = default;

        BTree(BTree<Sleutel, Data, m>&& other) = default;
        BTree& operator=(BTree<Sleutel, Data, m>&& other) = default;

        Data zoeken(const Sleutel& nieuwe_sleutel) const;
        void voegtoe(const Sleutel& nieuwe_sleutel, const Data&);

        std::string to_string() const;

    private:

        BKnoop<Sleutel, Data, m> splits_knoop(BKnoop<Sleutel, Data, m>& originele_knoop, Sleutel& middel_sleutel, Data& middel_data);

        Schijf<BKnoop<Sleutel, Data, m>>& schijf;
        BKnoop<Sleutel, Data, m> wortel;
        blokindex wortelindex;
};

    template<class Sleutel, class Data, blokindex m>
BTree<Sleutel, Data, m>::BTree(Schijf<BKnoop<Sleutel, Data, m>>& schijf)
    : schijf{schijf}, wortel{true}
{
    wortelindex = schijf.schrijf(wortel);
}

template<class Sleutel, class Data, blokindex m>
Data BTree<Sleutel, Data, m>::zoeken(const Sleutel& nieuwe_sleutel) const
{
    if (wortel.is_aanwezig(nieuwe_sleutel))
    {
        return wortel.geef_data(nieuwe_sleutel);
    }

    blokindex huidige_index = wortel.geef_kindindex(nieuwe_sleutel);
    BKnoop<Sleutel, Data, m> huidige_knoop;

    schijf.lees(huidige_knoop, huidige_index);

    while (!huidige_knoop.is_blad())
    {
        if (huidige_knoop.is_aanwezig(nieuwe_sleutel))
        {
            return huidige_knoop.geef_data(nieuwe_sleutel);
        }

        huidige_index = huidige_knoop.geef_kindindex(nieuwe_sleutel);
        schijf.lees(huidige_knoop, huidige_index);
    }

    if (!huidige_knoop.is_aanwezig(nieuwe_sleutel))
    {
        throw "Sleutel is niet aanwezig in de BTree";
    }

    return huidige_knoop.geef_data(nieuwe_sleutel);
}

    template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m> BTree<Sleutel, Data, m>::splits_knoop(BKnoop<Sleutel, Data, m>& originele_knoop, Sleutel& middel_sleutel, Data& middel_data)
{
    const int middel_index = static_cast<int>(std::floor(originele_knoop.aantal_kinderen() / 2));
    // bv.
    // 2 kinderen = 0 1 -> 1 (2/2 = 1)
    // 3 kinderen = 0 1 2 -> 1 (3/2 -> 1.5)
    // 4 kinderen = 0 1 2 3 -> 2 (4/2 -> 2)
    // 5 kinderen = 0 1 2 3 4 -> 2 (5/2 -> 2.5)

    BKnoop<Sleutel, Data, m> nieuwe_knoop{originele_knoop.is_blad()};

    middel_sleutel = nieuwe_knoop[middel_index];
    middel_data = nieuwe_knoop.geef_data(middel_sleutel);

    for (int i = middel_index + 1; i < nieuwe_knoop.aantal_kinderen(); i++)
    {
        nieuwe_knoop.voegtoe(
                nieuwe_knoop[i],
                nieuwe_knoop.geef_data(nieuwe_knoop[i])
                );
    }

    return nieuwe_knoop;
}

    template<class Sleutel, class Data, blokindex m>
void BTree<Sleutel, Data, m>::voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data)
{
    blokindex huidige_index = wortelindex;
    BKnoop<Sleutel, Data, m> huidige_knoop = wortel;

    std::stack<blokindex> gebruikte_blokindexen;

    while (!huidige_knoop.is_blad())
    {
        gebruikte_blokindexen.push(huidige_index);

        if (huidige_knoop.is_aanwezig(nieuwe_sleutel))
        {
            huidige_knoop.update_data(nieuwe_sleutel, nieuwe_data);
            schijf.herschrijf(huidige_knoop, huidige_index);
            return;
        }

        huidige_index = huidige_knoop.geef_kindindex(nieuwe_sleutel);
        schijf.lees(huidige_knoop, huidige_index);
    }

    if (huidige_knoop.is_aanwezig(nieuwe_sleutel))
    {
        huidige_knoop.update_data(nieuwe_sleutel, nieuwe_data);
        schijf.herschrijf(huidige_knoop, huidige_index);
        return;
    }

    huidige_knoop.voegtoe(nieuwe_sleutel, nieuwe_data);

    while (huidige_knoop.is_vol()
            && huidige_index != wortelindex
            && !gebruikte_blokindexen.empty())
    {
        Sleutel middel_sleutel;
        Data middel_data;

        BKnoop<Sleutel, Data, m> nieuwe_knoop = splits_knoop(huidige_knoop, middel_sleutel, middel_data);
        schijf.herschrijf(huidige_knoop, huidige_index);
        blokindex nieuwe_knoop_index = schijf.schrijf(nieuwe_knoop);

        huidige_index = gebruikte_blokindexen.top();
        schijf.lees(huidige_knoop, huidige_index);

        huidige_knoop.voegtoe(middel_sleutel, middel_data, nieuwe_knoop_index);
    }

    if (!wortel.is_vol())
    {
        schijf.herschrijf(huidige_knoop, huidige_index);
    }
    else
    {
        Sleutel middel_sleutel;
        Data middel_data;
        BKnoop<Sleutel, Data, m> nieuwe_knoop = splits_knoop(wortel, middel_sleutel, middel_sleutel);
        schijf.herschrijf(wortel, wortelindex);
        blokindex nieuwe_knoop_index = schijf.schrijf(nieuwe_knoop);

        wortel = BKnoop<Sleutel, Data, m>{wortelindex};
        wortel.voegtoe(middel_sleutel, middel_data, nieuwe_knoop_index);
        blokindex wortelindex = schijf.schrijf(wortel);
    }
}

template<class Sleutel, class Data, blokindex m>
std::string BTree<Sleutel, Data, m>::to_string() const
{
    std::stringstream out;



    return out.str();
}

#endif

