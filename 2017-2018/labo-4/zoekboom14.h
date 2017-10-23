
#ifndef ZOEKBOOM_H
#define ZOEKBOOM_H

#include "zoekknoop14.h"

#include <algorithm>
// #include <functional>
#include <memory>

template <class Data>
class Zoekknoop;

template <class Data>
class Zoekboom : public std::unique_ptr<Zoekknoop<Data>>
{
public:
    Zoekboom() = default;
    Zoekboom(double x, double y, const Data& data);
    virtual ~Zoekboom() = default;

    Zoekboom(const Zoekboom<Data>& andere) = delete;
    Zoekboom<Data>& operator=(const Zoekboom<Data>& andere) = delete;

    Zoekboom(Zoekboom<Data>&& andere) = default;
    Zoekboom<Data>& operator=(Zoekboom<Data>&& andere) = default;

    int diepte() const;
    void voeg_toe(double x, double y, const Data& data);

protected:
    Zoekboom<Data>* zoek(double x, double y);
};

/******************************************************************************/

template <class Data>
Zoekboom<Data>::Zoekboom(double x, double y, const Data& data)
    : std::unique_ptr<Zoekknoop<Data>>{std::make_unique<Zoekknoop<Data>>(x, y, data)}
{
}

template <class Data>
int Zoekboom<Data>::diepte() const
{
    if (!(*this))
    {
        return -1; // Enkel bestaande knopen kunnen een diepte hebben
    }

    int max_hoogte_kinderen = std::max({(*this)->linksboven.diepte(),
                                        (*this)->rechtsboven.diepte(),
                                        (*this)->linksonder.diepte(),
                                        (*this)->rechtsonder.diepte()});

    return (max_hoogte_kinderen + 1);
}

template <class Data>
void Zoekboom<Data>::voeg_toe(double x, double y, const Data& data)
{
    Zoekboom<Data>* plaats = zoek(x, y);

    if (!(*plaats))
    {
        *plaats = Zoekboom<Data>{x, y, data};
    }
}

template <class Data>
Zoekboom<Data>* Zoekboom<Data>::zoek(double x, double y)
{
    Zoekboom<Data>* plaats = this;

    while (*plaats && ((*plaats)->x != x) && ((*plaats)->y != y))
    {
        if (x < (*plaats)->x)
        {
            if (y < (*plaats)->y)
            {
                plaats = &(*plaats)->linksonder;
            }
            else
            {
                plaats = &(*plaats)->linksboven;
            }
        }
        else
        {
            if (y < (*plaats)->y)
            {
                plaats = &(*plaats)->rechtsonder;
            }
            else
            {
                plaats = &(*plaats)->rechtsboven;
            }
        }
    };

    return plaats;
};

#endif /* ZOEKBOOM_H */
