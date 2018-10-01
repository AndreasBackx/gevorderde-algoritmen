
#ifndef RZBOOM_BU_H
#define RZBOOM_BU_H

#include "rzboom.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <sstream>
#include <stack>
#include <tuple>

template <class Sleutel, class Data>
class RZBoomBottomUp : public RZBoom<Sleutel, Data>
{
public:
    using RZBoom<Sleutel, Data>::RZBoom;

    void voeg_toe(const Sleutel& sleutel, const Data& data) override;
};

/******************************************************************************/

template <class Sleutel, class Data>
void RZBoomBottomUp<Sleutel, Data>::voeg_toe(const Sleutel& sleutel, const Data& data)
{
    auto [plaats, ouder] = this->zoek(sleutel);

    if (*plaats)
    {
        return;
    }

    *plaats = RZBoom<Sleutel, Data>{sleutel, data, Kleur::ROOD};
    (*plaats)->ouder = ouder;

    while (plaats && (*plaats) && (*plaats)->ouder // Voor de zekerheid
           && (*plaats)->ouder->kleur == Kleur::ROOD
           && (*plaats)->ouder->ouder) // We itereren zolang er een grootouder is (eigenlijk onnodige check: als de
    // ouder rood is, heeft die sowieso een grootouder)
    {
        auto [grootouder, ouder, nonkel] = this->geef_familie(plaats);

        if (nonkel->geef_kleur() == Kleur::ROOD) // Geef_kleur() ipv ...->kleur voor als nonkel == nullptr
        {
            (*ouder)->kleur = Kleur::ZWART;
            (*nonkel)->kleur = Kleur::ZWART;
            (*grootouder)->kleur = Kleur::ROOD;
            plaats = grootouder;
        }
        else if (nonkel->geef_kleur() == Kleur::ZWART)
        {
            Richting grootouder_ouder_richting = (*ouder)->is_welk_kind();
            Richting ouder_kind_richting = (*plaats)->is_welk_kind();

            if (ouder_kind_richting == grootouder_ouder_richting)
            {
                // Eenvoudiger eerst de kleuren te veranderen, nu alle unique_ptr's nog kloppen
                (*ouder)->kleur = Kleur::ZWART;
                (*grootouder)->kleur = Kleur::ROOD;

                grootouder->roteer(!grootouder_ouder_richting);
            }
            else
            {
                Richting draairichting = !ouder_kind_richting;
                ouder->roteer(draairichting);
                plaats = &((*ouder)->geef_kind(draairichting));
            }
        }
        else
        {
            throw;
        }
    }

    // Voor als het nieuwe kind root, kind van root is, of "plaats" naar daar is opgeschoven na bottom up
    (*this)->kleur = Kleur::ZWART;
}

#endif
