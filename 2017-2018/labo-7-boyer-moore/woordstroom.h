#ifndef WOORDSTROOM_H
#define WOORDSTROOM_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**********************************************************************

   Class: Woordstroom

   beschrijving: Een Woordstroom beheert een bestand (naam opgegeven in de constructor).
   Met de >>-operator lees je strings in: afwisselend krijg je een woord en een string met
   karakters tussen twee opeenvolgende woorden.

***************************************************************************/

class Woordstroom : protected std::ifstream
{
public:

    Woordstroom(const char* c) : std::ifstream(c)
    {
        laatstgelezen = get();
    };

    bool fail()
    {
        return std::ifstream::fail();
    }

    friend Woordstroom& operator>>(Woordstroom& ws, std::string& str)
    {
        str.clear();
        str += ws.laatstgelezen;
        char c = ws.get();
        while (!ws.fail() && isalpha(ws.laatstgelezen) == isalpha(c))
        {
            str += c;
            c = ws.get();
        };
        ws.laatstgelezen = c;
        return ws;
    };

protected:
    char laatstgelezen;
};

#endif
