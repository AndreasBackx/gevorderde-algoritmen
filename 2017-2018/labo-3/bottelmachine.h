
#include <cassert>
#include <limits>
#include <unordered_map>
#include <vector>

class Bottelmachine
{
public:
    int min_aantal_flessen(int inhoud, const std::vector<int>& fles_types);
    int min_aantal_flessen_uitgebreid_bottom_up(int inhoud, const std::vector<int>& flessen);

private:
    int bereken_min_aantal_flessen(int inhoud,
                                   const std::vector<int>& fles_types,
                                   std::unordered_map<int, int>& aantal_flessen_per_inhoud);
};

// bv. 90 cl met volgende flessen {80, 23, 22, 1}
//
// greedy: 80 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 = 11 flessen
// DP: 23 + 22 + 23 + 22 = 4 flessen
int Bottelmachine::min_aantal_flessen(int inhoud, const std::vector<int>& fles_types)
{
    std::unordered_map<int, int> aantal_flessen_per_inhoud;

    int min_aantal = bereken_min_aantal_flessen(inhoud, fles_types, aantal_flessen_per_inhoud);

    if (min_aantal < 0)
    {
        throw "Geen oplossing gevonden";
    }

    return min_aantal;
}

int Bottelmachine::bereken_min_aantal_flessen(int inhoud,
                                              const std::vector<int>& fles_types,
                                              std::unordered_map<int, int>& aantal_flessen_per_inhoud)
{
    if (inhoud < 0)
    {
        return -1;
    }
    else if (inhoud == 0)
    {
        return 0;
    }

    auto gevonden_aantal = aantal_flessen_per_inhoud.find(inhoud);
    if (gevonden_aantal != aantal_flessen_per_inhoud.end())
    {
        return gevonden_aantal->second;
    }

    int min_aantal = std::numeric_limits<int>::max();
    for (int fles_inhoud : fles_types)
    {
        int huidig_aantal = bereken_min_aantal_flessen((inhoud - fles_inhoud), fles_types, aantal_flessen_per_inhoud);

        if ((huidig_aantal >= 0) && (huidig_aantal < min_aantal))
        {
            min_aantal = huidig_aantal;
        }
    }

    if (min_aantal == std::numeric_limits<int>::max())
    {
        min_aantal = -1;
    }

    aantal_flessen_per_inhoud[inhoud] = (min_aantal + 1);
    return aantal_flessen_per_inhoud[inhoud];
}

// Subset-sum problem
// https://www.youtube.com/watch?v=zKwwjAkaXLI
int Bottelmachine::min_aantal_flessen_uitgebreid_bottom_up(int inhoud, const std::vector<int>& flessen)
{
    assert(inhoud > 0);
    assert(!flessen.empty());

    assert(flessen.size() < std::numeric_limits<int>::max());
    const int FLESSEN_GROOTTE = static_cast<int>(flessen.size());
    const int INHOUD_GROOTTE = (inhoud + 1);

    std::vector<std::vector<int>> oplossingen(FLESSEN_GROOTTE, std::vector<int>(INHOUD_GROOTTE, false));

    for (int i = 0; i < FLESSEN_GROOTTE; i++)
    {
        assert(flessen[i] >= 0);

        for (int j = 0; j < INHOUD_GROOTTE; j++)
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
                else if ((flessen[i] == j)) // Het huidige element zorgt voor op
                // zichzelf voor dat de som bereikt
                // wordt
                {
                    oplossingen[i][j] = 1;
                }
                else if ((j >= flessen[i]) && oplossingen[i - 1][j - flessen[i]]) // De combinatie
                // vorige met
                // huidige
                // zorgt ervoor dat de som bereikt
                // wordt
                {
                    oplossingen[i][j] = (oplossingen[i - 1][j - flessen[i]] + 1);
                }
            }
        }
    }

    return oplossingen[FLESSEN_GROOTTE - 1][INHOUD_GROOTTE - 1];
}
