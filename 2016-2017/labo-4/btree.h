#ifndef BTREE_H
#define BTREE_H

#include "schijf.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream> // DEBUG
#include <iomanip> // DEBUG
#include <stack>
#include <cmath>

template<class T, class D, unsigned int m>
class Btree;

template<class T, class D, unsigned int m>
class Bknoop
{
private:

    friend class Btree<T, D, m>;

    static int crashtest;

public:

    Bknoop();
    const Bknoop<T, D, m>& operator=(const Bknoop<T, D, m>& b);
    bool is_vol() const;
    void voeg_toe(const T& sleutel, const D& data, const blokindex& voegtoe_index = 0);

    friend std::ostream& operator<<(std::ostream& out, const Bknoop<T, D, m>& bknoop)
    {
        out << "k: " << bknoop.k << std::endl;
        if (bknoop.isblad)
        {
            out << "is blad";
        }
        else
        {
            out << "is geen blad";
        }
        out << std::endl << "Sleutels: ";
        for (int i = 0; i < bknoop.k; i++)
//        for (const T& s : bknoop.sleutel)
        {
            out << bknoop.sleutel[i] << ", ";
        }
        out << std::endl << "Data: ";
        for (int i = 0; i < bknoop.k; i++)
        {
            out << bknoop.data[i] << ", ";
        }
        out << std::endl << "Kinderen: ";
        for (int i = 0; i <= bknoop.k; i++)
        {
            out << bknoop.index[i] << ", ";
        }
        return out;
    }

    T sleutel[m];
    D data[m];
    blokindex index[m + 1]; // schijfadres: "pointer" naar kinderknopen
    unsigned int k;
    bool isblad;
};

template<class T, class D, unsigned int m>
class Btree
{
private:

    typedef Bknoop<T, D, m> Knoop;

    Schijf<Knoop>& schijf;
    Knoop wortel;
    blokindex wortelindex;

public:

    Btree(Schijf<Knoop>& s);
    void voeg_toe(const T& sleutel, const D& data);
    //    D& zoek(const T& sleutel) const;

    friend std::ostream& operator<<(std::ostream& out, const Btree<T, D, m>& btree)
    {
        out << "Btree with m = " << m << std::left << std::setfill('-') << std::setw(80) << std::endl
            << btree.wortelindex << std::endl
            << "Wortel:" << std::endl
            << btree.wortel << std::endl
            << std::endl;

        std::stack<blokindex> te_verwerken_knopen;
        te_verwerken_knopen.push(btree.wortelindex);

        while (!te_verwerken_knopen.empty())
        {
            Knoop huidige_knoop{};
            blokindex huidige_knoop_index = te_verwerken_knopen.top();
            btree.schijf.lees(huidige_knoop, huidige_knoop_index);
            te_verwerken_knopen.pop();

            if (!huidige_knoop.isblad)
            {
                for (int i = 0; i < huidige_knoop.k + 1; i++)
                {
                    te_verwerken_knopen.push(huidige_knoop.index[i]);
                }
            }

            out << "Huidige knoop met index " << huidige_knoop_index << std::endl
                << huidige_knoop << std::endl
                << std::endl;
        }

        return out;
    }
};

/******************************************************************************/

template<class T, class D, unsigned int m>
int Bknoop<T, D, m>::crashtest = 0;

/******************************************************************************/

template<class T, class D, unsigned int m>
Bknoop<T, D, m>::Bknoop() : k{0}, sleutel{0}, data{0}, isblad{false}, index{0}
{
    static_cast<void> (4 / (-4 + crashtest++));
};

template<class T, class D, unsigned int m>
const Bknoop<T, D, m>& Bknoop<T, D, m>::operator=(const Bknoop<T, D, m>& b)
{
    k = b.k;
    isblad = b.isblad;

    for (unsigned int i = 0; i < k; i++)
    {
        sleutel[i] = b.sleutel[i];
        data[i] = b.data[i];
    }

    if (!isblad)
    {
        for (unsigned int i = 0; i <= k; i++)
        {
            index[i] = b.index[i];
        }
    }

    return *this;
}

template<class T, class D, unsigned int m>
bool Bknoop<T, D, m>::is_vol() const
{
    return k >= m;
    /* k = amount of key-values in this node
     * m = amount of children this node may have
     * Before splitting we need to store k == m keys (one too many) to calculate the middle index and facilitate the split
     */
}

template<class T, class D, unsigned int m>
void Bknoop<T, D, m>::voeg_toe(const T& voegtoe_sleutel, const D& voegtoe_data, const blokindex& voegtoe_index)
{
    if (is_vol())
    {
        throw "De knoop zit al vol.";
    }

    int i = k - 1;
    while (i >= 0 && voegtoe_sleutel < sleutel[i])
    {
        data[i + 1] = data[i];
        sleutel[i + 1] = sleutel[i];
        index[i + 2] = index[i + 1];
        i--;
    }
    sleutel[i + 1] = voegtoe_sleutel;
    data[i + 1] = voegtoe_data;
    index[i + 2] = voegtoe_index;
    k++;
}

/******************************************************************************/

template<class T, class D, unsigned int m>
Btree<T, D, m>::Btree(Schijf<Knoop>& s) : schijf{s}
{
    wortel.k = 0;
    wortel.isblad = true;
    wortelindex = schijf.schrijf(wortel);
}

template<class T, class D, unsigned int m>
void Btree<T, D, m>::voeg_toe(const T& sleutel, const D& data)
{
    Knoop huidige_knoop = wortel;
    std::stack<blokindex> pad_blokindexen;
    pad_blokindexen.push(wortelindex);

    while (!huidige_knoop.isblad)
    {
        int i = 0;
        while (i < huidige_knoop.k && sleutel > huidige_knoop.sleutel[i])
        {
            i++;
        }
        schijf.lees(huidige_knoop, huidige_knoop.index[i]);
        pad_blokindexen.push(huidige_knoop.index[i]);
    }  
    
    huidige_knoop.voeg_toe(sleutel, data);
    
    blokindex huidige_knoop_blokindex = pad_blokindexen.top();
    schijf.herschrijf(huidige_knoop, huidige_knoop_blokindex);  
    if (huidige_knoop_blokindex == wortelindex) {
        wortel = huidige_knoop;
    }

    while (huidige_knoop.is_vol() && !pad_blokindexen.empty())
    {
        int midden_index =
            static_cast<int> (
                std::ceil(
                    static_cast<double> (huidige_knoop.k)
                    / static_cast<double> (2)
                )
            ) - 1; // -1 because counting from 0

        Knoop nieuwe_knoop{};
        nieuwe_knoop.isblad = huidige_knoop.isblad;

        for (int i = midden_index + 1; i < huidige_knoop.k; i++)
        {
            int j = i - (midden_index + 1);
            nieuwe_knoop.sleutel[j] = huidige_knoop.sleutel[i];
            nieuwe_knoop.data[j] = huidige_knoop.data[i];
            nieuwe_knoop.index[j] = huidige_knoop.index[i];

            // Not necessary, but easier for understanding and debugging
            huidige_knoop.sleutel[i] = 0;
            huidige_knoop.data[i] = 0;
            huidige_knoop.index[i] = 0;
        }

        nieuwe_knoop.k = huidige_knoop.k - (midden_index + 1);
        huidige_knoop.k = midden_index;

        blokindex nieuwe_knoop_blokindex = schijf.schrijf(nieuwe_knoop);
        blokindex huidige_knoop_blokindex = pad_blokindexen.top();
        pad_blokindexen.pop();

        Knoop ouder_knoop{};

        if (pad_blokindexen.empty()) // huidige_knoop_blokindex == wortelindex
        {
            ouder_knoop.index[0] = huidige_knoop_blokindex;

            ouder_knoop.voeg_toe(
                huidige_knoop.sleutel[midden_index],
                huidige_knoop.data[midden_index],
                nieuwe_knoop_blokindex
                );

            // Not necessary, but easier for understanding and debugging
            huidige_knoop.sleutel[midden_index] = 0;
            huidige_knoop.data[midden_index] = 0;
            
            schijf.herschrijf(huidige_knoop, huidige_knoop_blokindex);

            wortel = ouder_knoop;
            wortel.isblad = false;
            wortelindex = schijf.schrijf(ouder_knoop);
        }
        else
        {
            blokindex ouder_knoop_blokindex = pad_blokindexen.top();

            schijf.lees(ouder_knoop, ouder_knoop_blokindex);

            ouder_knoop.voeg_toe(
                huidige_knoop.sleutel[midden_index],
                huidige_knoop.data[midden_index],
                nieuwe_knoop_blokindex
                );

            // Not necessary, but easier for understanding and debugging
            huidige_knoop.sleutel[midden_index] = 0;
            huidige_knoop.data[midden_index] = 0;

            schijf.herschrijf(huidige_knoop, huidige_knoop_blokindex);
            
            huidige_knoop = ouder_knoop;
        }
    }
}

#endif
