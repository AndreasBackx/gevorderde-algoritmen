
#ifndef BKNOOP_H
#define BKNOOP_H

#include "../schijf/schijf.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Betekenis m: zie cursus
// TODO BKnoop opslitsen in klasse Blad en NietBlad die elk een interface overerven
//

template <class Sleutel, class Data, blokindex m>
class BKnoop
{

public:
    BKnoop(bool is_blad_knoop);
    BKnoop();
    BKnoop(blokindex linkerkind);
    BKnoop(blokindex linkerkind, const Sleutel& sl, const Data& d, blokindex rechterkind);
    ~BKnoop();

    BKnoop(const BKnoop<Sleutel, Data, m>& other) = default;
    BKnoop& operator=(const BKnoop<Sleutel, Data, m>& other) = default;

    BKnoop(BKnoop<Sleutel, Data, m>&& other) = default;
    BKnoop& operator=(BKnoop<Sleutel, Data, m>&& other) = default;

    bool is_aanwezig(const Sleutel& nieuwe_sleutel) const;
    bool is_blad() const;
    Data geef_data(const Sleutel& nieuwe_sleutel)
            const; // Geen return by reference, want schijfpagina wordt niet in het geheugen gehouden (max 3)
    void update_data(const Sleutel& sl, const Data& d);
    void voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data);
    void voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data, blokindex nieuw_rechterkind);
    bool is_vol() const;
    blokindex geef_kindindex(const Sleutel& nieuwe_sleutel) const;
    int aantal_kinderen() const;
    void beperk_kinderen(int aantal_kinderen);
    void splits_knoop(BKnoop<Sleutel, Data, m>& nieuwe_broer_knoop, Sleutel& middel_sleutel, Data& middel_data);
    std::vector<blokindex> geef_alle_kindindexen();

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

template <class Sleutel, class Data, blokindex m>
int BKnoop<Sleutel, Data, m>::crashtest = 0;

template <class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop(bool is_blad_knoop) : k{0}, is_blad_knoop{is_blad_knoop}
{
    crashtest++;

    // // Dit crasht als er een 4e knoop wordt aangemaakt
    // // Zie de limiet van 3 schijfknopen in het intern (RAM) geheugen in de opgave
    // int crashresult = 4 / (-4 + crashtest);

    if (crashtest > 3)
    {
        std::cout << "Te veel knopen in het geheugen!" << std::endl;
        assert(crashtest > 3);
    }
    //    std::cout << crashtest << std::endl;
}

template <class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop() : BKnoop<Sleutel, Data, m>{true}
{
}

template <class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop(blokindex linkerkind) : BKnoop<Sleutel, Data, m>{false}
{
    index[0] = linkerkind;
}

template <class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop(blokindex linkerkind, const Sleutel& sl, const Data& d, blokindex rechterkind)
: BKnoop<Sleutel, Data, m>{false}
{
    sleutel[1] = sl;
    data[1] = d;
    index[0] = linkerkind;
    index[1] = rechterkind;
}

template <class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::~BKnoop()
{
    --crashtest;
}

template <class Sleutel, class Data, blokindex m>
Sleutel BKnoop<Sleutel, Data, m>::operator[](int index) const
{
    index++;

    if (index > aantal_kinderen())
    {
        throw "Index is groter dan het aantal sleutels";
    }

    if (index < 1)
    {
        throw "Index moet groter zijn dan 0";
    }

    return sleutel[index];
}

template <class Sleutel, class Data, blokindex m>
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

template <class Sleutel, class Data, blokindex m>
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

template <class Sleutel, class Data, blokindex m>
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

template <class Sleutel, class Data, blokindex m>
bool BKnoop<Sleutel, Data, m>::is_vol() const
{
    return k == m;
}

template <class Sleutel, class Data, blokindex m>
bool BKnoop<Sleutel, Data, m>::is_blad() const
{
    return is_blad_knoop;
}

template <class Sleutel, class Data, blokindex m>
void BKnoop<Sleutel, Data, m>::voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data)
{
    voegtoe(nieuwe_sleutel, nieuwe_data, Schijf<BKnoop<Sleutel, Data, m>>::NULL_BLOKINDEX);
}

template <class Sleutel, class Data, blokindex m>
void BKnoop<Sleutel, Data, m>::voegtoe(const Sleutel& nieuwe_sleutel,
                                       const Data& nieuwe_data,
                                       blokindex nieuw_rechterkind)
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

template <class Sleutel, class Data, blokindex m>
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

template <class Sleutel, class Data, blokindex m>
int BKnoop<Sleutel, Data, m>::aantal_kinderen() const
{
    return k;
}

template <class Sleutel, class Data, blokindex m>
std::string BKnoop<Sleutel, Data, m>::to_string() const
{
    std::stringstream out;

    out << "Knoop met " << k << "/" << m << " sleutels (" << (is_blad_knoop ? "Blad" : "Nietblad") << "): ";

    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        if (i <= k)
        {
            out << sleutel[i] << "(" << data[i] << ") ";
        }
        else
        {
            out << "X ";
        }
    }

    return out.str();
}

template <class Sleutel, class Data, blokindex m>
void BKnoop<Sleutel, Data, m>::beperk_kinderen(int aantal_kinderen)
{
    k = aantal_kinderen;
}

template <class Sleutel, class Data, blokindex m>
void BKnoop<Sleutel, Data, m>::splits_knoop(BKnoop<Sleutel, Data, m>& nieuwe_broer_knoop,
                                            Sleutel& middel_sleutel,
                                            Data& middel_data)
{
    const int middel_pivot = static_cast<int>(std::ceil(k / 2));

    nieuwe_broer_knoop.is_blad_knoop = is_blad_knoop;

    middel_sleutel = sleutel[middel_pivot];
    middel_data = data[middel_pivot];

    if (!nieuwe_broer_knoop.is_blad_knoop)
    {
        nieuwe_broer_knoop.index[0] = index[middel_pivot];
    }

    for (int i = middel_pivot + 1; i <= k; i++)
    {
        nieuwe_broer_knoop.sleutel[i - middel_pivot] = sleutel[i];
        nieuwe_broer_knoop.data[i - middel_pivot] = data[i];

        if (!nieuwe_broer_knoop.is_blad_knoop)
        {
            nieuwe_broer_knoop.index[i - middel_pivot] = index[i];
        }
    }

    nieuwe_broer_knoop.k = k - middel_pivot;
    k = middel_pivot - 1;
}

template <class Sleutel, class Data, blokindex m>
std::vector<blokindex> BKnoop<Sleutel, Data, m>::geef_alle_kindindexen()
{
    std::vector<blokindex> indexen;

    for (int i = 0; i <= k; i++)
    {
        indexen.push_back(index[i]);
    }

    return indexen;
}

#endif /* BKNOOP_H */
