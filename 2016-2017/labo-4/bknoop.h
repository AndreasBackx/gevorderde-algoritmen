
#ifndef BKNOOP_H
#define BKNOOP_H

#include "schijf.h"

#include <string>
#include <sstream>

// Betekenis m: zie cursus

template<class Sleutel, class Data, blokindex m>
class BKnoop
{
public:

    BKnoop<Sleutel, Data, m>(bool is_blad);
    ~BKnoop<Sleutel, Data, m>();

    BKnoop<Sleutel, Data, m>(const BKnoop<Sleutel, Data, m>& other) = default;
    BKnoop<Sleutel, Data, m>& operator=(const BKnoop<Sleutel, Data, m>& other) = default;

    BKnoop<Sleutel, Data, m>(BKnoop<Sleutel, Data, m>&& other) = default;
    BKnoop<Sleutel, Data, m>& operator=(BKnoop<Sleutel, Data, m>&& other) = default;

    bool is_aanwezig(const Sleutel& nieuwe_sleutel) const;
    Data geef_data(const Sleutel& nieuwe_sleutel); // Geen return by reference, want schijfpagina wordt niet in het geheugen gehouden (max 3)
    void voegtoe(const Sleutel& nieuwe_sleutel, const Data& nieuwe_data, blokindex nieuw_rechterkind);
    bool is_vol() const;
    blokindex geef_kindindex(const Sleutel& nieuwe_sleutel) const;
    int aantal_kinderen() const;


    Sleutel operator[] (const int index) const;
    std::string to_string() const;

private:

    // Opmerking: sleutel[0] en data[0] worden niet gebruikt, zodat indexen vanaf 1 lopen
    //
    // m = 4
    // ARRAY_SIZE = 5
    // sleutel[]   (0)   1   2   3   4
    // data[]      (0)   1   2   3   4
    // index[]         0   1   2   3   4
    const static int ARRAY_SIZE = m + 1;
    Sleutel sleutel[ARRAY_SIZE];
    Data data[ARRAY_SIZE];
    blokindex index[ARRAY_SIZE];
    int k;
    bool is_blad;
    

    // Crashtest voorkomt dat er meer dan drie knopen in het 'geheugen' zitten (excusief de wortel).
    // Dit veronderstelt wel dat er nooit knopen op de Schijf gedeletete worden.
    static int crashtest;
};

template<class Sleutel, class Data, blokindex m>
int BKnoop<Sleutel, Data, m>::crashtest = 0;

template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::BKnoop(bool is_blad)
: k{0}, is_blad{is_blad}
{
    crashtest++;

    // Dit crasht als er een 4e knoop wordt aangemaakt
    // Zie de limiet van 3 schijfknopen in het intern (RAM) geheugen in de opgave
    int crashresult = 4 / (-4 + crashtest);
}

template<class Sleutel, class Data, blokindex m>
BKnoop<Sleutel, Data, m>::~BKnoop()
{
    --crashtest;
}

template<class Sleutel, class Data, blokindex m>
Sleutel BKnoop<Sleutel, Data, m>::operator[] (const int index) const
{
    if(index > aantal_kinderen())
    {
        throw "Index is larger than amount of keys";
    }

    if (index <= 0)
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
Data BKnoop<Sleutel, Data, m>::geef_data(const Sleutel& sl)
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
bool BKnoop<Sleutel, Data, m>::is_vol() const
{
    return k == m;
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
    while (i < ARRAY_SIZE && nieuwe_sleutel < sleutel[i])
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
    out << "Blad: " << (is_blad ? "Ja" : "Nee") << std::endl;
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
