#ifndef WOORDSTROOM_H
#define WOORDSTROOM_H

#include <fstream>
#include <iostream>
#include <string>

/**********************************************************************

   Class: Woordstroom

   beschrijving: Een Woordstroom beheert een bestand (naam opgegeven in de constructor).
   Met de >>-operator lees je strings in: afwisselend krijg je een woord en een string met
   karakters tussen twee opeenvolgende woorden.

***************************************************************************/

class Woordstroom : public std::ifstream
{
public:
    // constructor opent het bestand.
    Woordstroom(const char* c);

    bool fail() const;

    friend Woordstroom& operator>>(Woordstroom& ws, std::string& str);

protected:
    bool zijn_zelfde_type(char a, char b) const;
};

Woordstroom::Woordstroom(const char* c) : std::ifstream(c)
{
}

bool Woordstroom::fail() const
{
    return std::ifstream::fail();
}

Woordstroom& operator>>(Woordstroom& ws, std::string& str)
{
    str.clear();
    char eerste_char = ws.peek();

    while ((ws.peek() != std::char_traits<char>::eof()) && ws.zijn_zelfde_type(eerste_char, ws.peek()))
    {
        str += static_cast<char>(ws.get());
    }

    return ws;
}

bool Woordstroom::zijn_zelfde_type(char a, char b) const
{
    bool is_a_alpha = (isalpha(a) > 0);
    bool is_b_alpha = (isalpha(b) > 0);

    return (is_a_alpha == is_b_alpha);
}
#endif
