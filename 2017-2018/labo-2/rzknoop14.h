
#ifndef ZOEKKNOOP_H
#define ZOEKKNOOP_H

#include "rzboom14.h"

#include <memory>
#include <cassert>

enum class Kleur
{
    ROOD,
    ZWART
};

enum class Richting
{
    LINKS,
    RECHTS
};

Richting inverse_richting(const Richting& richting)
{
    if (richting == Richting::LINKS)
    {
        return Richting::RECHTS;
    }
    else if (richting == Richting::RECHTS)
    {
        return Richting::LINKS;
    }
    else
    {
        throw;
    }
}

template <class Sleutel, class Data>
class RZBoom;

template <class Sleutel, class Data>
class RZKnoop
{
public:

    friend class RZBoom<Sleutel, Data>;

    RZKnoop(const Sleutel& sleutel, const Data& data, const Kleur& kleur);
    virtual ~RZKnoop();

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
    RZBoom<Sleutel, Data>* geef_kind(const Richting& richting);

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
RZKnoop<Sleutel, Data>::RZKnoop(const Sleutel& sleutel, const Data& data, const Kleur& kleur)
: sleutel{sleutel}, data{data}, kleur{kleur}, ouder{nullptr}
{}

template <class Sleutel, class Data>
RZKnoop<Sleutel, Data>::~RZKnoop()
{}

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
    if (links) {
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
    return ((sleutel == andere.sleutel)
            && (data == andere.data)
            && (kleur == andere.kleur));
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
    else if (this == (ouder->rechts).get())
    {
        return Richting::RECHTS;
    }
    else
    {
        throw;
    }
}

template <class Sleutel, class Data>
RZBoom<Sleutel, Data>* RZKnoop<Sleutel, Data>::geef_kind(const Richting& richting)
{
    if (richting == Richting::LINKS)
    {
        return &(links);
    }
    else if (richting == Richting::RECHTS)
    {
        return &(rechts);
    }
    else
    {
        throw;
    }
}

#endif /* ZOEKKNOOP_H */
