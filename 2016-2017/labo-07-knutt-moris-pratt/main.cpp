
#include "knuthmorrispratt.h"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstring>
#include <cassert>

void schrijf_gevonden_resultaten(std::queue<const uchar*> gevonden, uint naald_lengte, const uchar* hooiberg)
{
    int aantal = gevonden.size();
    int i = 0;

    std::cout << "Aantal gevonden: " << gevonden.size() << std::endl;
    while (!gevonden.empty())
    {
        const uchar* resultaat = gevonden.front();
        gevonden.pop();

        i++;
        int resultaat_index = (resultaat - hooiberg);

        std::cout << "Gevonden op plaats " << resultaat_index
            << " (" << i << "/" << aantal << ")"
            << " : ";

        for (int j = 0; j < naald_lengte; j++)
        {
            std::cout << hooiberg[resultaat_index + j];
        }

        std::cout << std::endl;
    }
}

void test_prefix_functie()
{
    const uchar naald[]{'a', 'b', 'a', 'b', 'a', 'a'};
    int naald_lengte = sizeof (naald) / sizeof (naald[0]);
    KnuthMorrisPratt kmp(naald, naald_lengte);
    std::cout << kmp.to_string() << std::endl;

    // j:           0    1    2    3    4    5    6
    // naald[j]:    a    b    a    b    a    a
    // prefix[j]:  -1    0    0    1    2    3    1

    std::vector<int> prefix = kmp.get_prefix();
    assert(prefix[0] == -1);
    assert(prefix[1] == 0);
    assert(prefix[2] == 0);
    assert(prefix[3] == 1);
    assert(prefix[4] == 2);
    assert(prefix[5] == 3);
    assert(prefix[6] == 1);
}

void test_eenvoudig_zoeken()
{
    const uchar naald[]{'a', 'b', 'a', 'b'};
    int naald_lengte = sizeof (naald) / sizeof (naald[0]);

    const uchar hooiberg[]{'a', 'b', 'c', 'b', 'a', 'b', 'a', 'b', 'a', 'b'};
    int hooiberg_lengte = sizeof (hooiberg) / sizeof (hooiberg[0]);

    KnuthMorrisPratt kmp(naald, naald_lengte);

    std::queue<const uchar*> gevonden;
    kmp.zoek_eenvoudig(gevonden, hooiberg, hooiberg_lengte);
    assert(gevonden.size() == 2);
    schrijf_gevonden_resultaten(gevonden, naald_lengte, hooiberg);

    // j:                                    0   1   2   3   4   5   6   7   8   9   10
    // naald[j]:    a    b    a    b    X    a   b   c   b   a   b   a   b   a   b
    // prefix[j]:  -1    0    0    1    2    0   1   2   0   0   1   2   3   4   3   4
    // oplossing:                                            ^-------^------------
}

void test_shakespeare()
{
        std::ifstream in("shakespeare.txt");

        std::stringstream ss;
        ss << in.rdbuf();
        std::string shakespeare = ss.str();

        const uchar naald[]{'t', 'h', 'e'};
        int naald_lengte = sizeof (naald) / sizeof (naald[0]);

        const uchar* hooiberg = reinterpret_cast<const uchar*>(shakespeare.c_str());
        int hooiberg_lengte = shakespeare.size();

        KnuthMorrisPratt kmp_shakespeare(naald, naald_lengte);

        std::queue<const uchar*> gevonden;
        kmp_shakespeare.zoek_eenvoudig(gevonden, hooiberg, hooiberg_lengte);
        // schrijf_gevonden_resultaten(gevonden, naald_lengte, hooiberg);
        std::cout << "Aantal gevonden: " << gevonden.size() << std::endl;

        assert(gevonden.size() == 44074);
}

// 44074 voor "the" case insensitive
// http://c-faq.com/stdio/getcharc.html

int main()
{
    test_prefix_functie();
    test_eenvoudig_zoeken();
    test_shakespeare();

    return 0;
}