
#include "bottelmachine.h"
#include "dnasequentie.h"

#include <cassert>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

void test_labo_uitbreiding()
{
    Bottelmachine bm;

    assert(bm.min_aantal_flessen(450, {200, 50}) == 3);
    assert(bm.min_aantal_flessen(450, {200, 50}) == 3);

    std::vector<int> test_reeks = {0, 3, 2, 7, 1};

    constexpr int AANTAL_OPLOSSINGEN = 7;
    constexpr int OPLOSSINGEN[AANTAL_OPLOSSINGEN][2] = {{13, 4}, {3, 1}, {5, 2}, {8, 2}, {10, 2}, {12, 3}, {1, 1}};

    for (size_t i = 0; i < AANTAL_OPLOSSINGEN; i++)
    {
        const int SOM = OPLOSSINGEN[i][0];
        const int AANTAL_FLESSEN = bm.min_aantal_flessen_uitgebreid_bottom_up(SOM, test_reeks);
        assert(AANTAL_FLESSEN == bm.min_aantal_flessen_uitgebreid_top_down(SOM, test_reeks));
        const int OPLOSSING = OPLOSSINGEN[i][1];

        std::cout << "Check aantal flessen nodig voor " << SOM << " geeft (" << AANTAL_FLESSEN << ") en verwacht "
                  << OPLOSSING;

        assert(AANTAL_FLESSEN == OPLOSSING);

        std::cout << " [ OK ]" << std::endl;
    }

    std::cout << "Test bottelmachine OK" << std::endl;
}

void vergelijk_mens_aap()
{
    std::vector<DNASequentie> aap_dna;

    std::ifstream bestand{"chimp.dna"};
    DNASequentie aap;

    while (bestand >> aap)
    {
        aap_dna.push_back(aap);
    }

    bestand.close();
    bestand.clear();

    bestand.open("human.dna");
    DNASequentie mens;

    auto start_time = std::chrono::steady_clock::now();

    int i = 0;
    while (bestand >> mens)
    {
        size_t min_index = 0;
        int min_afstand = std::numeric_limits<int>::max();

        assert(aap_dna.size() < std::numeric_limits<int>::max());
        for (int j = 0; j < static_cast<int>(aap_dna.size()); j++)
        {
            int afstand = mens.d_bottom_up_volgens_conventie(aap_dna[j]);
            // int afstand = mens.d_top_down_volgens_conventie(aap_dna[j]);
            assert(afstand == mens.d_bottom_up(aap_dna[j]));
            assert(afstand == mens.d_top_down_volgens_conventie(aap_dna[j]));

            if (afstand < min_afstand)
            {
                min_afstand = afstand;
                min_index = j;
            }
        }

        std::cout << "Menselijk " << i << " komt overeen met chimpansees " << min_index << " met afstand "
                  << min_afstand << std::endl;

        i++;
    }

    auto finish_time = std::chrono::steady_clock::now();

    bestand.close();

    std::cout << "Verstreken tijd voor DNA vergelijkingen: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time).count() << " ms"
              << std::endl;
}

int main()
{
    vergelijk_mens_aap();
    test_labo_uitbreiding();

    return 0;
}
