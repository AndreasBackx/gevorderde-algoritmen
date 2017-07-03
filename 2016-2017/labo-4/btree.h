
#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include "bknoop.h"

#include <stack>
#include <cmath>
#include <string>
#include <sstream>
#include <unordered_set>

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

    BTree(Schijf<BKnoop<Sleutel, Data, m>>&schijf);
    ~BTree() = default;

    BTree(const BTree<Sleutel, Data, m>& other) = default;
    BTree& operator=(const BTree<Sleutel, Data, m>& other) = default;

    BTree(BTree<Sleutel, Data, m>&& other) = default;
    BTree& operator=(BTree<Sleutel, Data, m>&& other) = default;

    Data zoek(const Sleutel& nieuwe_sleutel) const;
    void voegtoe(const Sleutel& nieuwe_sleutel, const Data&);

    std::string to_string() const;

private:

    BKnoop<Sleutel, Data, m> splits_knoop(BKnoop<Sleutel, Data, m>& originele_knoop, Sleutel& middel_sleutel, Data& middel_data);
    void append_knoop_to_string(blokindex huidige_index, std::stringstream& out, std::unordered_set<blokindex>& bezochte_indexen, int depth) const;

    Schijf<BKnoop<Sleutel, Data, m>>&schijf;
    BKnoop<Sleutel, Data, m> wortel;
    blokindex wortelindex;
};

template<class Sleutel, class Data, blokindex m>
BTree<Sleutel, Data, m>::BTree(Schijf<BKnoop<Sleutel, Data, m>>&schijf)
: schijf{schijf}, wortel{true}
{
    wortelindex = schijf.schrijf(wortel);
}

template<class Sleutel, class Data, blokindex m>
Data BTree<Sleutel, Data, m>::zoek(const Sleutel& nieuwe_sleutel) const
{
    BKnoop<Sleutel, Data, m> huidige_knoop = wortel;
    blokindex huidige_index = wortelindex;

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
    // TODO vertalen naar public functies in BKnoop

    const int middel_pivot = static_cast<int> (std::ceil(originele_knoop.aantal_kinderen() / 2));

    BKnoop<Sleutel, Data, m> nieuwe_knoop{originele_knoop.is_blad_knoop};

    middel_sleutel = originele_knoop.sleutel[middel_pivot];
    middel_data = originele_knoop.data[middel_pivot];

    if (!nieuwe_knoop.is_blad_knoop)
    {
        nieuwe_knoop.index[0] = originele_knoop.index[middel_pivot];
    }

    for (int i = middel_pivot + 1; i <= originele_knoop.k; i++)
    {
        nieuwe_knoop.sleutel[i - middel_pivot] = originele_knoop.sleutel[i];
        nieuwe_knoop.data[i - middel_pivot] = originele_knoop.data[i];

        if (!nieuwe_knoop.is_blad_knoop)
        {
            nieuwe_knoop.index[i - middel_pivot] = originele_knoop.index[i];
        }
    }

    nieuwe_knoop.k = originele_knoop.k - middel_pivot;
    originele_knoop.k = middel_pivot - 1;

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

            if (huidige_index == wortelindex)
            {
                wortel = huidige_knoop;
            }

            schijf.herschrijf(huidige_knoop, huidige_index);

            return;
        }

        huidige_index = huidige_knoop.geef_kindindex(nieuwe_sleutel);
        schijf.lees(huidige_knoop, huidige_index);
    }

    if (huidige_knoop.is_aanwezig(nieuwe_sleutel))
    {
        huidige_knoop.update_data(nieuwe_sleutel, nieuwe_data);

        if (huidige_index == wortelindex)
        {
            wortel = huidige_knoop;
        }

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
        gebruikte_blokindexen.pop();
        schijf.lees(huidige_knoop, huidige_index);

        huidige_knoop.voegtoe(middel_sleutel, middel_data, nieuwe_knoop_index);
    }

    if (huidige_index == wortelindex)
    {
        wortel = huidige_knoop;
    }

    if (!wortel.is_vol())
    {
        schijf.herschrijf(huidige_knoop, huidige_index);
    }
    else
    {
        Sleutel middel_sleutel;
        Data middel_data;
        BKnoop<Sleutel, Data, m> nieuwe_knoop = splits_knoop(wortel, middel_sleutel, middel_data);
        schijf.herschrijf(wortel, wortelindex);
        blokindex nieuwe_knoop_index = schijf.schrijf(nieuwe_knoop);

        wortel = BKnoop<Sleutel, Data, m>{wortelindex};
        wortel.voegtoe(middel_sleutel, middel_data, nieuwe_knoop_index);
        wortelindex = schijf.schrijf(wortel);
    }
}

template<class Sleutel, class Data, blokindex m>
std::string BTree<Sleutel, Data, m>::to_string() const
{
    std::stringstream out;

    out << "Wortel: index " << wortelindex << " | " << wortel.to_string() << std::endl;

    std::stack<blokindex> te_bezoeken_indexen;
    std::unordered_set<blokindex> bezochte_indexen;
    append_knoop_to_string(wortelindex, out, bezochte_indexen, 0);

    return out.str();
}

template<class Sleutel, class Data, blokindex m>
void BTree<Sleutel, Data, m>::append_knoop_to_string(blokindex huidige_index, std::stringstream& out, std::unordered_set<blokindex>& bezochte_indexen, int depth) const
{
    if (bezochte_indexen.find(huidige_index) != bezochte_indexen.end())
    {
        return;
    }

    BKnoop<Sleutel, Data, m> huidige_knoop;
    schijf.lees(huidige_knoop, huidige_index);

    for (int i = 0; i < depth; i++)
    {
        out << "    ";
    }

    out << "Index " << huidige_index << " | " << huidige_knoop.to_string() << std::endl;

    bezochte_indexen.insert(huidige_index);

    if (!huidige_knoop.is_blad())
    {
        for (int i = 0; i <= huidige_knoop.k; i++) // TODO vertalen naar public functies in BKnoop
        {
            append_knoop_to_string(huidige_knoop.index[i], out, bezochte_indexen, (depth + 1));
        }
    }
}

#endif

