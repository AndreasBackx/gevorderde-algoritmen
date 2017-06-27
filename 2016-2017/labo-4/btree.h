
#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include "bknoop.h"

#include <iostream>

// Betekenis m: zie cursus
template<class Sleutel, class Data, blokindex m>
class Btree
{
private:

    using Knoop = BKnoop<Sleutel, Data, m>;

public:

    Btree(Schijf<Knoop>& s) : schijf(s)
    {
        wortelindex = schijf.schrijf(wortel);
    }

    Data zoeken(const Sleutel& sl);

private:

    Schijf<Knoop>& schijf;
    Knoop wortel;
    blokindex wortelindex;
};

template<class Sleutel, class Data, blokindex m>
Data Btree<Sleutel, Data, m>::zoeken(const Sleutel& sl)
{
    Knoop huidig = wortel;

    while (!huidig.is_blad)
    {
        int i = 1;
        while (i <= huidig.k
            && sl < huidig.sl[i])
        {
            if (huidig.sl[i] == sl)
            {
                return huidig.data[i];
            }

            i++;
        }
        i--;

        schijf.lees(huidig, huidig.index[i]);
    }

}

#endif
