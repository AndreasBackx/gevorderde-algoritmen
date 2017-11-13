
#include "Boom2D.h"

#include <iostream>
#include <fstream>
#include <cassert>

void test_boom()
{
    Boom2D boom;

    assert(boom.is_leeg());

    Punt2D p1{30, 40};
    Punt2D p2{70, 70};
    try
    {
        boom.voeg_toe(p1);
        boom.voeg_toe(Punt2D{5, 25});
        boom.voeg_toe(Punt2D{10, 12});
        boom.voeg_toe(p2);
        boom.voeg_toe(Punt2D{50, 30});
        boom.voeg_toe(Punt2D{35, 45});
    }
    catch (const char * ex)
    {
        std::cerr << ex << std::endl;
    }

    assert(!boom.is_leeg());

    //    \---(30,40)(X)
    //        +---(5,25)(Y)
    //        |   +---(10,12)(X)
    //        |   |   +---
    //        |   |   \---
    //        |   \---
    //        \---(70,70)(Y)
    //            +---(50,30)(X)
    //            |   +---(35,45)(Y)
    //            |   |   +---
    //            |   |   \---
    //            |   \---
    //            \---
    // std::cout << boom << std::endl;

    assert(boom.is_aanwezig(p1));
    assert(boom.is_aanwezig(p2));
    assert(!boom.is_aanwezig(Punt2D{18, 4}));
}

int main()
{
    test_boom();

    Boom2D ingelezen_boom;
    std::ifstream input_file("puntenverz.txt");

    std::cout << "Inlezen..." << std::endl;
    while (input_file)
    {
        int x;
        int y;

        input_file >> x >> y;
        try
        {
            ingelezen_boom.voeg_toe(Punt2D{x, y});
        }
        catch (const char * ex)
        {
            std::cerr << ex << std::endl;
        }
    }
    std::cout << "Bestand is ingelezen." << std::endl;

    for (int i = 0; i < 200; i += 10)
    {
        for (int j = 0; j < 200; j += 10)
        {
            Punt2D zoek_punt{i, j};
            Punt2D dichtste_buur{};
            unsigned int aantal_bezochte_knopen = 0;
            unsigned int kleinste_afstand = 0;

            ingelezen_boom.zoek_dichtste_buur(
                zoek_punt,
                dichtste_buur,
                aantal_bezochte_knopen,
                kleinste_afstand,
                true
            );

            std::cout << zoek_punt << " heeft buur " << dichtste_buur 
                << ",  gevonden in " << std::setw(5) << aantal_bezochte_knopen 
                << " doorzochte knopen met afstand " << kleinste_afstand 
                << std::endl;
        }
    }

    return 0;
}