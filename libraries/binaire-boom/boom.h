#ifndef BINAIREBOOM_H
#define BINAIREBOOM_H

#include "knoop.h"
#include "richting.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <sstream>
#include <stack>
#include <tuple>
#include <vector>

template <class Sleutel, class Data>
class Knoop;

// http://www.cs.technion.ac.il/users/yechiel/c++-faq/template-friends.html
template <class Sleutel, class Data>
class BinaireBoom;
template <class Sleutel, class Data>
bool operator==(const BinaireBoom<Sleutel, Data>& b1, const BinaireBoom<Sleutel, Data>& b2);
template <class Sleutel, class Data>
bool operator!=(const BinaireBoom<Sleutel, Data>& b1, const BinaireBoom<Sleutel, Data>& b2);

template <class Sleutel, class Data>
class BinaireBoom : public std::unique_ptr<Knoop<Sleutel, Data>>
{
public:
    BinaireBoom();
    BinaireBoom(const Sleutel& sleutel, const Data& data);
    BinaireBoom(const BinaireBoom<Sleutel, Data>& andere);
    BinaireBoom<Sleutel, Data>& operator=(const BinaireBoom<Sleutel, Data>& andere);
    BinaireBoom(BinaireBoom<Sleutel, Data>&& andere);
    BinaireBoom<Sleutel, Data>& operator=(BinaireBoom<Sleutel, Data>&& andere);
    virtual ~BinaireBoom() = default;

    int diepte() const;
    void voeg_toe(const Sleutel& sleutel, const Data& data);
    void roteer(const Richting& richting);
    void maak_onevenwichtig();
    void maak_evenwichtig();
    bool is_rep_ok() const;
    void overloop_inorder(std::function<void(const Knoop<Sleutel, Data>&)> bezoek) const;
    std::vector<std::pair<Sleutel, Data>> geef_inhoud_inorder() const;
    bool is_enkel_content_gelijk(const BinaireBoom<Sleutel, Data>& andere) const;

    friend bool operator==<>(const BinaireBoom<Sleutel, Data>& b1, const BinaireBoom<Sleutel, Data>& b2);
    friend bool operator!=<>(const BinaireBoom<Sleutel, Data>& b1, const BinaireBoom<Sleutel, Data>& b2);

    std::string get_dot_code() const;

    // TODO nakijken of de constness correct is geimplementeerd
    class const_iterator
    // https://www.cs.northwestern.edu/~riesbeck/programming/c++/stl-iterator-define.html#TOC11
    // http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2001/0101/austern/austern.htm
    {
    public:
        const_iterator(Knoop<Sleutel, Data>* huidige_knoop);
        const_iterator(const const_iterator& andere) = default;
        const_iterator(const_iterator&&) = default;
        const_iterator& operator=(const const_iterator& andere) = default;
        const_iterator& operator=(const_iterator&& andere) = default;
        virtual ~const_iterator() = default;

        const_iterator operator++(int);
        const const_iterator& operator++();
        bool operator==(const const_iterator& andere) const;
        bool operator!=(const const_iterator& andere) const;
        const Knoop<Sleutel, Data>& operator*();
        const Knoop<Sleutel, Data>* operator->();

    private:
        const Knoop<Sleutel, Data>* huidige_knoop;
    };

    const_iterator begin() const;
    const_iterator end() const;

protected:
    void maak_lijst_evenwichtig();
    void controleer_is_gelijk(const BinaireBoom<Sleutel, Data>& andere, bool& is_gelijk) const;
    std::tuple<BinaireBoom<Sleutel, Data>*, Knoop<Sleutel, Data>*> zoek(const Sleutel& sleutel);
};

/******************************************************************************/

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>::BinaireBoom() : std::unique_ptr<Knoop<Sleutel, Data>>{nullptr}
{
}

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>::BinaireBoom(const Sleutel& sleutel, const Data& data)
: std::unique_ptr<Knoop<Sleutel, Data>>{std::make_unique<Knoop<Sleutel, Data>>(sleutel, data)}
{
}

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>::BinaireBoom(const BinaireBoom<Sleutel, Data>& andere)
: std::unique_ptr<Knoop<Sleutel, Data>>{nullptr}
{
    if (andere)
    {
        this->reset(new Knoop<Sleutel, Data>{*andere});
    }
}

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>::BinaireBoom(BinaireBoom<Sleutel, Data>&& andere)
: std::unique_ptr<Knoop<Sleutel, Data>>{std::move(andere)}
{
}

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>& BinaireBoom<Sleutel, Data>::operator=(BinaireBoom<Sleutel, Data>&& andere)
{
    this->swap(andere);
    return *this;
}

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>& BinaireBoom<Sleutel, Data>::operator=(const BinaireBoom<Sleutel, Data>& andere)
{
    BinaireBoom<Sleutel, Data> temp(andere);
    temp.swap(*this);

    return *this;
}

template <class Sleutel, class Data>
bool operator==(const BinaireBoom<Sleutel, Data>& b1, const BinaireBoom<Sleutel, Data>& b2)
{
    bool is_gelijk = true;

    // controleert zowel op vorm, als op inhoud
    b1.controleer_is_gelijk(b2, is_gelijk);

    return is_gelijk;
}

template <class Sleutel, class Data>
bool operator!=(const BinaireBoom<Sleutel, Data>& b1, const BinaireBoom<Sleutel, Data>& b2)
{
    return !(b1 == b2);
}

template <class Sleutel, class Data>
int BinaireBoom<Sleutel, Data>::diepte() const
{
    if (!(*this))
    {
        return -1; // Enkel bestaande knopen kunnen een diepte hebben
    }

    return (std::max((*this)->links.diepte(), (*this)->rechts.diepte()) + 1);
}

template <class Sleutel, class Data>
void BinaireBoom<Sleutel, Data>::voeg_toe(const Sleutel& sleutel, const Data& data)
{
    BinaireBoom<Sleutel, Data>* plaats;
    Knoop<Sleutel, Data>* ouder;

    std::tie(plaats, ouder) = zoek(sleutel);
    // C++17 (nog niet ondersteund): auto [plaats, ouder] = zoek(sleutel);

    if (!(*plaats))
    {
        *plaats = BinaireBoom<Sleutel, Data>{sleutel, data};
        (*plaats)->ouder = ouder;
    }
}

template <class Sleutel, class Data>
void BinaireBoom<Sleutel, Data>::roteer(const Richting& richting)
{
    if (!(*this))
    {
        return;
    }

    if (!((*this)->geef_kind(!richting)))
    {
        return;
    }

    BinaireBoom temp{std::move(*this)};
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
void BinaireBoom<Sleutel, Data>::maak_onevenwichtig()
{
    if (!(*this))
    {
        return;
    }

    while ((*this)->links)
    {
        roteer(Richting::RECHTS);
    }

    if ((*this)->rechts)
    {
        ((*this)->rechts).maak_onevenwichtig();
    }
}

// Verwacht een gelinkte lijst
template <class Sleutel, class Data>
void BinaireBoom<Sleutel, Data>::maak_lijst_evenwichtig()
{
    if (!(*this))
    {
        return;
    }

    Richting roteer_naar_richting;

    if ((*this)->links)
    {
        roteer_naar_richting = Richting::RECHTS;
    }
    else
    {
        roteer_naar_richting = Richting::LINKS;
    }

    int huidige_diepte = diepte(); // Is nodig!
    for (int i = 0; i < (huidige_diepte / 2); i++)
    {
        roteer(roteer_naar_richting);
    }

    ((*this)->links).maak_lijst_evenwichtig();
    ((*this)->rechts).maak_lijst_evenwichtig();
}

template <class Sleutel, class Data>
void BinaireBoom<Sleutel, Data>::maak_evenwichtig()
{
    maak_onevenwichtig();
    maak_lijst_evenwichtig();
}

template <class Sleutel, class Data>
std::tuple<BinaireBoom<Sleutel, Data>*, Knoop<Sleutel, Data>*> BinaireBoom<Sleutel, Data>::zoek(const Sleutel& sleutel)
{
    BinaireBoom<Sleutel, Data>* plaats = this;
    Knoop<Sleutel, Data>* ouder = nullptr;

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
};

template <class Sleutel, class Data>
void BinaireBoom<Sleutel, Data>::overloop_inorder(std::function<void(const Knoop<Sleutel, Data>&)> bezoek_functie) const
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
std::vector<std::pair<Sleutel, Data>> BinaireBoom<Sleutel, Data>::geef_inhoud_inorder() const
{
    std::vector<std::pair<Sleutel, Data>> inorder_waarden;

    overloop_inorder([&inorder_waarden](const Knoop<Sleutel, Data>& knoop) {
        inorder_waarden.push_back(std::make_pair(knoop.geef_sleutel(), knoop.geef_data()));
    });

    return inorder_waarden;
}

template <class Sleutel, class Data>
bool BinaireBoom<Sleutel, Data>::is_rep_ok() const
{
    bool is_correct = true;

    const Sleutel* vorige = nullptr;

    overloop_inorder([&vorige, &is_correct](const Knoop<Sleutel, Data>& knoop) {
        if (!is_correct)
        {
            return;
        }

        if (vorige && (*vorige > knoop.sleutel))
        {
            is_correct = false;
            return;
        }

        if (knoop.ouder && ((knoop.ouder->links).get() != &knoop) && ((knoop.ouder->rechts).get() != &knoop))
        {
            is_correct = false;
            return;
        }
    });

    return is_correct;
}

template <class Sleutel, class Data>
void BinaireBoom<Sleutel, Data>::controleer_is_gelijk(const BinaireBoom<Sleutel, Data>& andere, bool& is_gelijk) const
{
    if (!is_gelijk)
    {
        return;
    }

    if (!(*this) && !andere)
    {
        return;
    }
    else if (!(*this) || !andere)
    {
        is_gelijk = false;
        return;
    }

    if (*(*this) != *andere)
    {
        is_gelijk = false;
        return;
    }

    (*this)->links.controleer_is_gelijk(andere->links, is_gelijk);
    (*this)->rechts.controleer_is_gelijk(andere->rechts, is_gelijk);
}

template <class Sleutel, class Data>
bool BinaireBoom<Sleutel, Data>::is_enkel_content_gelijk(const BinaireBoom<Sleutel, Data>& andere) const
{
    auto this_iter = this->begin();
    auto andere_iter = andere.begin();

    while (this_iter != this->end() && andere_iter != andere.end())
    {
        if ((this_iter->sleutel != andere_iter->sleutel) || (this_iter->data != andere_iter->data))
        {
            return false;
        }

        this_iter++;
        andere_iter++;
    }

    return true;
}

// Niet de mooiste methode
template <class Sleutel, class Data>
std::string BinaireBoom<Sleutel, Data>::get_dot_code() const
{
    std::stringstream out;

    if (!(*this))
    {
        out << "digraph BST {" << std::endl << "\t null [shape=point];" << std::endl << "}" << std::endl;
    }
    else
    {
        std::stack<const BinaireBoom<Sleutel, Data>*> te_bezoeken_deelbomen;
        te_bezoeken_deelbomen.push(this);
        int nullptr_teller = 0;

        out << "digraph BST {" << std::endl;

        while (!te_bezoeken_deelbomen.empty())
        {
            const BinaireBoom<Sleutel, Data>* huidige_deelboom = te_bezoeken_deelbomen.top();
            te_bezoeken_deelbomen.pop();

            if (*huidige_deelboom)
            {
                if ((*huidige_deelboom)->ouder)
                {
                    out << "\t " << (*huidige_deelboom)->sleutel << " -> " << (*huidige_deelboom)->ouder->sleutel
                        << " [style=dashed];" << std::endl;
                }

                for (const BinaireBoom<Sleutel, Data>* kind :
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

/******************************************************************************/

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>::const_iterator::const_iterator(Knoop<Sleutel, Data>* huidige_knoop)
: huidige_knoop{huidige_knoop}
{
}

template <class Sleutel, class Data>
typename BinaireBoom<Sleutel, Data>::const_iterator BinaireBoom<Sleutel, Data>::const_iterator::operator++(int)
{
    const_iterator temp = *this;
    ++(*this);
    return temp;
}

template <class Sleutel, class Data>
const typename BinaireBoom<Sleutel, Data>::const_iterator& BinaireBoom<Sleutel, Data>::const_iterator::operator++()
{
    if (huidige_knoop)
    {
        if (huidige_knoop->rechts)
        {
            huidige_knoop = (huidige_knoop->rechts).get();
            while (huidige_knoop->links)
            {
                huidige_knoop = (huidige_knoop->links).get();
            }
        }
        else
        {
            const Knoop<Sleutel, Data>* vorige_knoop =
                    huidige_knoop; // Niet gelijkstellen aan nullptr (stel huidige_knoop geen kinderen)!
            while (huidige_knoop && (huidige_knoop->links).get() != vorige_knoop) // geen && huidige_knoop->ouder, want
                    // moet null teruggeven als er geen
                    // opvolger is
            {
                vorige_knoop = huidige_knoop;
                huidige_knoop = huidige_knoop->ouder;
            }
        }
    }

    return *this;
}

template <class Sleutel, class Data>
bool BinaireBoom<Sleutel, Data>::const_iterator::operator==(
        const BinaireBoom<Sleutel, Data>::const_iterator& andere) const
{
    return (huidige_knoop == andere.huidige_knoop);
}

template <class Sleutel, class Data>
bool BinaireBoom<Sleutel, Data>::const_iterator::operator!=(
        const BinaireBoom<Sleutel, Data>::const_iterator& andere) const
{
    return !((*this) == andere);
}

template <class Sleutel, class Data>
const Knoop<Sleutel, Data>& BinaireBoom<Sleutel, Data>::const_iterator::operator*()
{
    return *huidige_knoop;
}

template <class Sleutel, class Data>
const Knoop<Sleutel, Data>* BinaireBoom<Sleutel, Data>::const_iterator::operator->()
{
    return huidige_knoop;
}

template <class Sleutel, class Data>
typename BinaireBoom<Sleutel, Data>::const_iterator BinaireBoom<Sleutel, Data>::begin() const
{
    Knoop<Sleutel, Data>* huidige_knoop = this->get();
    while (huidige_knoop && huidige_knoop->links)
    {
        huidige_knoop = (huidige_knoop->links).get();
    }

    return BinaireBoom<Sleutel, Data>::const_iterator{huidige_knoop};
}

template <class Sleutel, class Data>
typename BinaireBoom<Sleutel, Data>::const_iterator BinaireBoom<Sleutel, Data>::end() const
{
    return BinaireBoom<Sleutel, Data>::const_iterator{nullptr};
}

#endif /* ZOEKBOOM_H */
