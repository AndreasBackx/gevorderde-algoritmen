
#ifndef ZOEKBOOM_H
#define ZOEKBOOM_H

#include "rzknoop14.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <sstream>
#include <stack>
#include <tuple>

template <class Sleutel, class Data>
class RZKnoop;

template <class Sleutel, class Data>
class RZBoom : public std::unique_ptr<RZKnoop<Sleutel, Data>>
{
public:
    RZBoom();
    RZBoom(const Sleutel& sleutel, const Data& data, const Kleur& kleur);
    virtual ~RZBoom() = default;

    RZBoom(const RZBoom<Sleutel, Data>& andere);
    RZBoom<Sleutel, Data>& operator=(const RZBoom<Sleutel, Data>& andere);

    RZBoom(RZBoom<Sleutel, Data>&& andere);
    RZBoom<Sleutel, Data>& operator=(RZBoom<Sleutel, Data>&& andere);

    int diepte() const;
    void voeg_toe_bottom_up(const Sleutel& sleutel, const Data& data);
    void voeg_toe_top_down(const Sleutel& sleutel, const Data& data);
    void roteer(const Richting& richting);
    bool is_rep_ok() const;
    void overloop_inorder(std::function<void(const RZKnoop<Sleutel, Data>&)> bezoek) const;
    Kleur geef_kleur() const;

    std::string get_dot_code() const;

protected:
    std::tuple<RZBoom<Sleutel, Data>*, RZKnoop<Sleutel, Data>*> zoek(const Sleutel& sleutel);
    std::tuple<RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*> geef_familie(
            const RZBoom<Sleutel, Data>* const plaats);
    int check_zwarte_diepte(bool& is_correct) const;
};

/******************************************************************************/

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom() : std::unique_ptr<RZKnoop<Sleutel, Data>>{nullptr}
{
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom(const Sleutel& sleutel, const Data& data, const Kleur& kleur)
    : std::unique_ptr<RZKnoop<Sleutel, Data>>{std::make_unique<RZKnoop<Sleutel, Data>>(sleutel, data, kleur)} {}

      ;

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom(const RZBoom<Sleutel, Data>& andere) : std::unique_ptr<RZKnoop<Sleutel, Data>>{nullptr}
{
    if (andere)
    {
        this->reset(new RZKnoop<Sleutel, Data>{*andere});
    }
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom(RZBoom<Sleutel, Data>&& andere)
    : std::unique_ptr<RZKnoop<Sleutel, Data>>{std::move(andere)}
{
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>& RZBoom<Sleutel, Data>::operator=(RZBoom<Sleutel, Data>&& andere)
{
    this->swap(andere);
    return *this;
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>& RZBoom<Sleutel, Data>::operator=(const RZBoom<Sleutel, Data>& andere)
{
    RZBoom<Sleutel, Data> temp{andere};
    temp.swap(*this);

    return *this;
}

template <class Sleutel, class Data>
int RZBoom<Sleutel, Data>::diepte() const
{
    if (!(*this))
    {
        return -1; // Enkel bestaande knopen kunnen een diepte hebben
    }

    return (std::max((*this)->links.diepte(), (*this)->rechts.diepte()) + 1);
}

template <class Sleutel, class Data>
std::tuple<RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*> RZBoom<Sleutel, Data>::geef_familie(
        const RZBoom<Sleutel, Data>* const plaats)
{
    assert(plaats && *plaats);
    assert((*plaats)->ouder && (*plaats)->ouder->ouder);

    RZBoom<Sleutel, Data>* grootouder = nullptr;
    if (!((*plaats)->ouder->ouder->ouder))
    {
        grootouder = this;
    }
    else
    {
        grootouder = (*plaats)->ouder->ouder->ouder->geef_kind((*plaats)->ouder->ouder->is_welk_kind());
    }

    Richting grootouder_ouder_richting = (*plaats)->ouder->is_welk_kind();
    RZBoom<Sleutel, Data>* ouder = (*grootouder)->geef_kind(grootouder_ouder_richting);
    RZBoom<Sleutel, Data>* nonkel = (*grootouder)->geef_kind(inverse_richting(grootouder_ouder_richting));

    return std::make_tuple(grootouder, ouder, nonkel);
}

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::voeg_toe_bottom_up(const Sleutel& sleutel, const Data& data)
{
    RZBoom<Sleutel, Data>* plaats;
    RZKnoop<Sleutel, Data>* ouder;

    std::tie(plaats, ouder) = zoek(sleutel);

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
        RZBoom<Sleutel, Data>* grootouder = nullptr;
        RZBoom<Sleutel, Data>* ouder = nullptr;
        RZBoom<Sleutel, Data>* nonkel = nullptr;
        std::tie(grootouder, ouder, nonkel) = geef_familie(plaats);

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

                grootouder->roteer(inverse_richting(grootouder_ouder_richting));
            }
            else
            {
                Richting draairichting = inverse_richting(ouder_kind_richting);
                ouder->roteer(draairichting);
                plaats = (*ouder)->geef_kind(draairichting);
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

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::voeg_toe_top_down(const Sleutel& sleutel, const Data& data)
{
    RZBoom<Sleutel, Data>* plaats = this;
    RZKnoop<Sleutel, Data>* ouder_knoop = nullptr;

    bool is_toegevoegd = false;
    while (!is_toegevoegd)
    {
        RZBoom<Sleutel, Data>* linkerkind = (*plaats)->geef_kind(Richting::LINKS);
        RZBoom<Sleutel, Data>* rechterkind = (*plaats)->geef_kind(Richting::RECHTS);

        if (!(*plaats))
        {
            *plaats = RZBoom<Sleutel, Data>{sleutel, data, Kleur::ROOD};
            (*plaats)->ouder = ouder_knoop;

            is_toegevoegd = true;
        }
        else if ((*plaats)->sleutel == sleutel)
        {
            throw "Sleutel is al aanwezig";
        }
        else if ((*plaats)->geef_kleur() == Kleur::ZWART && linkerkind->geef_kleur() == Kleur::ROOD
                 && rechterkind->geef_kleur() == Kleur::ROOD)
        {
            (*plaats)->kleur = Kleur::ROOD;
            (*linkerkind)->kleur = Kleur::ZWART;
            (*rechterkind)->kleur = Kleur::ZWART;
        }

        if ((*plaats)->ouder && (*plaats)->ouder->ouder && ((*plaats)->kleur == Kleur::ROOD)
            && ((*plaats)->ouder->kleur == Kleur::ROOD))
        { // Niet alleen bij het afdalen, maar ook na het toevoegen kan er dubbelrood optreden
            RZBoom<Sleutel, Data>* ouder = nullptr;
            RZBoom<Sleutel, Data>* nonkel = nullptr;
            RZBoom<Sleutel, Data>* grootouder = nullptr;

            std::tie(grootouder, ouder, nonkel) = geef_familie(plaats);

            Richting grootouder_ouder_richting = (*ouder)->is_welk_kind();
            Richting ouder_kind_richting = (*plaats)->is_welk_kind();

            if (ouder_kind_richting != grootouder_ouder_richting)
            {
                (*plaats)->kleur = Kleur::ZWART;
                (*grootouder)->kleur = Kleur::ROOD;

                ouder->roteer(inverse_richting(ouder_kind_richting));
                grootouder->roteer(inverse_richting(grootouder_ouder_richting));
            }
            else
            {
                (*ouder)->kleur = Kleur::ZWART;
                (*grootouder)->kleur = Kleur::ROOD;

                grootouder->roteer(inverse_richting(grootouder_ouder_richting));
            }
        }

        if (!is_toegevoegd)
        { // Anders wordt de throw opgeroepen na het toevoegen
            ouder_knoop = plaats->get();
            if (sleutel < (*plaats)->sleutel)
            {
                plaats = (*plaats)->geef_kind(Richting::LINKS);
            }
            else if (sleutel > (*plaats)->sleutel)
            {
                plaats = (*plaats)->geef_kind(Richting::RECHTS);
            }
            else
            {
                throw "Sleutel is al aanwezig";
            }
        }
    }

    (*this)->kleur = Kleur::ZWART;
}

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::roteer(const Richting& richting)
{
    if (!(*this))
    {
        return;
    }

    if (richting == Richting::RECHTS)
    {
        if (!((*this)->links))
        {
            return;
        }

        RZBoom temp{std::move(*this)};
        *this = std::move(temp->links);
        temp->links = std::move((*this)->rechts);
        (*this)->rechts = std::move(temp);

        (*this)->ouder = (*this)->rechts->ouder;
        (*this)->rechts->ouder = this->get();
        if ((*this)->rechts->links)
        {
            (*this)->rechts->links->ouder = ((*this)->rechts).get();
        }
    }
    else if (richting == Richting::LINKS)
    {
        if (!(*this)->rechts)
        {
            return;
        }

        RZBoom temp{std::move(*this)};
        (*this) = std::move(temp->rechts);
        temp->rechts = std::move((*this)->links);
        (*this)->links = std::move(temp);

        (*this)->ouder = (*this)->links->ouder;
        (*this)->links->ouder = this->get();
        if ((*this)->links->rechts)
        {
            (*this)->links->rechts->ouder = ((*this)->links).get();
        }
    }
}

template <class Sleutel, class Data>
std::tuple<RZBoom<Sleutel, Data>*, RZKnoop<Sleutel, Data>*> RZBoom<Sleutel, Data>::zoek(const Sleutel& sleutel)
{
    RZBoom<Sleutel, Data>* plaats = this;
    RZKnoop<Sleutel, Data>* ouder = nullptr;

    while (*plaats && (*plaats)->sleutel != sleutel)
    {
        ouder = plaats->get();

        if (sleutel > (*plaats)->sleutel)
        {
            plaats = &(*plaats)->rechts;
        }
        else
        {
            plaats = &(*plaats)->links;
        }
    };

    return std::make_tuple(plaats, ouder);
}

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::overloop_inorder(std::function<void(const RZKnoop<Sleutel, Data>&)> bezoek_functie) const
{
    if (!(*this))
    {
        return;
    }

    (*this)->links.overloop_inorder(bezoek_functie);

    bezoek_functie(*(*this));

    (*this)->rechts.overloop_inorder(bezoek_functie);
}

template <class Sleutel, class Data>
bool RZBoom<Sleutel, Data>::is_rep_ok() const
{
    bool is_correct = true;

    const Sleutel* vorige = nullptr;

    overloop_inorder([&vorige, &is_correct](const RZKnoop<Sleutel, Data>& knoop) {
        if (!is_correct)
        {
            return;
        }

        bool is_vorige_sleutel_incorrect = vorige && (*vorige > knoop.sleutel);
        bool is_ouder_incorrect
                = knoop.ouder && ((knoop.ouder->links).get() != &knoop) && ((knoop.ouder->rechts).get() != &knoop);
        bool is_dubbel_rood = knoop.ouder && (knoop.kleur == Kleur::ROOD) && (knoop.ouder->kleur == Kleur::ROOD);
        bool is_wortel_rood = !knoop.ouder && (knoop.kleur == Kleur::ROOD);

        if (is_vorige_sleutel_incorrect || is_ouder_incorrect || is_dubbel_rood || is_wortel_rood)
        {
            is_correct = false;
            return;
        }
    });

    if (is_correct)
    {
        (void) check_zwarte_diepte(is_correct);
    }

    return is_correct;
}

template <class Sleutel, class Data>
int RZBoom<Sleutel, Data>::check_zwarte_diepte(bool& is_correct) const
{
    if (!is_correct)
    {
        return -1;
    }

    if (!(*this))
    {
        return 1;
    }

    int zwarte_diepte_links = ((*this)->links).check_zwarte_diepte(is_correct);
    int zwarte_diepte_rechts = ((*this)->rechts).check_zwarte_diepte(is_correct);

    if (zwarte_diepte_links != zwarte_diepte_rechts)
    {
        is_correct = false;
        return -1;
    }

    if ((*this)->kleur == Kleur::ZWART)
    {
        return (zwarte_diepte_links + 1);
    }

    if ((*this)->kleur == Kleur::ROOD)
    {
        return zwarte_diepte_links;
    }

    throw;
}

template <class Sleutel, class Data>
Kleur RZBoom<Sleutel, Data>::geef_kleur() const
{
    if (!(*this))
    {
        return Kleur::ZWART;
    }

    return (*this)->geef_kleur();
}

// Niet de mooiste methode
template <class Sleutel, class Data>
std::string RZBoom<Sleutel, Data>::get_dot_code() const
{
    std::stringstream out;

    if (!(*this))
    {
        out << "digraph BST {" << std::endl << "\t null [shape=point];" << std::endl << "}" << std::endl;
    }
    else
    {
        std::stack<const RZBoom<Sleutel, Data>*> te_bezoeken_deelbomen;
        te_bezoeken_deelbomen.push(this);
        int nullptr_teller = 0;

        out << "digraph BST {" << std::endl;

        while (!te_bezoeken_deelbomen.empty())
        {
            const RZBoom<Sleutel, Data>* huidige_deelboom = te_bezoeken_deelbomen.top();
            te_bezoeken_deelbomen.pop();

            if (*huidige_deelboom)
            {
                if ((*huidige_deelboom)->kleur == Kleur::ROOD)
                {
                    out << "\t" << (*huidige_deelboom)->sleutel << " [color=\"red\"];" << std::endl;
                }

                if ((*huidige_deelboom)->ouder)
                {
                    out << "\t " << (*huidige_deelboom)->sleutel << " -> " << (*huidige_deelboom)->ouder->sleutel
                        << " [style=dashed];" << std::endl;
                }

                for (const RZBoom<Sleutel, Data>* kind :
                     {&((*huidige_deelboom)->links), &((*huidige_deelboom)->rechts)})
                {
                    if (kind)
                    {
                        if (*kind)
                        {
                            out << "\t " << (*huidige_deelboom)->sleutel << " -> " << (*kind)->sleutel << ";"
                                << std::endl;
                            te_bezoeken_deelbomen.push(kind);
                        }
                        else
                        {
                            out << "\t null" << nullptr_teller << " [shape=point]"
                                << ";" << std::endl;
                            out << "\t " << (*huidige_deelboom)->sleutel << " -> "
                                << "null" << nullptr_teller << ";" << std::endl;
                            nullptr_teller++;
                        }
                    }
                }
            }
        }

        out << "}" << std::endl;
    }

    return out.str();
}

#endif /* ZOEKBOOM_H */
