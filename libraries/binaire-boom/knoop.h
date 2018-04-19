#ifndef KNOOP_H
#define KNOOP_H

#include "boom.h"
#include "richting.h"

#include <memory>

template <class Sleutel, class Data>
class BinaireBoom;

// http://www.cs.technion.ac.il/users/yechiel/c++-faq/template-friends.html
template <class Sleutel, class Data>
class Knoop;
template <class Sleutel, class Data>
bool operator==(const Knoop<Sleutel, Data>& k1, const Knoop<Sleutel, Data>& k2);
template <class Sleutel, class Data>
bool operator!=(const Knoop<Sleutel, Data>& k1, const Knoop<Sleutel, Data>& k2);

template <class Sleutel, class Data>
class Knoop
{
public:
    friend class BinaireBoom<Sleutel, Data>;

    Knoop(const Sleutel& sleutel, const Data& data);
    Knoop(const Knoop<Sleutel, Data>& andere);
    Knoop<Sleutel, Data>& operator=(const Knoop<Sleutel, Data>& andere) = delete;
    Knoop(Knoop<Sleutel, Data>&& andere) = delete;
    Knoop<Sleutel, Data>& operator=(Knoop<Sleutel, Data>&& andere) = delete;
    virtual ~Knoop() = default;

    friend bool operator==<>(const Knoop<Sleutel, Data>& k1, const Knoop<Sleutel, Data>& k2);
    friend bool operator!=<>(const Knoop<Sleutel, Data>& k1, const Knoop<Sleutel, Data>& k3);

    const Sleutel& geef_sleutel() const;
    const Data& geef_data() const;

protected:
    BinaireBoom<Sleutel, Data>& geef_kind(const Richting& richting);

    Sleutel sleutel;
    Data data;

    Knoop<Sleutel, Data>* ouder;
    BinaireBoom<Sleutel, Data> links;
    BinaireBoom<Sleutel, Data> rechts;
};

/******************************************************************************/

template <class Sleutel, class Data>
Knoop<Sleutel, Data>::Knoop(const Sleutel& sleutel, const Data& data) : sleutel{sleutel}, data{data}, ouder{nullptr}
{
}

template <class Sleutel, class Data>
Knoop<Sleutel, Data>::Knoop(const Knoop<Sleutel, Data>& andere)
{
    sleutel = andere.sleutel;
    data = andere.data;
    ouder = nullptr; // Belangrijk voor root

    links = BinaireBoom<Sleutel, Data>{andere.links};
    if (links)
    {
        links->ouder = this;
    }

    rechts = BinaireBoom<Sleutel, Data>{andere.rechts};
    if (rechts)
    {
        rechts->ouder = this;
    }
}

template <class Sleutel, class Data>
bool operator==(const Knoop<Sleutel, Data>& k1, const Knoop<Sleutel, Data>& k2)
{
    return ((k1.sleutel == k2.sleutel) && (k1.data == k2.data));
}

template <class Sleutel, class Data>
bool operator!=(const Knoop<Sleutel, Data>& k1, const Knoop<Sleutel, Data>& k2)
{
    return !(k1 == k2);
}

template <class Sleutel, class Data>
const Sleutel& Knoop<Sleutel, Data>::geef_sleutel() const
{
    return sleutel;
}

template <class Sleutel, class Data>
const Data& Knoop<Sleutel, Data>::geef_data() const
{
    return data;
}

template <class Sleutel, class Data>
BinaireBoom<Sleutel, Data>& Knoop<Sleutel, Data>::geef_kind(const Richting& richting)
{
    if (richting == Richting::LINKS)
    {
        return links;
    }
    else if (richting == Richting::RECHTS)
    {
        return rechts;
    }
    else
    {
        throw "Verkeerde richting";
    }
}

#endif /* ZOEKKNOOP_H */
