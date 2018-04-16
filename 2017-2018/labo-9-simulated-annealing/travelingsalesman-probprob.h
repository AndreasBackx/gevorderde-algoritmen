#ifndef TRAVELINGSALESMANPROBPROB_H
#define TRAVELINGSALESMANPROBPROB_H

#include "probprob.h"
#include "stad.h"

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

class TravelingSalesmanProbProb : public ProbProb<Stad>
{
public:
    TravelingSalesmanProbProb(const std::vector<Stad>& steden);

protected:
    bool isOplossing(const std::vector<Stad>& deeloplossing);
    double f(const std::vector<Stad>& oplossing);
    std::vector<std::pair<Stad, double>> geefVoortzetting(const std::vector<Stad>& deeloplossing);
    bool bevat_deeloplossing_stad(const std::vector<Stad>& deeloplossing, const Stad& stad) const;

private:
    std::vector<Stad> steden;
};

TravelingSalesmanProbProb::TravelingSalesmanProbProb(const std::vector<Stad>& steden) : steden(steden)
{
}

bool TravelingSalesmanProbProb::isOplossing(const std::vector<Stad>& deeloplossing)
{
    return (deeloplossing.size() == steden.size());
}

double TravelingSalesmanProbProb::f(const std::vector<Stad>& oplossing)
{
    assert(oplossing.size() > 1);

    double som = 0;

    for (int i = 1; i < oplossing.size(); i++)
    {
        som += oplossing[i].bereken_afstand(oplossing[i - 1]);
    }
    som += oplossing[oplossing.size() - 1].bereken_afstand(oplossing[0]);

    return som;
}

std::vector<std::pair<Stad, double>> TravelingSalesmanProbProb::geefVoortzetting(const std::vector<Stad>& deeloplossing)
{
    std::vector<std::pair<Stad, double>> voortzetting;

    for (const auto& stad : steden)
    {
        if (!bevat_deeloplossing_stad(deeloplossing, stad))
        {
            // Groter == heuristisch beter
            double afstand_laatste_stad = 0;

            if (deeloplossing.size() > 0)
            {
                afstand_laatste_stad = stad.bereken_afstand(deeloplossing[deeloplossing.size() - 1]);
            }

            voortzetting.push_back(std::make_pair(stad, (1 / afstand_laatste_stad)));
        }

        // Sorteren of shuffelen is niet nodig wegens de manier van berekenen in probprob.h (volgens Cnops)
    }

    return voortzetting;
}

bool TravelingSalesmanProbProb::bevat_deeloplossing_stad(const std::vector<Stad>& deeloplossing, const Stad& stad) const
{
    // Vermoedelijk is een steden hiervoor beter een set voor performantie

    for (const auto& s : deeloplossing)
    {
        if (s == stad)
        {
            return true;
        }
    }

    return false;
}

#endif
