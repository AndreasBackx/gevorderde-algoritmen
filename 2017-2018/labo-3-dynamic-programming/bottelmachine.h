
#include <cassert>
#include <limits>
#include <unordered_map>
#include <vector>

class Bottelmachine
{
public:
    int min_aantal_flessen(int inhoud, const std::vector<int>& fles_types);
    int min_aantal_flessen_uitgebreid_bottom_up(int inhoud, const std::vector<int>& flessen);
    int min_aantal_flessen_uitgebreid_top_down(int inhoud, const std::vector<int>& flessen);

private:
    int bereken_min_aantal_flessen(int inhoud,
                                   const std::vector<int>& fles_types,
                                   std::unordered_map<int, int>& oplossingen);
    int bereken_min_aantal_flessen_uitgebreid_top_down(
            int inhoud,
            const std::vector<int>& flessen,
            int aantal_flessen,
            std::unordered_map<int, std::unordered_map<int, int>>& oplossingen);

    static constexpr int ONGELDIGE_COMBINATIE = -1;
};

// bv. 90 cl met volgende flessen {80, 23, 22, 1}
//
// greedy: 80 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 = 11 flessen
// DP: 23 + 22 + 23 + 22 = 4 flessen
int Bottelmachine::min_aantal_flessen(int inhoud, const std::vector<int>& fles_types)
{
    if (inhoud <= 0 || fles_types.empty())
    {
        return 0;
    }

    std::unordered_map<int, int> oplossingen;

    int min_aantal = bereken_min_aantal_flessen(inhoud, fles_types, oplossingen);

    if (min_aantal == ONGELDIGE_COMBINATIE)
    {
        return 0;
    }

    return min_aantal;
}

int Bottelmachine::bereken_min_aantal_flessen(int inhoud,
                                              const std::vector<int>& fles_types,
                                              std::unordered_map<int, int>& oplossingen)
{
    if (inhoud < 0)
    {
        return ONGELDIGE_COMBINATIE;
    }
    else if (inhoud == 0)
    {
        return 0;
    }

    auto gevonden_aantal = oplossingen.find(inhoud);
    if (gevonden_aantal != oplossingen.end())
    {
        return gevonden_aantal->second;
    }

    int min_aantal = std::numeric_limits<int>::max();
    for (int fles_inhoud : fles_types)
    {
        int huidig_aantal = bereken_min_aantal_flessen((inhoud - fles_inhoud), fles_types, oplossingen);

        if ((huidig_aantal != ONGELDIGE_COMBINATIE) && (huidig_aantal < min_aantal))
        {
            min_aantal = huidig_aantal;
        }
    }

    if (min_aantal == std::numeric_limits<int>::max())
    {
        oplossingen[inhoud] = ONGELDIGE_COMBINATIE;
    }
    else
    {
        oplossingen[inhoud] = (min_aantal + 1);
    }

    return oplossingen[inhoud];
}

int Bottelmachine::min_aantal_flessen_uitgebreid_top_down(int inhoud, const std::vector<int>& flessen)
{
    if (inhoud <= 0 || flessen.empty())
    {
        return 0;
    }

    std::unordered_map<int, std::unordered_map<int, int>> oplossingen;

    int min_aantal = bereken_min_aantal_flessen_uitgebreid_top_down(inhoud, flessen, flessen.size(), oplossingen);

    if (min_aantal == ONGELDIGE_COMBINATIE)
    {
        return 0;
    }

    return ONGELDIGE_COMBINATIE;
}

int Bottelmachine::bereken_min_aantal_flessen_uitgebreid_top_down(
        int inhoud,
        const std::vector<int>& flessen,
        int aantal_flessen,
        std::unordered_map<int, std::unordered_map<int, int>>& oplossingen)
{
    if (inhoud < 0)
    {
        return ONGELDIGE_COMBINATIE;
    }
    else if (inhoud == 0)
    {
        return 0;
    }
    else if (aantal_flessen == 0)
    {
        return 0;
    }

    auto gevonden_inhoud = oplossingen.find(inhoud);
    if (gevonden_inhoud != oplossingen.end())
    {
        auto gevonden_inhoud_aantal = oplossingen.at(inhoud).find(aantal_flessen);
        if (gevonden_inhoud_aantal != oplossingen.at(inhoud).end())
        {
            return gevonden_inhoud_aantal->second;
        }
    }

    const int huidige_fles = flessen[aantal_flessen - 1];
    if (inhoud == huidige_fles)
    {
        oplossingen[inhoud][aantal_flessen] = 1;
    }
    else
    {
        int vorige_subset
                = bereken_min_aantal_flessen_uitgebreid_top_down(inhoud, flessen, (aantal_flessen - 1), oplossingen);
        bool is_vorige_subset_geldig = (vorige_subset > 0);
        bool mag_oplossing_overschrijven
                = (oplossingen[inhoud][aantal_flessen] == 0) || (vorige_subset < oplossingen[inhoud][aantal_flessen]);

        if (is_vorige_subset_geldig && mag_oplossing_overschrijven)
        // Vorige subset heeft de som al bereikt
        {
            oplossingen[inhoud][aantal_flessen] = vorige_subset;
        }

        if (inhoud >= huidige_fles)
        {
            vorige_subset = bereken_min_aantal_flessen_uitgebreid_top_down(
                    (inhoud - huidige_fles), flessen, (aantal_flessen - 1), oplossingen);
            is_vorige_subset_geldig = (vorige_subset > 0);
            mag_oplossing_overschrijven = (oplossingen[inhoud][aantal_flessen] == 0)
                                          || (vorige_subset < oplossingen[inhoud][aantal_flessen]);

            if (is_vorige_subset_geldig && mag_oplossing_overschrijven)
            // De combinatie vorige som met huidige fles zorgt ervoor dat de som bereikt wordt
            {
                oplossingen[inhoud][aantal_flessen] = (vorige_subset + 1);
            }
        }
    }

    return oplossingen[inhoud][aantal_flessen];
}

// Subset-sum problem
// https://www.youtube.com/watch?v=zKwwjAkaXLI
int Bottelmachine::min_aantal_flessen_uitgebreid_bottom_up(int inhoud, const std::vector<int>& flessen)
{
    if (inhoud <= 0 || flessen.empty())
    {
        return 0;
    }

    assert(flessen.size() < std::numeric_limits<int>::max());
    const int FLESSEN_GROOTTE = static_cast<int>(flessen.size());
    const int INHOUD_GROOTTE = (inhoud + 1);

    std::vector<std::vector<int>> oplossingen(FLESSEN_GROOTTE, std::vector<int>(INHOUD_GROOTTE, 0));

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
                if (flessen[i] == j)
                // De huidige fles zorgt voor op zichzelf voor dat de som bereikt wordt
                {
                    oplossingen[i][j] = 1;
                }
                else
                {
                    int vorige_subset = oplossingen[i - 1][j];
                    bool is_vorige_subset_geldig = (vorige_subset > 0);
                    bool mag_oplossing_overschrijven = (oplossingen[i][j] == 0) || (vorige_subset < oplossingen[i][j]);

                    if (is_vorige_subset_geldig && mag_oplossing_overschrijven)
                    // Vorige subset heeft de som al bereikt
                    {
                        oplossingen[i][j] = vorige_subset;
                    }

                    if (j >= flessen[i])
                    {
                        vorige_subset = oplossingen[i - 1][j - flessen[i]];
                        is_vorige_subset_geldig = (vorige_subset > 0);
                        mag_oplossing_overschrijven = (oplossingen[i][j] == 0) || (vorige_subset < oplossingen[i][j]);

                        if (is_vorige_subset_geldig && mag_oplossing_overschrijven)
                        // De combinatie vorige som met huidige fles zorgt ervoor dat de som bereikt wordt
                        {
                            oplossingen[i][j] = (vorige_subset + 1);
                        }
                    }
                }
            }
        }
    }

    // 0 stelt hier geen oplossing voor
    return oplossingen[FLESSEN_GROOTTE - 1][INHOUD_GROOTTE - 1];
}
