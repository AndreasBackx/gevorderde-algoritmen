
// Labo 6 2016-2017 heeft een gerefactorde versie voor stroomnetwerken

#ifndef STROOMNETWERK_H
#define STROOMNETWERK_H

#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "graaf.h"

template <class T>
class Pad : public std::vector<int>
{
public:
    T geef_capaciteit() const;
    void set_capaciteit(const T& capaciteit);
    std::string to_string() const;

protected:
    T capaciteit;
};

template <class T>
T Pad<T>::geef_capaciteit() const
{
    return capaciteit;
}

template <class T>
void Pad<T>::set_capaciteit(const T& capaciteit)
{
    this->capaciteit = capaciteit;
}

template <class T>
std::string Pad<T>::to_string() const
{
    std::stringstream out;

    out << "Capaciteit: " << capaciteit << ", ";
    if (!empty())
    {
        out << (*this)[0];
        for (const T& k : (*this))
        {
            out << ", " << k;
        }
    }
    else
    {
        out << "leeg";
    }

    return out.str();
}

/**********************************************************************

   Class:Vergrotendpadzoeker

   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 en vergrotend pad teruggeeft.
                 van en verbruiker zijn de knoopnummers van bron en doel.


***************************************************************************/
template <class T>
class Vergrotendpadzoeker
{
public:
    Vergrotendpadzoeker(const GraafMetTakdata<GERICHT, T>& stroomnetwerk, int producent, int verbruiker, Pad<T>& pad);

protected:
    virtual void zoek_pad_vanuit_knoop(int knoopnr, int diepte);

    const GraafMetTakdata<GERICHT, T>& stroomnetwerk;
    Pad<T>& pad;
    int producent;
    int verbruiker;
    std::vector<int> voorlopers;
    std::vector<bool> is_knoop_bezocht;
};

template <class T>
Vergrotendpadzoeker<T>::Vergrotendpadzoeker(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                            int producent,
                                            int verbruiker,
                                            Pad<T>& pad)
    : stroomnetwerk{stroomnetwerk},
      pad{pad},
      producent{producent},
      verbruiker{verbruiker},
      voorlopers(stroomnetwerk.aantalKnopen()),
      is_knoop_bezocht(stroomnetwerk.aantalKnopen(), false)
{
    assert(producent != verbruiker);

    pad.clear();
    zoek_pad_vanuit_knoop(producent, 0);

    if (pad.size() > 1)
    {
        T min_capaciteit = *stroomnetwerk.geefTakdata(pad[0], pad[1]);

        assert(pad.size() < std::numeric_limits<int>::max());
        for (int i = 2; i < static_cast<int>(pad.size()); i++)
        {
            T capaciteit = *stroomnetwerk.geefTakdata(pad[i - 1], pad[i]);
            if (capaciteit < min_capaciteit)
            {
                min_capaciteit = capaciteit;
            }
        }
        pad.set_capaciteit(min_capaciteit);
    }
}

// Deze kortste pad zoeker is een niet-ideale zoeker. Het zoekt een lang pad, zodat er een grotere kans is dat eventuele
// fouten op terugverbindingen bij het restnetwerk bovenkomen.
template <class T>
void Vergrotendpadzoeker<T>::zoek_pad_vanuit_knoop(int knoopnr, int diepte)
{
    is_knoop_bezocht[knoopnr] = true;
    for (const auto& it : stroomnetwerk[knoopnr])
    {
        int buurnr = it.first;
        int lengte_huidig_pad = (diepte + 2); // producent + tussenliggende knopen + verbruiker

        if (*stroomnetwerk.geefTakdata(knoopnr, buurnr) > 0)
        {
            assert(pad.size() < std::numeric_limits<int>::max());
            if ((buurnr == verbruiker) && (lengte_huidig_pad >= static_cast<int>(pad.size())))
            // verbruiker gevonden en huidige pad is langer?
            {
                voorlopers[verbruiker] = knoopnr;
                pad.resize(lengte_huidig_pad);
                int nr = verbruiker;
                int i = (diepte + 1);
                while (nr != producent)
                {
                    pad[i] = nr;
                    i--;
                    nr = voorlopers[nr];
                }
                pad[0] = nr;
            }
            else if (buurnr != verbruiker
                     && !is_knoop_bezocht[buurnr]) // buurnr != verbruiker wegens kans op oneindige lussen
            {
                voorlopers[buurnr] = knoopnr;
                zoek_pad_vanuit_knoop(buurnr, (diepte + 1));
            }
        }
    }
}

/**********************************************************************

   Class: Stroomnetwerk

   beschrijving: Een stroomnetwerk gaat uit van een gewogen gerichte graaf
                 die in de constructor wordt opgegeven

***************************************************************************/

template <class T> // T = takdata
class Stroomnetwerk : public GraafMetTakdata<GERICHT, T>
{
public:
    // Graaf<GERICHT>(gr) moet toegevoegd, anders roept de copyconstructor van GraafMetTakdata de defaultconstructor van
    // Graaf op en krijgen we een lege graaf.
    Stroomnetwerk<T>(const GraafMetTakdata<GERICHT, T>& graaf);
    Stroomnetwerk<T>(const GraafMetTakdata<GERICHT, T>& graaf, int producer, int verbruiker);

    Stroomnetwerk<T>() = delete;
    virtual ~Stroomnetwerk<T>() = default;

    Stroomnetwerk<T>(const Stroomnetwerk<T>& ander) = default;
    Stroomnetwerk<T>& operator=(const Stroomnetwerk<T>& ander) = default;

    Stroomnetwerk<T>(Stroomnetwerk<T>&& ander) = default;
    Stroomnetwerk<T>& operator=(Stroomnetwerk<T>&& ander) = default;

    Stroomnetwerk<T>& operator+=(const Pad<T>& vergrotendpad);
    Stroomnetwerk<T>& operator-=(const Pad<T>& vergrotendpad);

    std::string genereer_dot_code() const;

protected:
    int producent;
    int verbruiker;
};

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& graaf)
    : Graaf<GERICHT>{graaf}, GraafMetTakdata<GERICHT, T>{graaf}
{
}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& graaf, int producent, int verbruiker)
    : Graaf<GERICHT>{graaf.aantalKnopen()}, producent{producent}, verbruiker{verbruiker}
{
    Stroomnetwerk<T> restnetwerk{graaf};

    Pad<T> vergrotendpad;
    Vergrotendpadzoeker<T> vg{restnetwerk, producent, verbruiker, vergrotendpad};
    while (vergrotendpad.size() > 0)
    {
        // += en -= hebben niets met elkaar te maken. Het een is voor het stroomnetwerk (enkel aanpassen takdata), het
        // ander voor het restnetwerk (aanpassen takdata en verbindingen). Je kan eventueel ook andere functienamen
        // gebruiken zoals updateStroomnetwerk en updateRestnetwerk
        restnetwerk -= vergrotendpad;
        *this += vergrotendpad;
        Vergrotendpadzoeker<T> vg{restnetwerk, producent, verbruiker, vergrotendpad};
    }
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator+=(const Pad<T>& vergrotendpad)
{
    if (!vergrotendpad.empty())
    {
        assert(vergrotendpad.size() < std::numeric_limits<int>::max());
        for (int i = 1; i < static_cast<int>(vergrotendpad.size()); i++)
        {
            int van = vergrotendpad[i - 1];
            int naar = vergrotendpad[i];

            if (this->verbindingsnummer(van, naar) == -1)
            {
                this->voegVerbindingToe(van, naar, vergrotendpad.geef_capaciteit());
            }
            else
            {
                T* stroom = this->geefTakdata(van, naar);
                *stroom += vergrotendpad.geef_capaciteit();
            }
        }
    }

    return (*this);
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator-=(const Pad<T>& vergrotendpad)
{
    if (!vergrotendpad.empty())
    {
        assert(vergrotendpad.size() < std::numeric_limits<int>::max());
        for (int i = 1; i < static_cast<int>(vergrotendpad.size()); i++)
        {
            int van = vergrotendpad[i - 1];
            int naar = vergrotendpad[i];

            assert(this->verbindingsnummer(van, naar) != -1);

            T* stroom = this->geefTakdata(van, naar);
            *stroom -= vergrotendpad.geef_capaciteit();

            if (*stroom == 0)
            {
                this->verwijderVerbinding(van, naar);
            }

            if (this->verbindingsnummer(naar, van) == -1)
            {
                this->voegVerbindingToe(naar, van, vergrotendpad.geef_capaciteit());
            }
            else
            {
                T* stroom = this->geefTakdata(naar, van);
                *stroom += vergrotendpad.geef_capaciteit();
            }
        }
    }

    return (*this);
}

template <class T>
std::string Stroomnetwerk<T>::genereer_dot_code() const
{
    std::stringstream out;

    out << "digraph stroomnetwerk {" << std::endl;

    for (int i = 0; i < this->aantalKnopen(); i++)
    {
        const auto& knoop = (*this)[i];

        for (const auto& it : knoop)
        {
            out << "\t" << i << " -> " << it.first << " [label=\"" << *(this->geefTakdata(i, it.first)) << "\"];"
                << std::endl;
        }
    }

    out << "}" << std::endl;

    return out.str();
}
#endif

// Originele code:
//
// template <class T>
// class Vergrotendpadzoeker
//{
// public:
//
//    Vergrotendpadzoeker(const GraafMetTakdata<GERICHT, T>& _gr, int _van, int _naar, Pad<T>& _pad)
//    : pad(_pad), gr(_gr), van(_van), naar(_naar), voorganger(gr.aantalKnopen()), bezocht(gr.aantalKnopen(), false)
//    {
//        pad.clear();
//        verwerk(van, 0);
//
//        if (pad.size() > 0)
//        {
//            T capaciteit = *gr.geefTakdata(pad[0], pad[1]);
//            for (int i = 2; i < pad.size(); i++)
//            {
//                T nucapaciteit = *gr.geefTakdata(pad[i - 1], pad[i]);
//                if (nucapaciteit < capaciteit)
//                    capaciteit = nucapaciteit;
//                assert(capaciteit > 0);
//            }
//            pad.zetCapaciteit(capaciteit);
//        }
//    }
//
//    void verwerk(int knoopnr, int diepte)
//    {
//        //    std::cerr <<" knoopnr "<<knoopnr  <<" <? "<< gr.aantalKnopen()<<"\n";
//        assert(knoopnr < gr.aantalKnopen());
//        bezocht[knoopnr] = true;
//        const typename GraafMetTakdata<GERICHT, T>::Knoop& kn = gr[knoopnr];
//        int nudiepte = diepte + 1;
//        for (typename GraafMetTakdata<GERICHT, T>::Knoop::const_iterator it = kn.begin(); it != kn.end(); it++)
//        {
//            int kind = it->first;
//            if (*gr.geefTakdata(knoopnr, kind) > 0)
//            {
//                if (it->first == naar && nudiepte + 1 = > pad.size())
//                {
//                    voorganger[naar] = knoopnr;
//                    pad.resize(nudiepte + 1);
//                    int nunr = naar;
//                    int i = nudiepte;
//                    while (nunr != van)
//                    {
//                        pad[i--] = nunr;
//                        nunr = voorganger[nunr];
//                    }
//                    assert(i == 0);
//                    assert(nunr == van);
//                    pad[0] = nunr;
//                }
//                else if (it->first != naar && !bezocht[kind])
//                {
//                    assert(*gr.geefTakdata(knoopnr, kind) > 0);
//                    voorganger[kind] = knoopnr;
//                    verwerk(kind, nudiepte);
//                }
//            }//if takdata> 0
//        }//for
//    }
//
//    const GraafMetTakdata<GERICHT, T>& gr;
//    Pad<T>& pad;
//    vector<int> voorganger;
//    vector<bool> bezocht;
//    int van, naar;
//};

// template < RichtType RT, class Takdata >
// void GraafMetTakdata< RT, Takdata >::
// wordMaxStroomVan(const GraafMetTakdata< RT, Takdata >& netwerk, int producent, int verbruiker)
//{
//    if (!isGericht())
//        throw GraafExceptie("Ongerichte grafen zijn geen stroomnetwerken.");
//    zetAantalKnopen(netwerk.aantalKnopen());
//    GraafMetTakdata< GERICHT, Takdata> restnetwerk = netwerk;
//    Pad< Takdata> vergrotendPad;
//
//    restnetwerk.geefVergrotendPad(vergrotendPad, producent, verbruiker);
//    while (vergrotendPad.size() != 0)
//    {
//        restnetwerk -= vergrotendPad;
//        *this += vergrotendPad;
//        restnetwerk.geefVergrotendPad(vergrotendPad, producent, verbruiker);
//    }
//}
