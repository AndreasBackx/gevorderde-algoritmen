#include "thompsonna.h"

#include <cassert>
#include <fstream>
#include <map>
#include <set>
#include <stack>
#include <utility>

#ifndef ZOEKNA_H
#define ZOEKNA_H

class ZoekNA
{
public:
    ZoekNA(const ThompsonNA& nda);
    ZoekNA(const ZoekNA& other) = default;
    ZoekNA(ZoekNA&& other) = default;
    ZoekNA& operator=(const ZoekNA& other) = default;
    ZoekNA& operator=(ZoekNA&& other) = default;
    virtual ~ZoekNA() = default;

    std::set<int> bevat_regexp(const std::string& regel) const;
    std::vector<std::pair<int, std::string>> grep_bestand(const std::string& bestandsnaam) const;

protected:
    std::set<int> bepaal_epsilon_sluiting(const std::set<int>& geactiveerde_staten) const;
    std::set<int> bepaal_volgende_geactiveerde_staten(const std::set<int>& geactiveerde_staten, uchar karakter) const;

    const ThompsonNA& nda;
    std::vector<std::multimap<uchar, int>> overgangstabel;
};

ZoekNA::ZoekNA(const ThompsonNA& nda) : nda{nda}, overgangstabel(nda.geefAantalStatenbits())
{
    for (int i = 0; i < nda.geefAantalStatenbits(); i++)
    {
        const Verbinding& verbinding = nda[i];

        overgangstabel[verbinding.bron].insert(std::make_pair(verbinding.karakter, verbinding.doel));
    }
}

std::set<int> ZoekNA::bepaal_epsilon_sluiting(const std::set<int>& geactiveerde_staten) const
{
    std::set<int> nieuwe_geactiveerde_staten(geactiveerde_staten);
    std::stack<int> te_overlopen_staten;

    for (const auto& staat : geactiveerde_staten)
    {
        te_overlopen_staten.push(staat);
    }

    while (!te_overlopen_staten.empty())
    {
        int huidige_staat = te_overlopen_staten.top();
        te_overlopen_staten.pop();

        auto overgangen = overgangstabel[huidige_staat].equal_range(epsilon);

        for (auto it = overgangen.first; it != overgangen.second; it++)
        {
            nieuwe_geactiveerde_staten.insert(it->second);
            te_overlopen_staten.push(it->second);
        }
    }

    return nieuwe_geactiveerde_staten;
}

std::set<int> ZoekNA::bepaal_volgende_geactiveerde_staten(const std::set<int>& geactiveerde_staten,
                                                          uchar karakter) const
{
    std::set<int> nieuwe_geactiveerde_staten;

    for (const auto& staat : geactiveerde_staten)
    {
        auto overgangen = overgangstabel[staat].equal_range(karakter);

        for (auto it = overgangen.first; it != overgangen.second; it++)
        {
            nieuwe_geactiveerde_staten.insert(it->second);
        }
    }

    nieuwe_geactiveerde_staten = bepaal_epsilon_sluiting(nieuwe_geactiveerde_staten);

    return nieuwe_geactiveerde_staten;
}

std::set<int> ZoekNA::bevat_regexp(const std::string& regel) const
{
    if (regel.size() == 0)
    {
        return std::set<int>{};
    }

    std::set<int> gevonden_indexen;
    std::set<int> geactiveerde_staten;

    for (int i = 0; i < regel.size(); i++)
    {
        geactiveerde_staten.insert(0);
        geactiveerde_staten = bepaal_epsilon_sluiting(geactiveerde_staten);

        std::set<int> nieuwe_geactiveerde_staten = bepaal_volgende_geactiveerde_staten(geactiveerde_staten, regel[i]);

        if (nieuwe_geactiveerde_staten.find(nda.geefAantalStatenbits() - 1) != nieuwe_geactiveerde_staten.end())
        {
            gevonden_indexen.insert(i);
        }

        geactiveerde_staten = std::move(nieuwe_geactiveerde_staten);
    }

    return gevonden_indexen;
}

std::vector<std::pair<int, std::string>> ZoekNA::grep_bestand(const std::string& bestandsnaam) const
{
    std::vector<std::pair<int, std::string>> gevonden_regels;

    std::ifstream in(bestandsnaam);
    assert(in);

    int regelnr = 1;
    std::string regel;
    while (getline(in, regel))
    {
        std::set<int> gevonden_indexen = bevat_regexp(regel);

        if (!gevonden_indexen.empty())
        {
            gevonden_regels.push_back(std::make_pair(regelnr, regel));
        }

        regelnr++;
    }

    in.close();

    return gevonden_regels;
}

#endif
