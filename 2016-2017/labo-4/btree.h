
#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include "bknoop.h"

#include <stack>

// Betekenis m: zie cursus

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

        Data zoeken(const Sleutel& sl) const;
        void voegtoe(const Sleutel& sl, const Data&);

    private:

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
Data BTree<Sleutel, Data, m>::zoeken(const Sleutel& sl) const
{
    if (wortel.is_aanwezig(sl))
    {
        return wortel.geef_data(sl);
    }

    blokindex huidige_index = wortel.geef_kindindex(sl);
    BKnoop<Sleutel, Data, m> huidige_knoop;

    schijf.lees(huidige_knoop, huidige_index);

    while (!huidige_knoop.is_blad())
    {
        if (huidige_knoop.is_aanwezig(sl))
        {
            return huidige_knoop.geef_data(sl);
        }

        huidige_index = huidige_knoop.geef_kindindex(sl);
        schijf.lees(huidige_knoop, huidige_index);
    }

    if (!huidige_knoop.is_aanwezig(sl))
    {
        throw "Sleutel is niet aanwezig in de BTree";
    }

    return huidige_knoop.geef_data(sl);
}


template<class Sleutel, class Data, blokindex m>
void BTree<Sleutel, Data, m>::voegtoe(const Sleutel& sl, const Data& d)
{
    // std::stack<blokindex> gebruikte_indexen;
    //
    // if (wortel.is_aanwezig(sl))
    // {
    //     wortel.update_data(sl, d);
    //     return;
    // }
    //
    // gebruikte_indexen.push(wortelindex);
    //
    // blokindex huidige_index = wortel.geef_kindindex(sl);
    // BKnoop<Sleutel, Data, m> huidige_knoop;
    //
    // schijf.lees(huidige_knoop, huidige_index);
    //
    // while (!huidige_knoop.is_blad())
    // {
    //     if (huidige_knoop.is_aanwezig(sl))
    //     {
    //         huidige_knoop.update_data(sl);
    //         return;
    //     }
    //
    //     gebruikte_indexen.push(huidige_index);
    //     huidige_index = huidige_knoop.geef_kindindex(sl);
    //     schijf.lees(huidige_knoop, huidige_index);
    // }
    //
    // if (huidige_knoop.is_aanwezig(sl))
    // {
    //     huidige_knoop.update_data(sl);
    //     return;
    // }
    //
    // while(huidige_knoop.is_vol)
    // {
    //     const int mid = huidige_knoop.aantal_kinderen() / 2;
    //
    //     BKnoop<Sleutel, Data, m> nieuwe_knoop{true};
    //
    //     for (int i = mid + 1; i < =)
    // }
}

#endif

