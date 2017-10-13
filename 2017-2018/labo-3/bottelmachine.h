
#include <vector>
#include <cassert>
#include <limits>
#include <iostream>

// Subset-sum problem
// https://www.youtube.com/watch?v=zKwwjAkaXLI

class Bottelmachine
{
public:
    int min_aantal_flessen(int inhoud, const std::vector<int>& flessen);
};

int Bottelmachine::min_aantal_flessen(int inhoud, const std::vector<int>& flessen)
{
    assert(inhoud > 0);
    assert(!flessen.empty());

    const size_t FLESSEN_GROOTTE = flessen.size();
    const size_t INHOUD_GROOTTE = (inhoud + 1);

    std::vector<std::vector<int>> oplossingen(FLESSEN_GROOTTE, std::vector<int>(INHOUD_GROOTTE, false));

    for (size_t i = 0; i < FLESSEN_GROOTTE; i++)
    {
        assert(flessen[i] >= 0);

        for (size_t j = 0; j < INHOUD_GROOTTE; j++)
        {
            if (i == 0)
            {
                oplossingen[i][j] = (j == 0 || flessen[i] == j);
            }
            else
            {
                if (oplossingen[i - 1][j] > 0) // Vorige subset heeft de som al bereikt
                {
                    oplossingen[i][j] = oplossingen[i - 1][j];
                }
                else if ((flessen[i] == j)) // Het huidige element zorgt voor op zichzelf voor dat de som bereikt wordt
                {
                    oplossingen[i][j] = 1;
                }
                else if ((j >= flessen[i]) && oplossingen[i - 1][j - flessen[i]]) // De combinatie vorige met huidige zorgt ervoor dat de som bereikt wordt
                {
                    oplossingen[i][j] = (oplossingen[i - 1][j - flessen[i]] + 1);
                }
            }
        }
    }

    // Uitschrijven oplossingen

//    for (const std::vector<int>& rij : oplossingen)
//    {
//        for (int is_mogelijk : rij)
//        {
//            std::cout << " " << is_mogelijk;
//        }
//        std::cout << std::endl;
//    }

    return oplossingen[FLESSEN_GROOTTE - 1][INHOUD_GROOTTE - 1];
}
