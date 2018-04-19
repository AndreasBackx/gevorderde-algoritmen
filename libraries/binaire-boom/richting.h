
#ifndef RICHTING_H
#define RICHTING_H

enum class Richting
{
    LINKS,
    RECHTS
};

Richting operator!(const Richting& richting)
{
    if (richting == Richting::LINKS)
    {
        return Richting::RECHTS;
    }
    else if (richting == Richting::RECHTS)
    {
        return Richting::LINKS;
    }
    else {
        throw "Verkeerde richting";
    }
}

#endif
