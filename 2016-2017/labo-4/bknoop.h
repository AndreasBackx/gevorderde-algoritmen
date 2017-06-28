
#ifndef BKNOOP_H
#define BKNOOP_H

#include "schijf.h"
#include "btree.h"

#include <string>
#include <sstream>
#include <iostream>

// Betekenis m: zie cursus
// TODO BKnoop opslitsen in klasse Blad en NietBlad die elk een interface overerven
//

template<class Sleutel, class Data, blokindex m>
class BTree;

template<class Sleutel, class Data, blokindex m>
class BKnoop
{

    friend class BTree<Sleutel, Data, m>;

public:

    BKnoop(bool is_blad_knoop);
    BKnoop();
    BKnoop(blokindex linkerkind, const Sleutel& sl, const Data& d, blokindex rechterkind);
    ~BKnoop();

    BKnoop(const BKnoop<Sleutel, Data, m>& other) = default;
    BKnoop& operator=(const BKnoop<Sleutel, Data, m>& other) = default;

    BKnoop(BKnoop<Sleutel, Data, m>&& other) = default;
    BKnoop& operator=(BKnoop<Sleutel, Data, m>&& other) = default;

    bool is_aanwezig(const Sleutel& nieuwe_sleutel) const;
    bool is_blad() const;
    Data geef_data(const Sleutel& nieuwe_sleutel) const; // Geen return by reference, want schijfpagina wordt niet in het geheugen gehouden (max 3)
    void update_data(const Sleutel& sl, const Data& d);
    void voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data, blokindex nieuw_rechterkind);
    bool is_vol() const;
    blokindex geef_kindindex(const Sleutel& nieuwe_sleutel) const;
    int aantal_kinderen() const;

    Sleutel operator[](int index) const;
    std::string to_string() const;

private:

    // Opmerking: sleutel[0] en data[0] worden niet gebruikt, zodat indexen vanaf 1 lopen
    //
    // m = 4
    // ARRAY_SIZE = 5
    // sleutel[]   (0)   1   2   3   4      | 4 sleutels
    // data[]      (0)   1   2   3   4      | 4 data
    // index[]         0   1   2   3   4    | 5 indexes
    const static int ARRAY_SIZE = m + 1;
    Sleutel sleutel[ARRAY_SIZE];
    Data data[ARRAY_SIZE];
    blokindex index[ARRAY_SIZE];
    int k;
    bool is_blad_knoop;


    // Crashtest voorkomt dat er meer dan drie knopen in het 'geheugen' zitten (excusief de wortel).
    // Dit veronderstelt wel dat er nooit knopen op de Schijf gedeletete worden.
    static int crashtest;
};

template<class Sleutel, class Data, blokindex m>
int BKnoop<Sleutel, Data, m>::crashtest = 0;

template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop(bool is_blad_knoop)
: k{0}, is_blad_knoop{is_blad_knoop}
{
    crashtest++;

    // // Dit crasht als er een 4e knoop wordt aangemaakt
    // // Zie de limiet van 3 schijfknopen in het intern (RAM) geheugen in de opgave
    // int crashresult = 4 / (-4 + crashtest);

    if (crashtest > 3)
    {
        std::cout << "Te veel knopen in het geheugen!" << std::endl;
        throw "Te veel knopen in het geheugen!";
    }
}

template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop()
: BKnoop<Sleutel, Data, m>{true}
{
}

template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop(blokindex linkerkind, const Sleutel& sl, const Data& d, blokindex rechterkind)
: BKnoop<Sleutel, Data, m>{false}
{
    sleutel[1] = sl;
    data[1] = d;
    index[0] = linkerkind;
    index[1] = rechterkind;
}

template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::~BKnoop()
{
    --crashtest;
}

template<class Sleutel, class Data, blokindex m>
Sleutel BKnoop<Sleutel, Data, m>::operator[](int index) const
{
    index++;

    if (index > aantal_kinderen())
    {
        throw "Index is larger than amount of keys";
    }

    if (index < 1)
    {
        throw "Index must be larger than 0";
    }

    return sleutel[index];
}

template<class Sleutel, class Data, blokindex m>
bool BKnoop<Sleutel, Data, m>::is_aanwezig(const Sleutel& nieuwe_sleutel) const
{
    int i = 1;

    while (i <= k)
    {
        if (sleutel[i] == nieuwe_sleutel)
        {
            return true;
        }

        i++;
    }

    return false;
}

template<class Sleutel, class Data, blokindex m>
Data BKnoop<Sleutel, Data, m>::geef_data(const Sleutel& sl) const
{
    if (!is_aanwezig(sl))
    {
        throw "Sleutel is niet aanwezig in deze knoop";
    }

    int i = 1;

    while (i <= k)
    {
        if (sleutel[i] == sl)
        {
            return data[i];
        }

        i++;
    }

    return 0;
}

template<class Sleutel, class Data, blokindex m>
void BKnoop<Sleutel, Data, m>::update_data(const Sleutel& sl, const Data& d)
{
    if (!is_aanwezig(sl))
    {
        throw "Sleutel is niet aanwezig in deze knoop";
    }

    int i = 1;

    while (i <= k)
    {
        if (sleutel[i] == sl)
        {
            data[i] = d;
        }

        i++;
    }
}

template<class Sleutel, class Data, blokindex m>
bool BKnoop<Sleutel, Data, m>::is_vol() const
{
    return k == m;
}

template<class Sleutel, class Data, blokindex m>
bool BKnoop<Sleutel, Data, m>::is_blad() const
{
    return is_blad_knoop;
}

template<class Sleutel, class Data, blokindex m>
void BKnoop<Sleutel, Data, m>::voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data, blokindex nieuw_rechterkind)
{
    if (is_aanwezig(nieuwe_sleutel))
    {
        throw "Sleutel is al aanwezig in deze knoop";
    }

    if (is_vol())
    {
        throw "Knoop zit al vol";
    }

    int i = k;

    while (i > 0 && nieuwe_sleutel < sleutel[i])
    {
        sleutel[i + 1] = sleutel[i];
        data[i + 1] = data[i];
        index[i + 1] = index[i];

        i--;
    }
    i++;

    sleutel[i] = nieuwe_sleutel;
    data[i] = nieuwe_data;
    index[i] = nieuw_rechterkind; // linkerkind bestaat altijd al
    k++;
}

template<class Sleutel, class Data, blokindex m>
blokindex BKnoop<Sleutel, Data, m>::geef_kindindex(const Sleutel& nieuwe_sleutel) const
{
    if (is_aanwezig(nieuwe_sleutel))
    {
        throw "Sleutel is al aanwezig in deze knoop";
    }

    int i = 1;
    while (i <= k && nieuwe_sleutel > sleutel[i])
    {
        i++;
    }
    i--;

    return index[i];
}

template<class Sleutel, class Data, blokindex m>
int BKnoop<Sleutel, Data, m>::aantal_kinderen() const
{
    return k;
}

template<class Sleutel, class Data, blokindex m>
std::string BKnoop<Sleutel, Data, m>::to_string() const
{
    std::stringstream out;

    out << "Knoop met " << k << "/" << m << " sleutels:" << std::endl;
    out << "Blad: " << (is_blad_knoop ? "Ja" : "Nee") << std::endl;
    out << "Sleutels: ";


    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        if (i <= k)
        {
            out << sleutel[i] << " ";
        }
        else
        {
            out << "X ";
        }
    }

    out << std::endl;

    return out.str();
}

#endif /* BKNOOP_H */
