
#ifndef ZOEKKNOOP_H
#define ZOEKKNOOP_H

#include "zoekboom14.h"

#include <memory>

template <class Sleutel, class Data>
class Zoekknoop;

template <class Sleutel, class Data>
class Zoekboom;

template <class Sleutel, class Data>
class Zoekknoop
{
public:

    friend class Zoekboom<Sleutel, Data>;

    Zoekknoop(const Sleutel& sleutel, const Data& data);
    virtual ~Zoekknoop();

    Zoekknoop(const Zoekknoop<Sleutel, Data>& andere);
    Zoekknoop<Sleutel, Data>& operator=(const Zoekknoop<Sleutel, Data>& andere);

    Zoekknoop(Zoekknoop<Sleutel, Data>&& andere) = delete;
    Zoekknoop<Sleutel, Data>& operator=(Zoekknoop<Sleutel, Data>&& andere) = delete;

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

#endif /* ZOEKKNOOP_H */
