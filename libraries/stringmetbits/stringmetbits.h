#ifndef STRINGMETBITS_H
#define STRINGMETBITS_H

#include <algorithm>
#include <cassert>
#include <string>

/**********************************************************************

   Class: StringMetBits

   beschrijving: klasse van strings met bitbehandelingsmogelijkheden.
   Elke StringMetBits wordt impliciet aangevuld met oneindig veel 0-bits en kan dus geen
   prefix zijn van een andere StringMetBits. Wel zou het karakter char(0) achteraan
   tot problemen leiden.
   Een StringMetBits mag dan ook het karakter char(0) niet bevatten en dit wordt gecheckt
   in de constructor, maar niet in wijzigingsoperatoren.

***************************************************************************/

class StringMetBits : public std::string
{
public:
    StringMetBits(const std::string& str);
    StringMetBits(const StringMetBits& str);
    StringMetBits(const char* str = "");

    void check0();
    bool bit(int i) const;
    int geefAantalBits() const;

    // Geeft -1 terug als strings gelijk zijn, anders plaats van meest linkse verschilbit
    int verschilplaats(const StringMetBits& sb) const;
};

StringMetBits::StringMetBits(const std::string& str) : std::string(str)
{
    check0();
}

StringMetBits::StringMetBits(const StringMetBits& str) : std::string(str)
{
    check0();
}

StringMetBits::StringMetBits(const char* str) : std::string(str)
{
    check0();
}

void StringMetBits::check0()
{
    const char null = static_cast<char>(0);

    for (int i = 0; i < size(); i++)
    {
        if ((*this)[i] == null)
        {
            throw "NULL in string";
        }
    }
}

bool StringMetBits::bit(int i) const
{
    if (i >= geefAantalBits())
    {
        return false;
    }

    unsigned int mask = static_cast<unsigned int>(1) << (7 - (i % 8));

    return ((*this)[i / 8] & mask);
}

int StringMetBits::geefAantalBits() const
{
    return (size() * 8);
}

int StringMetBits::verschilplaats(const StringMetBits& ander) const
{
    int minlengte = std::min(size(), ander.size());

    int i = 0;
    while ((i < minlengte) && ((*this)[i] == ander[i]))
    {
        i++;
    }

    if ((i == minlengte) && (size() == ander.size()))
    {
        return -1;
    }

    // While moet niet testen op overloop: er is een verschillende bit
    int bitplaats = (8 * i); // Plaats 1e bit in verschillend karakter
    while ((bitplaats < geefAantalBits()) && (bitplaats < ander.geefAantalBits())
           && (bit(bitplaats) == ander.bit(bitplaats)))
    {
        bitplaats++;
    }

    return bitplaats;
}

#endif
