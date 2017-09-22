
#ifndef ZOEKBOOM_H
#define ZOEKBOOM_H

#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <sstream>
#include <stack>
#include <cassert>
#include <algorithm>

/**********************************************************************

   Class: Zoekboom
   
   beschrijving: Binaire zoekboom waarin geen duplicaatsleutels zijn toegestaan.
   
 ***************************************************************************/

template <class Sleutel, class Data>
class Zoekknoop;

template <class Sleutel, class Data>
class Zoekboom : public std::unique_ptr<Zoekknoop<Sleutel, Data>>
{
public:

    Zoekboom();
    Zoekboom(const Sleutel& sleutel, const Data& data);
    Zoekboom(const Zoekboom& andere, Zoekknoop<Sleutel, Data>* ouder);
    virtual ~Zoekboom();

    Zoekboom(const Zoekboom& andere);
    Zoekboom& operator=(const Zoekboom & andere);

    Zoekboom(Zoekboom&& andere) : std::unique_ptr<Zoekknoop<Sleutel, Data>>{std::move(andere)} {};
    using std::unique_ptr<Zoekknoop<Sleutel, Data>>::operator=;
    Zoekboom& operator=(Zoekboom&& andere) = default;

    void voegtoe(const Sleutel& sleutel, const Data & data);
    
    std::string get_dot_code() const;

protected:

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
Zoekboom<Sleutel, Data>::Zoekboom(const Zoekboom& andere)
{
    if (!andere)
    {
        this->reset(nullptr);
    }
    else
    {
        *this = std::make_unique<Zoekknoop<Sleutel, Data>>(*andere, nullptr);
    }
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekboom<Sleutel, Data>::operator=(const Zoekboom<Sleutel, Data>& andere)
{
    Zoekboom<Sleutel, Data> temp(andere);
    temp.swap(*this);

    return *this;
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>::Zoekboom(const Zoekboom& andere, Zoekknoop<Sleutel, Data>* ouder)
{
    if (!andere)
    {
        this->reset(nullptr);
    }
    else
    {
        *this = std::make_unique<Zoekknoop<Sleutel, Data>>(*andere, ouder);
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data)
{
    Zoekboom<Sleutel, Data>* plaats;
    Zoekknoop<Sleutel, Data>* ouder;
    
    zoek(sleutel, ouder, plaats);
    
    if (!(*plaats))
    {
        *plaats = Zoekboom<Sleutel, Data>{sleutel, data};
        (*plaats)->ouder = ouder;
    };
};

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
std::string Zoekboom<Sleutel, Data>::get_dot_code() const 
{
    std::stringstream ss;

    if (!(*this))
    {
        ss << "digraph BST {" << std::endl
            << "\t null [shape=point];" << std::endl
            << "}" << std::endl;
    }
    else
    {
        std::stack<const Zoekboom<Sleutel, Data>*> te_bezoeken_deelbomen;
        te_bezoeken_deelbomen.push(this);
        int nullptr_teller = 0;

        ss << "digraph BST {" << std::endl;

        while (!te_bezoeken_deelbomen.empty())
        {
            const Zoekboom<Sleutel, Data>* huidige_deelboom = te_bezoeken_deelbomen.top();
            te_bezoeken_deelbomen.pop();

            const Zoekboom<Sleutel, Data>& linker_kind = (*huidige_deelboom)->links;
            const Zoekboom<Sleutel, Data>& rechter_kind = (*huidige_deelboom)->rechts;
            
            if (linker_kind)
            {
                ss << "\t " << (*huidige_deelboom)->sleutel << " -> " << linker_kind->sleutel << ";" << std::endl;
                te_bezoeken_deelbomen.push(&linker_kind);
            }
            else
            {
                ss << "\t null" << nullptr_teller << " [shape=point]" << ";" << std::endl;
                ss << "\t " << (*huidige_deelboom)->sleutel << " -> " << "null" << nullptr_teller << ";" << std::endl;
                nullptr_teller++;
            }

            if (rechter_kind)
            {
                ss << "\t " << (*huidige_deelboom)->sleutel << " -> " << rechter_kind->sleutel << ";" << std::endl;
                te_bezoeken_deelbomen.push(&rechter_kind);
            }
            else
            {
                ss << "\t null" << nullptr_teller << " [shape=point]" << ";" << std::endl;
                ss << "\t " << (*huidige_deelboom)->sleutel << " -> " << "null" << nullptr_teller << ";" << std::endl;
                nullptr_teller++;
            }
        }

        ss << "}" << std::endl;
    }

    return ss.str();
}

/******************************************************************************/

template <class Sleutel, class Data>
class Zoekknoop
{
public:

    friend class Zoekboom<Sleutel, Data>;

    Zoekknoop(const Sleutel& sleutel, const Data& data);
    Zoekknoop(const Zoekknoop<Sleutel, Data>& ander, Zoekknoop<Sleutel, Data>* ouder);
    virtual ~Zoekknoop();

    Zoekknoop(const Zoekknoop& andere) = delete;
    Zoekknoop& operator=(const Zoekknoop & andere) = delete;

    Zoekknoop(Zoekknoop&& andere) = delete;
    Zoekknoop& operator=(Zoekknoop&& andere) = delete;

protected:

    Sleutel sleutel;
    Data data;

    Zoekknoop<Sleutel, Data>* ouder;
    Zoekboom<Sleutel, Data> links, rechts;
};

/******************************************************************************/

template <class Sleutel, class Data>
Zoekknoop<Sleutel, Data>::Zoekknoop(const Sleutel& sleutel, const Data& data) 
: sleutel{sleutel}, data{data}, ouder{nullptr}
{}

template <class Sleutel, class Data>
Zoekknoop<Sleutel, Data>::~Zoekknoop() 
{}

template <class Sleutel, class Data>
Zoekknoop<Sleutel, Data>::Zoekknoop(const Zoekknoop<Sleutel, Data>& ander, Zoekknoop<Sleutel, Data>* ouder) 
{
    sleutel = ander.sleutel;
    data = ander.data;
    
    this->ouder = ouder;
    
    links = Zoekboom<Sleutel, Data>(ander.links, ouder);
    rechts = Zoekboom<Sleutel, Data>(ander.rechts, ouder);
}

#endif /* ZOEKBOOM_H */
