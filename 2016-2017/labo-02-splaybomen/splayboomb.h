
#ifndef SPLAYBOOM_H
#define SPLAYBOOM_H

#include <cmath>

#include "../labo-1-week-1b/zoekboom.h"
#include "csv.h"

template <class Sleutel, class Data>
class Splayboom : public Zoekboom<Sleutel, Data>
{
private:

    void splay(Zoekboom<Sleutel, Data>*& plaats);
    void bereken_potentiaal(const Zoekboom<Sleutel, Data>& splayboom, double& som_gewichten, int& aantal_knopen) const; // Deze funcite is eigen aan splayboom en steken we dus niet in de Zoekboom klasse

public:
    using Zoekboom<Sleutel, Data>::is_leeg;
    using Zoekboom<Sleutel, Data>::vind_zoekboom_van_wortel;

    void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);
    void voeg_toe(const Sleutel& sleutel, const Data& data);
    double potentiaal() const;
};

/******************************************************************************/

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::bereken_potentiaal(const Zoekboom<Sleutel, Data>& splayboom, double& som_gewichten, int& aantal_knopen) const
{
    int aantal_knopen_links = 0;
    int aantal_knopen_rechts = 0;

    if (splayboom->get_links())
    {
        bereken_potentiaal(splayboom->get_links(), som_gewichten, aantal_knopen_links);
    }

    if (splayboom->get_rechts())
    {
        bereken_potentiaal(splayboom->get_rechts(), som_gewichten, aantal_knopen_rechts);
    }

    aantal_knopen = aantal_knopen_links + aantal_knopen_rechts + 1;
    som_gewichten += std::log2(aantal_knopen);
}

template <class Sleutel, class Data>
double Splayboom<Sleutel, Data>::potentiaal() const
{
    int aantal_knopen = 0;
    double som_gewichten = 0;

    bereken_potentiaal(*this, som_gewichten, aantal_knopen);

    return som_gewichten;
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats)
{
    if (is_leeg())
    {
        return;
    }

    ouder = nullptr;
    plaats = nullptr;

    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);

    if (!(*plaats))
    {
        if (!ouder)
        {
            throw "Er is geen ouder.";
        }

        plaats = &vind_zoekboom_van_wortel(*ouder);
    }

    splay(plaats);
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::voeg_toe(const Sleutel& sleutel, const Data& data)
{
    Zoekboom<Sleutel, Data>* plaats = nullptr;
    Zoekknoop<Sleutel, Data>* ouder = nullptr;

    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);

    if (*plaats)
    {
        throw "De sleutel is al aanwezig in de boom.";
    }

    Zoekboom<Sleutel, Data> nieuw{new Zoekknoop<Sleutel, Data>{sleutel, data, ouder}};

    *plaats = std::move(nieuw);

    splay(plaats);
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::splay(Zoekboom<Sleutel, Data>*& plaats)
{
    if (!plaats)
    {
        return;
    }

    while (plaats != this)
    {
        // Ik maak veel tussenvariabelen aan.
        // Deze heeft slechts een kleine constante invloed op de performantie, maar verduidelijken de code een groot stuk
        // Het kan zijn dat de compiler deze zelfs wegfiltered (TODO nakijken en bronnen opzoeken)

        Zoekboom<Sleutel, Data>& huidige_boom = *plaats;
        Zoekboom<Sleutel, Data>& ouder_boom = vind_zoekboom_van_wortel(*(huidige_boom->get_ouder()));

        if (ouder_boom.get() == this->get())
        {
            this->roteer(
                andere_richting(huidige_boom->is_linker_of_rechter_kind())
            );

            // De boomwijzer naar de ouder verwijst nu naar het kind, dus moet de plaatspointer daar naar aangepast worden
            plaats = &ouder_boom;
        }
        else // Er is sowieso een grootouder, want de huidige plaats en ouder zijn al gechecked
        {
            Zoekboom<Sleutel, Data>& grootouder_boom = vind_zoekboom_van_wortel(*(ouder_boom->get_ouder()));

            if (huidige_boom->is_linker_of_rechter_kind() == ouder_boom->is_linker_of_rechter_kind())
            {
                grootouder_boom.roteer(
                    andere_richting(
                        ouder_boom->is_linker_of_rechter_kind()
                    )
                );

                // De zoekboompointer die naar de grootouder wees, wijst nu naar de ouder
                grootouder_boom.roteer(
                    andere_richting(
                        // Door de nieuwe structuur na vorige rotatie moet er naar ouder -> kind richting gekeken worden voor de richting
                        huidige_boom->is_linker_of_rechter_kind()
                    )
                );
            }
            else
            {
                ouder_boom.roteer(
                    andere_richting(
                        huidige_boom->is_linker_of_rechter_kind()
                    )
                );

                grootouder_boom.roteer(
                    andere_richting(
                        ouder_boom->is_linker_of_rechter_kind()
                    )
                );
            } // else

            // De boomwijzer naar de grootouder verwijst nu naar het kind, dus moet de plaatspointer daar naar aangepast worden
            plaats = &grootouder_boom;
        } // else
    } // while
}

#endif