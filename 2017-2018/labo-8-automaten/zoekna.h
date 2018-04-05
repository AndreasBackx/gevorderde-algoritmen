#include "thompsonna.h"

#include <map>
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

protected:
    std::vector<bool> bepaal_epsilon_sluiting(const std::vector<bool>& geactiveerde_staten) const;
    std::vector<bool> bepaal_volgende_geactiveerde_staten(const std::vector<bool>& geactiveerde_staten,
                                                          uchar karakter) const;

    const ThompsonNA& nda;
    std::vector<std::multimap<uchar, int>> transitietabel;
};

ZoekNA::ZoekNA(const ThompsonNA& nda) : nda{nda}, transitietabel(nda.geefAantalStatenbits())
{
    for (int i = 0; i < nda.geefAantalStatenbits(); i++)
    {
        const Verbinding& verbinding = nda[i];

        transitietabel[verbinding.bron].insert(std::make_pair(verbinding.karakter, verbinding.doel));
    }
}

std::vector<bool> ZoekNA::bepaal_epsilon_sluiting(const std::vector<bool>& geactiveerde_staten) const
{
    std::vector<bool> nieuwe_geactiveerde_staten(geactiveerde_staten);
    std::stack<int> te_overlopen_staten;

    for (int i = 0; i < geactiveerde_staten.size(); i++)
    {
        if (geactiveerde_staten[i])
        {
            te_overlopen_staten.push(i);
        }
    }

    while (!te_overlopen_staten.empty())
    {
        int huidige_staat = te_overlopen_staten.top();
        te_overlopen_staten.pop();

        auto overgangen = transitietabel[huidige_staat].equal_range(epsilon);

        for (auto it = overgangen.first; it != overgangen.second; it++)
        {
            nieuwe_geactiveerde_staten[it->second] = true;
            te_overlopen_staten.push(it->second);
        }
    }

    return nieuwe_geactiveerde_staten;
}

std::vector<bool> ZoekNA::bepaal_volgende_geactiveerde_staten(const std::vector<bool>& geactiveerde_staten,
                                                              uchar karakter) const
{
    std::vector<bool> nieuwe_geactiveerde_staten(nda.geefAantalStatenbits(), false);

    for (int i = 0; i < geactiveerde_staten.size(); i++)
    {
        if (geactiveerde_staten[i])
        {
            auto overgangen = transitietabel[i].equal_range(karakter);

            for (auto it = overgangen.first; it != overgangen.second; it++)
            {
                nieuwe_geactiveerde_staten[it->second] = true;
            }
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
    std::vector<bool> geactiveerde_staten(nda.geefAantalStatenbits(), false);

    for (int i = 0; i < regel.size(); i++)
    {
        geactiveerde_staten[0] = true;
        geactiveerde_staten = bepaal_epsilon_sluiting(geactiveerde_staten);

        std::vector<bool> nieuwe_geactiveerde_staten =
                bepaal_volgende_geactiveerde_staten(geactiveerde_staten, regel[i]);

        if (nieuwe_geactiveerde_staten[nda.geefAantalStatenbits() - 1])
        {
            gevonden_indexen.insert(i);
        }

        geactiveerde_staten = std::move(nieuwe_geactiveerde_staten);
    }

    return gevonden_indexen;
}

#endif
