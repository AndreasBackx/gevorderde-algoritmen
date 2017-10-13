
#include "dnasequentie.h"
#include "bottelmachine.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <iomanip>

std::vector<DNASequentie> lees_bestand(const std::string& bestandsnaam)
{
    std::vector<DNASequentie> lijnen;

    std::ifstream bestand(bestandsnaam);

    std::string lijn;
    while(bestand >> lijn)
    {
        lijnen.push_back(DNASequentie{lijn});
    }

    bestand.close();

    return lijnen;
}

void test_labo()
{
    std::vector<DNASequentie> human_dna{lees_bestand("human.dna")};
    std::cout << "Mens:" << std::endl;
    for (size_t i = 0; i < human_dna.size(); i++)
    {
        std::cout << i << ": " << human_dna[i] << std::endl;
    }
    std::cout << std::endl;

    std::vector<DNASequentie> chimp_dna{lees_bestand("chimp.dna")};
    std::cout << "Aap:" << std::endl;
    for (size_t i = 0; i < chimp_dna.size(); i++)
    {
        std::cout << i << ": " << chimp_dna[i] << std::endl;
    }
    std::cout << std::endl;

    constexpr int AANTAL_OPLOSSINGEN = 75;
    constexpr int OPLOSSINGEN[AANTAL_OPLOSSINGEN][3] =
    {   // human, chimp, oplossing
        {0, 69 ,24},
        {1, 63 ,10},
        {2, 27 ,25},
        {3, 45 ,26},
        {4, 46 ,12},
        {5, 25 ,30},
        {6, 61 ,10},
        {7, 57 ,12},
        {8, 74 ,35},
        {9, 59 ,9},
        {10, 43 ,26},
        {11, 47 ,25},
        {12, 33 ,36},
        {13, 66 ,23},
        {14, 18 ,12},
        {15, 40 ,7},
        {16, 44 ,11},
        {17, 6 ,11},
        {18, 32 ,17},
        {19, 2 ,23},
        {20, 54 ,23},
        {21, 62 ,13},
        {22, 4 ,13},
        {23, 15 ,21},
        {24, 48 ,26},
        {25, 12 ,21},
        {26, 21 ,25},
        {27, 19 ,34},
        {28, 8 ,20},
        {29, 3 ,17},
        {30, 9 ,20},
        {31, 73 ,18},
        {32, 1 ,22},
        {33, 22 ,23},
        {34, 11 ,24},
        {35, 24 ,10},
        {36, 13 ,5},
        {37, 5 ,23},
        {38, 65 ,12},
        {39, 7 ,25},
        {40, 35 ,24},
        {41, 60 ,28},
        {42, 10 ,8},
        {43, 49 ,22},
        {44, 29 ,21},
        {45, 67 ,28},
        {46, 16 ,7},
        {47, 64 ,28},
        {48, 71 ,28},
        {49, 50 ,11},
        {50, 37 ,27},
        {51, 56 ,30},
        {52, 17 ,5},
        {53, 38 ,26},
        {54, 31 ,25},
        {55, 23 ,3},
        {56, 68 ,14},
        {57, 36 ,19},
        {58, 53 ,12},
        {59, 51 ,23},
        {60, 39 ,7},
        {61, 42 ,20},
        {62, 55 ,20},
        {63, 52 ,24},
        {64, 30 ,24},
        {65, 70 ,24},
        {66, 14 ,29},
        {67, 34 ,17},
        {68, 26 ,13},
        {69, 0 ,14},
        {70, 20 ,5},
        {71, 58 ,20},
        {72, 72 ,28},
        {73, 41 ,21},
        {74, 28 ,18}
    };

    for (int i = 0; i < AANTAL_OPLOSSINGEN; i++)
    {
        const int HUMAN_INDEX = OPLOSSINGEN[i][0];
        const int CHIMP_INDEX = OPLOSSINGEN[i][1];
        const int OPLOSSING = OPLOSSINGEN[i][2];
        const int AFSTAND = human_dna[HUMAN_INDEX].d(chimp_dna[CHIMP_INDEX]);
        const int CONVENT_AFSTAND = human_dna[HUMAN_INDEX].d_volgens_conventie(chimp_dna[CHIMP_INDEX]);

        std::cout << "Check " << std::setw(2) << i << " voor mens " << std::setw(2) << HUMAN_INDEX
                  << " en aap " << std::setw(2) << CHIMP_INDEX << " geeft (" << std::setw(2) << AFSTAND
                  << " & " << std::setw(2) << CONVENT_AFSTAND << ") en verwacht " << std::setw(2) << OPLOSSING;

        assert(human_dna[HUMAN_INDEX].d(chimp_dna[CHIMP_INDEX]) == OPLOSSING);
        assert(human_dna[HUMAN_INDEX].d_volgens_conventie(chimp_dna[CHIMP_INDEX]) == OPLOSSING);

        std::cout << " [ OK ]" << std::endl;
    }

    std::cout << "Test DNA OK" << std::endl;
}

void test_labo_uitbreiding()
{
    Bottelmachine bm;
    std::vector<int> test_reeks = {0, 3, 2, 7, 1};

    constexpr int AANTAL_OPLOSSINGEN = 7;
    constexpr int OPLOSSINGEN[AANTAL_OPLOSSINGEN][2] =
    {
        {13,4},
        {3,1},
        {5,2},
        {8,2},
        {10,2},
        {12,3},
        {1,1}
    };

    for (size_t i = 0; i < AANTAL_OPLOSSINGEN; i++)
    {
        const int SOM = OPLOSSINGEN[i][0];
        const int AANTAL_FLESSEN = bm.min_aantal_flessen(SOM, test_reeks);
        const int OPLOSSING = OPLOSSINGEN[i][1];

        std::cout << "Check aantal flessen nodig voor " << SOM << " geeft ("
                  << AANTAL_FLESSEN << ") en verwacht " << OPLOSSING;

        assert(AANTAL_FLESSEN == OPLOSSING);

        std::cout << " [ OK ]" << std::endl;
    }

    assert(bm.min_aantal_flessen(13, test_reeks) == 4);
    assert(bm.min_aantal_flessen(3, test_reeks) == 1);
    assert(bm.min_aantal_flessen(5, test_reeks) == 2);
    assert(bm.min_aantal_flessen(8, test_reeks) == 2);
    assert(bm.min_aantal_flessen(10, test_reeks) == 2);
    assert(bm.min_aantal_flessen(12, test_reeks) == 3);
    assert(bm.min_aantal_flessen(1, test_reeks) == 1);

    std::cout << "Test bottelmachine OK" << std::endl;
}

int main()
{
    test_labo();
    test_labo_uitbreiding();

    return 0;
}
