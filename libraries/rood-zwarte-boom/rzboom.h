
#ifndef RZBOOM_H
#define RZBOOM_H

#include "../binaire-boom/richting.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <stack>
#include <tuple>
#include <vector>

enum class Kleur
{
    ROOD,
    ZWART
};

template <class Sleutel, class Data>
class RZKnoop;

template <class Sleutel, class Data>
class RZBoom : public std::unique_ptr<RZKnoop<Sleutel, Data>>
{
public:
    RZBoom();
    RZBoom(const std::vector<std::tuple<Sleutel, Data, Kleur>>& inhoud);
    RZBoom(const Sleutel& sleutel, const Data& data, const Kleur& kleur);
    virtual ~RZBoom() = default;

    RZBoom(const RZBoom<Sleutel, Data>& andere);
    RZBoom<Sleutel, Data>& operator=(const RZBoom<Sleutel, Data>& andere);

    RZBoom(RZBoom<Sleutel, Data>&& andere);
    RZBoom<Sleutel, Data>& operator=(RZBoom<Sleutel, Data>&& andere);

    virtual void voeg_toe(const Sleutel& sleutel, const Data& data)
    {
        // = 0 (pure abstract method) werkt niet, omdat de methodes in RZBoom instanties van RZBoom gebruiken
        throw "Unsupported";
    }

    int diepte() const;
    void roteer(const Richting& richting);
    bool is_rep_ok() const;
    void overloop_inorder(std::function<void(const RZKnoop<Sleutel, Data>&)> bezoek) const;
    void overloop_postorder(std::function<void(const RZKnoop<Sleutel, Data>&)> bezoek) const;
    Kleur geef_kleur() const;

    std::string get_dot_code() const;

    void teken_als_234_boom(const char* bestandsnaam) const;
    void teken_als_binaire_boom(const char* bestandsnaam) const;

protected:
    std::tuple<RZBoom<Sleutel, Data>*, RZKnoop<Sleutel, Data>*> zoek(const Sleutel& sleutel);
    std::tuple<RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*>
    geef_familie(const RZBoom<Sleutel, Data>* const plaats);

    int check_zwarte_diepte(bool& is_correct) const;

    std::string teken_als_234_boom_rec(std::ostream& uit, int& nullteller, int& knoop34teller) const;
    std::string teken_als_binaire_boom_rec(std::ostream& uit, int& nullteller) const;
};

template <class Sleutel, class Data>
class RZBoomBottomUp;

template <class Sleutel, class Data>
class RZBoomTopDown;

template <class Sleutel, class Data>
class RZKnoop
{
public:
    // friend is niet overerfbaar, vandaar dat we hier gedwongen zijn dit toe te voegen
    // Dit is geen best practice, maar door de iovererf-structuur van rzboom en unique_ptr
    // hebben we hier geen keuze ...
    friend class RZBoom<Sleutel, Data>;
    friend class RZBoomBottomUp<Sleutel, Data>;
    friend class RZBoomTopDown<Sleutel, Data>;

    RZKnoop(const Sleutel& sleutel, const Data& data, const Kleur& kleur);
    virtual ~RZKnoop() = default;

    RZKnoop(const RZKnoop<Sleutel, Data>& andere);
    RZKnoop<Sleutel, Data>& operator=(const RZKnoop<Sleutel, Data>& andere);

    RZKnoop(RZKnoop<Sleutel, Data>&& andere) = delete;
    RZKnoop<Sleutel, Data>& operator=(RZKnoop<Sleutel, Data>&& andere) = delete;

    bool operator==(const RZKnoop<Sleutel, Data>& andere) const;
    bool operator!=(const RZKnoop<Sleutel, Data>& andere) const;

    const Sleutel& geef_sleutel() const;
    const Data& geef_data() const;
    const Kleur& geef_kleur() const;
    Richting is_welk_kind() const;
    RZBoom<Sleutel, Data>& geef_kind(const Richting& richting);

protected:
    Sleutel sleutel;
    Data data;
    Kleur kleur;

    RZKnoop<Sleutel, Data>* ouder;
    RZBoom<Sleutel, Data> links;
    RZBoom<Sleutel, Data> rechts;
};

/******************************************************************************/

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom() : std::unique_ptr<RZKnoop<Sleutel, Data>>{nullptr}
{
}

// Om een boom opnieuw op te bouwen, voeg je de sleutels in level order toe 
// (vergelijkbaar met breedte eerst zoeken). Dit komt omdat binaire per niveau
// onderaan groeien
template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom(const std::vector<std::tuple<Sleutel, Data, Kleur>>& inhoud)
{
    for (auto [sleutel, data, kleur] : inhoud)
    {
        auto [plaats, ouder] = zoek(sleutel);
        assert(!(*plaats));

        *plaats = RZBoom<Sleutel, Data>{sleutel, data, kleur};
        (*plaats)->ouder = ouder;
    }
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom(const Sleutel& sleutel, const Data& data, const Kleur& kleur)
: std::unique_ptr<RZKnoop<Sleutel, Data>>{std::make_unique<RZKnoop<Sleutel, Data>>(sleutel, data, kleur)}
{
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>::RZBoom(const RZBoom<Sleutel, Data>& andere) : std::unique_ptr<RZKnoop<Sleutel, Data>>{nullptr}
{
    if (andere)
    {
        // new vermijden -> zie uitleg binaire boom
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

    return (1 + std::max((*this)->links.diepte(), (*this)->rechts.diepte()));
}

template <class Sleutel, class Data>
std::tuple<RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*, RZBoom<Sleutel, Data>*>
RZBoom<Sleutel, Data>::geef_familie(const RZBoom<Sleutel, Data>* const plaats)
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
        grootouder = &((*plaats)->ouder->ouder->ouder->geef_kind((*plaats)->ouder->ouder->is_welk_kind()));
    }

    Richting grootouder_ouder_richting = (*plaats)->ouder->is_welk_kind();
    RZBoom<Sleutel, Data>* ouder = &((*grootouder)->geef_kind(grootouder_ouder_richting));
    RZBoom<Sleutel, Data>* nonkel = &((*grootouder)->geef_kind(!grootouder_ouder_richting));

    return std::make_tuple(grootouder, ouder, nonkel);
}

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::roteer(const Richting& richting)
{
    if (!(*this))
    {
        return;
    }

    if (!((*this)->geef_kind(!richting)))
    {
        return;
    }

    RZBoom temp{std::move(*this)};
    (*this) = std::move(temp->geef_kind(!richting));
    temp->geef_kind(!richting) = std::move((*this)->geef_kind(richting));
    (*this)->geef_kind(richting) = std::move(temp);

    (*this)->ouder = (*this)->geef_kind(richting)->ouder;
    (*this)->geef_kind(richting)->ouder = this->get();
    if ((*this)->geef_kind(richting)->geef_kind(!richting))
    {
        (*this)->geef_kind(richting)->geef_kind(!richting)->ouder = ((*this)->geef_kind(richting)).get();
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
void RZBoom<Sleutel, Data>::overloop_postorder(std::function<void(const RZKnoop<Sleutel, Data>&)> bezoek_functie) const
{
    if (!(*this))
    {
        return;
    }

    (*this)->links.overloop_inorder(bezoek_functie);
    (*this)->rechts.overloop_inorder(bezoek_functie);

    bezoek_functie(*(*this));
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
        bool is_ouder_incorrect =
                knoop.ouder && ((knoop.ouder->links).get() != &knoop) && ((knoop.ouder->rechts).get() != &knoop);
        bool is_dubbel_rood = knoop.ouder && (knoop.kleur == Kleur::ROOD) && (knoop.ouder->kleur == Kleur::ROOD);
        bool is_wortel_rood = !knoop.ouder && (knoop.kleur == Kleur::ROOD);

        if (is_vorige_sleutel_incorrect || is_ouder_incorrect || is_dubbel_rood || is_wortel_rood)
        {
            is_correct = false;
            return;
        }

        vorige = &(knoop.sleutel);
    });

    if (is_correct)
    {
        // Om de zwarte diepte te checken, heb je eerst de zwarte dieptes van de deelbomen nodig.
        // Dit is dus een post order beweging en kan dus niet (of ontzettend moeilijk en ingewikkeld)
        // gecombineerd worden met de inorder methode.
        // TODO: diepte en check_zwarte_diepte combineren met een postorder lambda
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

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::teken_als_234_boom(const char* bestandsnaam) const
{
    std::ofstream uit(bestandsnaam);
    assert(uit);

    int nullteller = 0; // Nullknopen moeten een eigen nummer krijgen.
    int knoop34teller = 0; // 3-knopen en 4-knopen worden ook genummerd

    uit << "digraph {\n";
    teken_als_234_boom_rec(uit, nullteller, knoop34teller);
    uit << "}";
}

template <class Sleutel, class Data>
std::string RZBoom<Sleutel, Data>::teken_als_234_boom_rec(std::ostream& uit, int& nullteller, int& knoop34teller) const
{
    std::ostringstream wortelstring;

    if (!(*this))
    {
        nullteller++;
        wortelstring << "null" << nullteller;
        uit << wortelstring.str() << " [shape=point];" << std::endl;
    }
    else
    {
        wortelstring << '"' << (*this)->sleutel << '"';
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << "\"]";
        if (this->geef_kleur() == Kleur::ROOD)
        {
            uit << "[color=red]";
        }

        uit << ";" << std::endl;
        std::string linkskind = (*this)->links.teken_als_234_boom_rec(uit, nullteller, knoop34teller);
        std::string rechtskind = (*this)->rechts.teken_als_234_boom_rec(uit, nullteller, knoop34teller);

        if ((*this)->links.geef_kleur() == Kleur::ROOD || (*this)->rechts.geef_kleur() == Kleur::ROOD)
        {
            uit << "subgraph cluster_" << ++knoop34teller << " {" << std::endl << "   { rank=\"same\"; ";
            if ((*this)->links.geef_kleur() == Kleur::ROOD)
            {
                uit << linkskind << " , ";
            }
            if ((*this)->rechts.geef_kleur() == Kleur::ROOD)
            {
                uit << rechtskind << " , ";
            }
            uit << wortelstring.str() << "}" << std::endl;
            if ((*this)->links.geef_kleur() == Kleur::ROOD)
            {
                uit << "   " << linkskind << " ->" << wortelstring.str() << "[dir=back];" << std::endl;
            }
            if ((*this)->rechts.geef_kleur() == Kleur::ROOD)
            {
                uit << "   " << wortelstring.str() << " -> " << rechtskind << ";" << std::endl;
            }
            uit << "color=white" << std::endl << "}" << std::endl;
        }

        if ((*this)->links.geef_kleur() == Kleur::ZWART)
        {
            uit << wortelstring.str() << " -> " << linkskind << ";" << std::endl;
        }

        if ((*this)->rechts.geef_kleur() == Kleur::ZWART)
        {
            uit << wortelstring.str() << " -> " << rechtskind << ";" << std::endl;
        }
    }

    return wortelstring.str();
}

template <class Sleutel, class Data>
void RZBoom<Sleutel, Data>::teken_als_binaire_boom(const char* bestandsnaam) const
{
    std::ofstream uit(bestandsnaam);
    assert(uit);

    int nullteller = 0; // Nullknopen moeten een eigen nummer krijgen.

    uit << "digraph {\n";
    this->teken_als_binaire_boom_rec(uit, nullteller);
    uit << "}";
}

template <class Sleutel, class Data>
std::string RZBoom<Sleutel, Data>::teken_als_binaire_boom_rec(std::ostream& uit, int& nullteller) const
{
    std::ostringstream wortelstring;

    if (!*this)
    {
        nullteller++;
        wortelstring << "null" << nullteller;
        uit << wortelstring.str() << " [shape=point];" << std::endl;
    }
    else
    {
        wortelstring << '"' << (*this)->sleutel << '"';
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << "\"]";
        if (this->geef_kleur() == Kleur::ROOD)
        {
            uit << "[color=red]";
        }
        uit << ";" << std::endl;

        std::string linkskind = (*this)->links.teken_als_binaire_boom_rec(uit, nullteller);
        std::string rechtskind = (*this)->rechts.teken_als_binaire_boom_rec(uit, nullteller);

        uit << wortelstring.str() << " -> " << linkskind << ";" << std::endl;
        uit << wortelstring.str() << " -> " << rechtskind << ";" << std::endl;
    }

    return wortelstring.str();
}

template <class Sleutel, class Data>
RZKnoop<Sleutel, Data>::RZKnoop(const Sleutel& sleutel, const Data& data, const Kleur& kleur)
: sleutel{sleutel}, data{data}, kleur{kleur}, ouder{nullptr}
{
}

template <class Sleutel, class Data>
RZKnoop<Sleutel, Data>& RZKnoop<Sleutel, Data>::operator=(const RZKnoop<Sleutel, Data>& andere)
{
    RZKnoop<Sleutel, Data> temp(andere);
    temp.swap(*this);

    return *this;
}

template <class Sleutel, class Data>
RZKnoop<Sleutel, Data>::RZKnoop(const RZKnoop<Sleutel, Data>& andere)
{
    sleutel = andere.sleutel;
    data = andere.data;
    kleur = andere.Kleur;
    ouder = nullptr; // Belangrijk voor root

    links = RZBoom<Sleutel, Data>{andere.links};
    if (links)
    {
        links->ouder = this;
    }

    rechts = RZBoom<Sleutel, Data>{andere.rechts};
    if (rechts)
    {
        rechts->ouder = this;
    }
}

template <class Sleutel, class Data>
bool RZKnoop<Sleutel, Data>::operator==(const RZKnoop<Sleutel, Data>& andere) const
{
    return ((sleutel == andere.sleutel) && (data == andere.data) && (kleur == andere.kleur));
}

template <class Sleutel, class Data>
bool RZKnoop<Sleutel, Data>::operator!=(const RZKnoop<Sleutel, Data>& andere) const
{
    return !(*this == andere);
}

template <class Sleutel, class Data>
const Sleutel& RZKnoop<Sleutel, Data>::geef_sleutel() const
{
    return sleutel;
}

template <class Sleutel, class Data>
const Data& RZKnoop<Sleutel, Data>::geef_data() const
{
    return data;
}

template <class Sleutel, class Data>
const Kleur& RZKnoop<Sleutel, Data>::geef_kleur() const
{
    return kleur;
}

template <class Sleutel, class Data>
Richting RZKnoop<Sleutel, Data>::is_welk_kind() const
{
    assert(ouder);

    if (this == (ouder->links).get())
    {
        return Richting::LINKS;
    }

    if (this == (ouder->rechts).get())
    {
        return Richting::RECHTS;
    }

    throw;
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>& RZKnoop<Sleutel, Data>::geef_kind(const Richting& richting)
{
    if (richting == Richting::LINKS)
    {
        return links;
    }

    if (richting == Richting::RECHTS)
    {
        return rechts;
    }

    throw;
}

#endif
