
#ifndef ZOEKKNOOP_H
#define ZOEKKNOOP_H

#include "zoekboom14.h"

#include <memory>

template <class Data>
class Zoekboom;

template <class Data>
class Zoekknoop
{
public:
    friend class Zoekboom<Data>;

    Zoekknoop(double x, double y, const Data& data);
    virtual ~Zoekknoop() = default;

    Zoekknoop(const Zoekknoop<Data>& andere) = delete;
    Zoekknoop<Data>& operator=(const Zoekknoop<Data>& andere) = delete;

    Zoekknoop(Zoekknoop<Data>&& andere) = delete;
    Zoekknoop<Data>& operator=(Zoekknoop<Data>&& andere) = delete;

protected:
    double x;
    double y;
    Data data;

    Zoekboom<Data> linksboven;
    Zoekboom<Data> rechtsboven;
    Zoekboom<Data> linksonder;
    Zoekboom<Data> rechtsonder;
};

/******************************************************************************/

template <class Data>
Zoekknoop<Data>::Zoekknoop(double x, double y, const Data& data) : x{x}, y{y}, data{data}
{
}

#endif /* ZOEKKNOOP_H */
