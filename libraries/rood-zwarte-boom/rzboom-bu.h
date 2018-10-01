
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

// template <class Sleutel, class Data>
// void RZBoom<Sleutel, Data>::voeg_toe_top_down(const Sleutel& sleutel, const Data& data)
// {
//     RZBoom<Sleutel, Data>* plaats = this;
//     RZKnoop<Sleutel, Data>* ouder_knoop = nullptr;

//     bool is_toegevoegd = false;
//     while (!is_toegevoegd)
//     {
//         RZBoom<Sleutel, Data>* linkerkind = (*plaats)->geef_kind(Richting::LINKS);
//         RZBoom<Sleutel, Data>* rechterkind = (*plaats)->geef_kind(Richting::RECHTS);

//         if (!(*plaats))
//         {
//             *plaats = RZBoom<Sleutel, Data>{sleutel, data, Kleur::ROOD};
//             (*plaats)->ouder = ouder_knoop;

//             is_toegevoegd = true;
//         }
//         else if ((*plaats)->sleutel == sleutel)
//         {
//             throw "Sleutel is al aanwezig";
//         }
//         else if ((*plaats)->geef_kleur() == Kleur::ZWART && linkerkind->geef_kleur() == Kleur::ROOD
//                  && rechterkind->geef_kleur() == Kleur::ROOD)
//         {
//             (*plaats)->kleur = Kleur::ROOD;
//             (*linkerkind)->kleur = Kleur::ZWART;
//             (*rechterkind)->kleur = Kleur::ZWART;
//         }

//         if ((*plaats)->ouder && (*plaats)->ouder->ouder && ((*plaats)->kleur == Kleur::ROOD)
//             && ((*plaats)->ouder->kleur == Kleur::ROOD))
//         { // Niet alleen bij het afdalen, maar ook na het toevoegen kan er dubbelrood optreden
//             auto [grootouder, ouder, nonkel] = geef_familie(plaats);

//             Richting grootouder_ouder_richting = (*ouder)->is_welk_kind();
//             Richting ouder_kind_richting = (*plaats)->is_welk_kind();

//             if (ouder_kind_richting != grootouder_ouder_richting)
//             {
//                 (*plaats)->kleur = Kleur::ZWART;
//                 (*grootouder)->kleur = Kleur::ROOD;

//                 ouder->roteer(!ouder_kind_richting);
//                 grootouder->roteer(!grootouder_ouder_richting);
//             }
//             else
//             {
//                 (*ouder)->kleur = Kleur::ZWART;
//                 (*grootouder)->kleur = Kleur::ROOD;

//                 grootouder->roteer(!grootouder_ouder_richting);
//             }
//         }

//         if (!is_toegevoegd)
//         { // Anders wordt de throw opgeroepen na het toevoegen
//             ouder_knoop = plaats->get();
//             if (sleutel < (*plaats)->sleutel)
//             {
//                 plaats = (*plaats)->geef_kind(Richting::LINKS);
//             }
//             else if (sleutel > (*plaats)->sleutel)
//             {
//                 plaats = (*plaats)->geef_kind(Richting::RECHTS);
//             }
//             else
//             {
//                 throw "Sleutel is al aanwezig";
//             }
//         }
//     }

//     (*this)->kleur = Kleur::ZWART;
// }

#endif
