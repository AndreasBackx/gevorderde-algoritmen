
#ifndef ZOEKBOOM_H
#define ZOEKBOOM_H

#include "zoekknoop14.h"

#include <memory>
#include <sstream>
#include <stack>
#include <functional>
#include <algorithm>

enum class Richting
{
    LINKS,
    RECHTS
};

template <class Sleutel, class Data>
class Zoekknoop;

template <class Sleutel, class Data>
class Zoekboom : public std::unique_ptr<Zoekknoop<Sleutel, Data>>
{
public:

    Zoekboom();
    Zoekboom(const Sleutel& sleutel, const Data& data);
    virtual ~Zoekboom();

    Zoekboom(const Zoekboom<Sleutel, Data>& andere);
    Zoekboom<Sleutel, Data>& operator=(const Zoekboom<Sleutel, Data>& andere);

    Zoekboom(Zoekboom<Sleutel, Data>&& andere);
    Zoekboom<Sleutel, Data>& operator=(Zoekboom<Sleutel, Data>&& andere);

    int diepte() const;
    void voeg_toe(const Sleutel& sleutel, const Data & data);
    void roteer(const Richting& richting);
    void maak_onevenwichtig();
    void maak_evenwichtig();
    bool is_rep_ok() const;
    void overloop_inorder(std::function<void(const Zoekknoop<Sleutel,Data>&)> bezoek) const;
    
    bool is_gelijk(const Zoekboom<Sleutel, Data>& andere) const;

    std::string get_dot_code() const;

protected:

    void maak_lijst_evenwichtig();
    void controleer_is_gelijk(const Zoekboom<Sleutel, Data>& andere, bool& is_gelijk) const;
    void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);
};

/******************************************************************************/

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>::Zoekboom() 
: std::unique_ptr<Zoekknoop<Sleutel, Data>>{nullptr}
{}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>::Zoekboom(const Sleutel& sleutel, const Data& data) 
: std::unique_ptr<Zoekknoop<Sleutel, Data>>{std::make_unique<Zoekknoop<Sleutel, Data>>(sleutel, data)}
{}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>::~Zoekboom()
{}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>::Zoekboom(const Zoekboom<Sleutel, Data>& andere)
: std::unique_ptr<Zoekknoop<Sleutel, Data>>{nullptr}
{
    if (andere)
    {
        this->reset(new Zoekknoop<Sleutel, Data>{*andere});
    }
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>::Zoekboom(Zoekboom<Sleutel, Data>&& andere)
: std::unique_ptr<Zoekknoop<Sleutel, Data>>{std::move(andere)} 
{}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekboom<Sleutel, Data>::operator=(Zoekboom<Sleutel, Data>&& andere)
{
    this->swap(andere);
    return *this;
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekboom<Sleutel, Data>::operator=(const Zoekboom<Sleutel, Data>& andere)
{
    Zoekboom<Sleutel, Data> temp(andere);
    temp.swap(*this);

    return *this;
}

template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::diepte() const
{
    if (!(*this)) {
        return -1; // Enkel bestaande knopen kunnen een diepte hebben
    } 
    
    return (std::max((*this)->links.diepte(), (*this)->rechts.diepte()) + 1);
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voeg_toe(const Sleutel& sleutel, const Data& data)
{
    Zoekboom<Sleutel, Data>* plaats;
    Zoekknoop<Sleutel, Data>* ouder;
    
    zoek(sleutel, ouder, plaats);
    
    if (!(*plaats))
    {        
        *plaats = Zoekboom<Sleutel, Data>{sleutel, data};
        (*plaats)->ouder = ouder;
    };
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteer(const Richting& richting)
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

        Zoekboom temp{std::move(*this)};
        *this = std::move(temp->links);
        temp->links = std::move((*this)->rechts);
        (*this)->rechts = std::move(temp);

        (*this)->ouder = (*this)->rechts->ouder;
        (*this)->rechts->ouder = this->get();
        if ((*this)->rechts->links)
        {
            (*this)->rechts->links->ouder = ((*this)->rechts).get();
        }
        
        //        // Ook mogelijk:
//        if ((*this)->rechts->links)
//        {
//            (*this)->rechts->links->ouder = (*this)->ouder;
//        }
//        (*this)->ouder = (*this)->rechts->ouder;
//        (*this)->rechts->ouder = this->get();
    }
    else if (richting == Richting::LINKS)
    {
        if (!(*this)->rechts) 
        {
            return;
        }
        
        Zoekboom temp{std::move(*this)};
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
void Zoekboom<Sleutel, Data>::maak_onevenwichtig()
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
void Zoekboom<Sleutel, Data>::maak_lijst_evenwichtig()
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
void Zoekboom<Sleutel, Data>::maak_evenwichtig()
{
    maak_onevenwichtig();
    maak_lijst_evenwichtig();
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats)
{
    plaats = this;
    ouder = nullptr;
    
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
};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::overloop_inorder(std::function<void(const Zoekknoop<Sleutel, Data>&) > bezoek_functie) const
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
bool Zoekboom<Sleutel, Data>::is_rep_ok() const 
{
    bool is_correct = true;
    
    const Sleutel* vorige = nullptr;
    
    overloop_inorder([&vorige, &is_correct](const Zoekknoop<Sleutel, Data>& knoop)
    {
        if (!is_correct)
        {
            return;
        }
        
        if (vorige 
            && (*vorige > knoop.sleutel))
        {
            is_correct = false;
            return;
        }
        
        if(knoop.ouder 
            && ((knoop.ouder->links).get() != &knoop)
            && ((knoop.ouder->rechts).get() != &knoop))
        {
            is_correct = false;
            return;
        }
    });
    
    return is_correct;
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::controleer_is_gelijk(const Zoekboom<Sleutel, Data>& andere, bool& is_gelijk) const
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
bool Zoekboom<Sleutel, Data>::is_gelijk(const Zoekboom<Sleutel, Data>& andere) const
{
    bool is_gelijk = true;

    controleer_is_gelijk(andere, is_gelijk);
    
    return is_gelijk;
}

// Niet de mooiste methode
template <class Sleutel, class Data>
std::string Zoekboom<Sleutel, Data>::get_dot_code() const 
{
    std::stringstream out;

    if (!(*this))
    {
        out << "digraph BST {" << std::endl
            << "\t null [shape=point];" << std::endl
            << "}" << std::endl;
    }
    else
    {
        std::stack<const Zoekboom<Sleutel, Data>*> te_bezoeken_deelbomen;
        te_bezoeken_deelbomen.push(this);
        int nullptr_teller = 0;

        out << "digraph BST {" << std::endl;

        while (!te_bezoeken_deelbomen.empty())
        {
            const Zoekboom<Sleutel, Data>* huidige_deelboom = te_bezoeken_deelbomen.top();
            te_bezoeken_deelbomen.pop();

            if (*huidige_deelboom)
            {
                if ((*huidige_deelboom)->ouder)
                {
                    out << "\t " << (*huidige_deelboom)->sleutel << " -> " << (*huidige_deelboom)->ouder->sleutel << " [style=dashed];" << std::endl;
                }

                for (const Zoekboom<Sleutel, Data>* kind : { &((*huidige_deelboom)->links), &((*huidige_deelboom)->rechts)})
                {
                    if (kind)
                    {
                        if (*kind)
                        {
                            out << "\t " << (*huidige_deelboom)->sleutel << " -> " << (*kind)->sleutel << ";" << std::endl;
                            te_bezoeken_deelbomen.push(kind);
                        }
                        else
                        {
                            out << "\t null" << nullptr_teller << " [shape=point]" << ";" << std::endl;
                            out << "\t " << (*huidige_deelboom)->sleutel << " -> " << "null" << nullptr_teller << ";" << std::endl;
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
