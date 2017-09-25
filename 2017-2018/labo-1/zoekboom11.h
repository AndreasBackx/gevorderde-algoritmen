
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

    Zoekboom(const Zoekboom& andere);
    Zoekboom& operator=(const Zoekboom & andere);

    Zoekboom(Zoekboom&& andere) : std::unique_ptr<Zoekknoop<Sleutel, Data>>{std::move(andere)} {};
    using std::unique_ptr<Zoekknoop<Sleutel, Data>>::operator=;
    Zoekboom& operator=(Zoekboom&& andere) = default;

    int diepte() const;
    double gemiddelde_diepte() const;
    void voegtoe(const Sleutel& sleutel, const Data & data);
    void roteer(const Richting& richting);
    
    std::string get_dot_code() const;

protected:

    void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);
    void gemiddelde_diepte(int diepte, int& som_dieptes, int& aantal_knopen) const;
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
        *this = std::make_unique<Zoekknoop<Sleutel, Data>>(*andere);
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
int Zoekboom<Sleutel, Data>::diepte() const
{
    if (!(*this)) {
        return -1; // Enkel bestaande knopen kunnen een diepte hebben
    } 
    
    return (std::max((*this)->links.diepte(), (*this)->rechts.diepte()) + 1);
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::gemiddelde_diepte(int diepte, int& som_dieptes, int& aantal_knopen) const
{
    if (!(*this)) {
        return;
    } 
    
    aantal_knopen++;
    som_dieptes += diepte;
    
    (*this)->links.gemiddelde_diepte((diepte + 1), som_dieptes, aantal_knopen);
    (*this)->rechts.gemiddelde_diepte((diepte + 1), som_dieptes, aantal_knopen);
}

template <class Sleutel, class Data>
double Zoekboom<Sleutel, Data>::gemiddelde_diepte() const
{
    if (!(*this)) {
        return 0;
    } 
    
    int som_dieptes = 0;
    int aantal_knopen = 0;
    
    this->gemiddelde_diepte(0, som_dieptes, aantal_knopen);
    
    std::cout << som_dieptes << ", " << aantal_knopen << std::endl;
    return (static_cast<double>(som_dieptes) / static_cast<double>(aantal_knopen));
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

//        if ((*this)->rechts->links)
//        {
//            (*this)->rechts->links->ouder = (*this)->ouder;
//        }
//        (*this)->ouder = (*this)->rechts->ouder;
//        (*this)->rechts->ouder = this->get();

        (*this)->ouder = (*this)->rechts->ouder;
        (*this)->rechts->ouder = this->get();
        if ((*this)->rechts->links)
        {
            (*this)->rechts->links->ouder = ((*this)->rechts).get();
        }
    }
    else if (richting == Richting::LINKS)
    {

    }
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

            if (*huidige_deelboom)
            {

                const Zoekboom<Sleutel, Data>& linker_kind = (*huidige_deelboom)->links;
                const Zoekboom<Sleutel, Data>& rechter_kind = (*huidige_deelboom)->rechts;

                if ((*huidige_deelboom)->ouder)
                {
                    ss << "\t " << (*huidige_deelboom)->sleutel << " -> " << (*huidige_deelboom)->ouder->sleutel << " [style=dashed];" << std::endl;
                }

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
    virtual ~Zoekknoop();

    Zoekknoop(const Zoekknoop& andere);
    Zoekknoop& operator=(const Zoekknoop & andere);

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
Zoekknoop<Sleutel, Data>& Zoekknoop<Sleutel, Data>::operator=(const Zoekknoop<Sleutel, Data>& andere)
{
    Zoekknoop<Sleutel, Data> temp(andere);
    temp.swap(*this);

    return *this;
}

template <class Sleutel, class Data>
Zoekknoop<Sleutel, Data>::Zoekknoop(const Zoekknoop<Sleutel, Data>& ander) 
{
    sleutel = ander.sleutel;
    data = ander.data;
    ouder = nullptr; // Belangrijk voor root

    links = Zoekboom<Sleutel, Data>{ander.links};
    if (links) {
        links->ouder = this;
    }
    
    rechts = Zoekboom<Sleutel, Data>{ander.rechts};
    if (rechts)
    {
        rechts->ouder = this;
    }
}

#endif /* ZOEKBOOM_H */
