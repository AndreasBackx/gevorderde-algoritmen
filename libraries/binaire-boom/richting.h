
#ifndef RICHTING_H 
#define RICHTING_H

enum class Richting
{
    LINKS,
    RECHTS
};

// Moet inline door templates (anders multiple definitions)
inline Richting operator!(const Richting& richting)
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
        throw "Verkeerde richting";
    }
}

#endif