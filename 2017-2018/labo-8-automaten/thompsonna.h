#ifndef THOMPSONNA_H
#define THOMPSONNA_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "regexp11.h"

const uchar epsilon = 0;
// ThompsonNA: met een beginstatenbit (index 0) en een eindstatenbit (index aantalStatenbits-1). Alleen gebruikt om
// algoritme van Thompson te implementeren

class Verbinding
{
public:
    Verbinding(int bron, int doel, uchar karakter);

    int bron;
    int doel;
    uchar karakter;
};

class ThompsonNA
{
public:
    ThompsonNA(const Regexp& re);

    int geefAantalVerbindingen() const;
    int geefAantalStatenbits() const;
    const Verbinding& operator[](int i) const;

    std::string to_string() const;

protected:
    // Haalt de verbindingen van ThompsonNA binnen, maar verschuift de knoopnrs;
    void haalbinnen(const ThompsonNA& TNA, int verschuiving);
    void verbind(int bron, int doel, uchar c);

    int aantalStatenbits;
    std::vector<Verbinding> overgang;
};

Verbinding::Verbinding(int bron, int doel, uchar karakter) : bron{bron}, doel{doel}, karakter{karakter}
{
}

int ThompsonNA::geefAantalVerbindingen() const
{
    return overgang.size();
}

int ThompsonNA::geefAantalStatenbits() const
{
    return aantalStatenbits;
}

const Verbinding& ThompsonNA::operator[](int i) const
{
    return overgang[i];
}

void ThompsonNA::haalbinnen(const ThompsonNA& TNA, int verschuiving)
{
    for (int i = 0; i < TNA.overgang.size(); i++)
    {
        const Verbinding& oud = TNA.overgang[i];
        overgang.push_back(Verbinding(oud.bron + verschuiving, oud.doel + verschuiving, oud.karakter));
    }
}
void ThompsonNA::verbind(int bron, int doel, uchar c = epsilon)
{
    overgang.push_back(Verbinding(bron, doel, c));
}

ThompsonNA::ThompsonNA(const Regexp& re)
{
    switch (re.geefOpcode())
    {
        case Regexp::letter:
            aantalStatenbits = 2;
            verbind(0, 1, re.geefLetter());
            break;
        case Regexp::plus:
        {
            ThompsonNA op1(*re.geefEersteOperand());
            ThompsonNA op2(*re.geefTweedeOperand());
            // beginknoop tweede operand overlapt met eindknoop eerste
            aantalStatenbits = op1.aantalStatenbits + op2.aantalStatenbits - 1;
            haalbinnen(op1, 0);
            haalbinnen(op2, op1.aantalStatenbits - 1);
        }
        break;
        case Regexp::of:
        {
            ThompsonNA op1(*re.geefEersteOperand());
            ThompsonNA op2(*re.geefTweedeOperand());
            aantalStatenbits = op1.aantalStatenbits + op2.aantalStatenbits + 2;
            haalbinnen(op1, 1);
            haalbinnen(op2, 1 + op1.aantalStatenbits);
            verbind(0, 1);
            verbind(0, op1.aantalStatenbits + 1);
            verbind(op1.aantalStatenbits, aantalStatenbits - 1);
            verbind(aantalStatenbits - 2, aantalStatenbits - 1);
        }
        break;
        case Regexp::ster:
        {
            ThompsonNA op1(*re.geefEersteOperand());
            aantalStatenbits = op1.aantalStatenbits + 2;
            haalbinnen(op1, 1);
            verbind(0, 1);
            verbind(0, aantalStatenbits - 1);
            verbind(aantalStatenbits - 2, aantalStatenbits - 1);
            verbind(aantalStatenbits - 2, 1);
        }
    }
}

std::string ThompsonNA::to_string() const
{
    std::stringstream out;

    out << "digraph {" << std::endl;
    out << "\t rankdir=\"LR\";" << std::endl;
    out << "\t -1 [shape=point];" << std::endl;
    out << "\t -1 -> 0;" << std::endl;
    out << "\t " << (geefAantalStatenbits() - 1) << " [shape=square];" << std::endl;

    for (int i = 0; i < overgang.size(); i++)
    {
        const Verbinding& v = overgang[i];

        out << "\t " << v.bron << " -> " << v.doel << " [label=\"";
        if (v.karakter == epsilon)
        {
            out << "epsilon";
        }
        else
        {
            out << ((char) v.karakter);
        }
        out << "\"];" << std::endl;
    }

    out << "}" << std::endl;

    return out.str();
}

#endif
