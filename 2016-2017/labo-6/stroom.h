
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

#include "graaf.h"

template<class T>
// Template (bv int vs double)
// Een Pad is een reeks van knoopnummers.
class Pad : public std::vector<int>
{
public:

    Pad() : capaciteit{0}
    {
    }

    friend std::ostream& operator<<(std::ostream& out, const Pad<T>& p)
    {
        if (p.size() > 0)
        {
            out << p[0];

            for (int i = 1; i < p.size(); i++)
            {
                out << " -> " << p[i];
            }

            out << " ";
        }
        out << "(capaciteit = " << p.capaciteit << ")";

        return out;
    }

    T capaciteit;
};

class Stroomnetwerk
{
public:

    Stroomnetwerk(const GraafMetTakdata<GERICHT, int>& capaciteiten, int producent, int verbruiker);

    int BepaalMaximumFlow();
    Pad<int> BepaalVergrotendPad(const GraafMetTakdata<GERICHT, int>& rest);

private:

    Pad<int> MaakVergrotendPad(const GraafMetTakdata<GERICHT, int>& rest, const std::vector<int>& voorgangers);

    GraafMetTakdata<GERICHT, int> capaciteiten;
    int producent;
    int verbruiker;
};

Stroomnetwerk::Stroomnetwerk(const GraafMetTakdata<GERICHT, int>& capaciteiten, int producent, int verbruiker)
: capaciteiten{capaciteiten}, producent{producent}, verbruiker{verbruiker}
{
}

int Stroomnetwerk::BepaalMaximumFlow()
{
    int maximale_netwerkstroom = 0;
    GraafMetTakdata<GERICHT, int> rest{capaciteiten};

    Pad<int> vergrotend_pad = BepaalVergrotendPad(rest);

    while (!vergrotend_pad.empty())
    {
        std::cout << vergrotend_pad << std::endl;

        maximale_netwerkstroom += vergrotend_pad.capaciteit;

        for (int i = 1; i < vergrotend_pad.size(); i++)
        {
            const int *restcapaciteit = rest.geefTakdata(vergrotend_pad[i - 1], vergrotend_pad[i]);
            int verminderde_restcapaciteit = *restcapaciteit - vergrotend_pad.capaciteit;
            rest.verwijderVerbinding(vergrotend_pad[i - 1], vergrotend_pad[i]);

            if (verminderde_restcapaciteit > 0)
            {
                rest.voegVerbindingToe(vergrotend_pad[i - 1], vergrotend_pad[i], verminderde_restcapaciteit);
            }
        }

        // std::cout << rest << std::endl;

        vergrotend_pad = BepaalVergrotendPad(rest);
    }

    return maximale_netwerkstroom;
}

Pad<int> Stroomnetwerk::BepaalVergrotendPad(const GraafMetTakdata<GERICHT, int>& rest)
{
    std::queue<int> te_bezoeken;
    std::vector<bool> bezocht(rest.aantalKnopen());
    std::vector<int> voorgangers(rest.aantalKnopen(), -1);

    te_bezoeken.push(producent);

    bool verbruiker_gevonden = false;
    while (!te_bezoeken.empty() && !verbruiker_gevonden)
    {
        int huidige_knoop = te_bezoeken.front();
        te_bezoeken.pop();

        bezocht[huidige_knoop] = true;

        auto buur_iterator = rest[huidige_knoop].begin();
        while (buur_iterator != rest[huidige_knoop].end() && !verbruiker_gevonden)
        {
            if (!bezocht[buur_iterator->first])
            {
                te_bezoeken.push(buur_iterator->first);
                voorgangers[buur_iterator->first] = huidige_knoop;
            }

            if (buur_iterator->first == verbruiker)
            {
                verbruiker_gevonden = true;
            }

            buur_iterator++;
        }
    }

    Pad<int> vergrotend_pad;

    if (verbruiker_gevonden)
    {
        return vergrotend_pad = MaakVergrotendPad(rest, voorgangers);
    }

    return vergrotend_pad;
}

Pad<int> Stroomnetwerk::MaakVergrotendPad(const GraafMetTakdata<GERICHT, int>& rest, const std::vector<int>& voorgangers)
{
    Pad<int> vergrotend_pad;

    vergrotend_pad.capaciteit = std::numeric_limits<int>::max();
    int huidige_knoop = verbruiker;

    while (voorgangers[huidige_knoop] >= 0)
    {
        vergrotend_pad.push_back(huidige_knoop);

        const int* rest_capaciteit = rest.geefTakdata(voorgangers[huidige_knoop], huidige_knoop);
        if (*rest_capaciteit < vergrotend_pad.capaciteit)
        {
            vergrotend_pad.capaciteit = *rest_capaciteit;
        }

        huidige_knoop = voorgangers[huidige_knoop];
    }

    vergrotend_pad.push_back(producent);
    std::reverse(vergrotend_pad.begin(), vergrotend_pad.end());

    return vergrotend_pad;
}

//template <class T>
//class Vergrotendpadzoeker
//{
//public:
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

//template < RichtType RT, class Takdata >
//void GraafMetTakdata< RT, Takdata >::
//wordMaxStroomVan(const GraafMetTakdata< RT, Takdata >& netwerk, int producent, int verbruiker)
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
