
#ifndef SHIFTAND_H
#define SHIFTAND_H

#include "bitpatroon.h"

#include <sstream>
#include <iomanip>
#include <queue>

typedef unsigned int uint;
typedef unsigned char uchar;

class ShiftAnd
{
public:
    //naaldlengte moet kleiner dan patroonlengte zijn,
    //anders wordt een exceptie opgegooid.
    ShiftAnd(const uchar* naald, uint _naaldlengte);
    std::queue<const uchar*> zoek_exact(const uchar* hooiberg, uint hooiberglengte, bool overlap = false);
    std::queue<const uchar*> zoek_1_vervanging(const uchar* hooiberg, uint hooiberglengte, bool overlap = false);
    std::string to_string() const;

    // puur om te debuggen
//    std::vector<std::pair<uchar, Bitpatroon>> debug_zonder_fouten;
//    std::vector<std::pair<uchar, Bitpatroon>> debug_1_vervanging;
//    std::string to_string_debug_zonder_fouten() const;
//    std::string to_string_debug_1_vervanging() const;

private:
    const static int ALFABET_GROOTTE = 256; // 2^8
    Bitpatroon letter_patroon[ALFABET_GROOTTE];
    const uchar* naald;
    uint naald_lengte;
};

ShiftAnd::ShiftAnd(const uchar* naald, uint naald_lengte) : naald{naald}, naald_lengte{naald_lengte}
{
    if (naald_lengte > patroonlengte)
    {
        throw "De naald lengte is te groot";
    }

    for (int i = 0; i < naald_lengte; i++)
    {
        letter_patroon[naald[i]] |= Bitpatroon::eenbit(i);
    }
}

std::queue<const uchar*> ShiftAnd::zoek_exact(const uchar* hooiberg, uint hooiberg_lengte, bool overlap)
{
    std::queue<const uchar*> gevonden;

    Bitpatroon huidig;

    for (int i = 0; i < hooiberg_lengte; i++)
    {
        huidig = huidig.shiftrechts(1);
        huidig |= Bitpatroon::eenbit(0);
        huidig &= letter_patroon[hooiberg[i]];

        if (huidig.en(Bitpatroon::eenbit(naald_lengte - 1)))
        {
            gevonden.push(hooiberg + i);

            if (!overlap)
            {
                huidig &= Bitpatroon();
            }
        }

//        debug_zonder_fouten.push_back(std::pair<uchar, Bitpatroon>(hooiberg[i], huidig));
    }

    return gevonden;
}

std::queue<const uchar*> ShiftAnd::zoek_1_vervanging(const uchar* hooiberg, uint hooiberg_lengte, bool overlap)
{
    std::queue<const uchar*> gevonden;

    Bitpatroon huidig_zonder_fout;
    Bitpatroon huidig_1_vervanging;
    Bitpatroon schuif_huidig_zonder_fout;

    for (int i = 0; i < hooiberg_lengte; i++)
    {
        // Schuif(R^0_j)
        schuif_huidig_zonder_fout = huidig_zonder_fout.shiftrechts(1);
        schuif_huidig_zonder_fout |= Bitpatroon::eenbit(0);

        // Schuif(R^1_j EN S[T[j + 1]]
        huidig_1_vervanging = huidig_1_vervanging.shiftrechts(1);
        huidig_1_vervanging |= Bitpatroon::eenbit(0);
        huidig_1_vervanging &= letter_patroon[hooiberg[i]];

        // Schuif(R^0_j) OR (Schuif(R^1_j EN S[T[j + 1]])
        huidig_1_vervanging |= schuif_huidig_zonder_fout;

        // Schuif(R^0_j EN S[T[j + 1]]
        huidig_zonder_fout = huidig_zonder_fout.shiftrechts(1);
        huidig_zonder_fout |= Bitpatroon::eenbit(0);
        huidig_zonder_fout &= letter_patroon[hooiberg[i]];

        if (huidig_1_vervanging.en(Bitpatroon::eenbit(naald_lengte - 1)))
        {
            gevonden.push(hooiberg + i);

            if (!overlap)
            {
                huidig_zonder_fout &= Bitpatroon();
                huidig_1_vervanging &= Bitpatroon();
                schuif_huidig_zonder_fout &= Bitpatroon();
            }
        }

//        debug_zonder_fouten.push_back(std::pair<uchar, Bitpatroon>(hooiberg[i], huidig_zonder_fout));
//        debug_1_vervanging.push_back(std::pair<uchar, Bitpatroon>(hooiberg[i], huidig_1_vervanging));
    }

    return gevonden;
}

//std::string ShiftAnd::to_string_debug_zonder_fouten() const
//{
//    std::stringstream out;
//
//    out << "Debug zonder fouten:" << std::endl
//        << std::setw(6) << "";
//    for (int i = 0; i < naald_lengte; i++)
//    {
//        out << naald[i];
//    }
//    out << std::endl;
//
//    for (std::pair<uchar, Bitpatroon> debug : debug_zonder_fouten)
//    {
//        out << std::setw(6) << std::left << static_cast<char> (debug.first) << debug.second << std::endl;
//    }
//
//    return out.str();
//}
//
//std::string ShiftAnd::to_string_debug_1_vervanging() const
//{
//    std::stringstream out;
//
//    out << "Debug 1 vervanging:" << std::endl
//        << std::setw(6) << "";
//    for (int i = 0; i < naald_lengte; i++)
//    {
//        out << naald[i];
//    }
//    out << std::endl;
//
//    for (std::pair<uchar, Bitpatroon> debug : debug_1_vervanging)
//    {
//        out << std::setw(6) << std::left << static_cast<char> (debug.first) << debug.second << std::endl;
//    }
//
//    return out.str();
//}

std::string ShiftAnd::to_string() const
{
    const int LOWER_READABLE_BOUND = 32;
    const int UPPER_READABLE_BOUND = 126;

    std::stringstream out;

    out << "Naald: ";
    for (int i = 0; i < naald_lengte; i++)
    {
        out << naald[i];
    }
    out << std::endl
        << "Naaldlengte:" << naald_lengte << std::endl
        << std::endl;

    out << std::setw(6) << "";
    for (int i = 0; i < naald_lengte; i++)
    {
        out << static_cast<char> (naald[i]);
    }
    out << std::endl;

    for (int i = LOWER_READABLE_BOUND; i <= UPPER_READABLE_BOUND; i++)
    {
        out << std::setw(6) << std::left << static_cast<char> (i) << letter_patroon[i] << std::endl;
    }

    return out.str();
}

#endif