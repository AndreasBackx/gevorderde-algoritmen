#include "thompsonna.h"

#include <stack>

#ifndef ZOEKNA_H
#define ZOEKNA_H

// De keuze van datastructuren kan beter (nu werkt alles met vectoren)

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
};

ZoekNA::ZoekNA(const ThompsonNA& nda) : nda{nda}
{
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

        for (int i = 0; i < nda.geefAantalStatenbits(); i++)
        {
            const Verbinding& verbinding = nda[i];

            if ((verbinding.bron == huidige_staat) && (verbinding.karakter == epsilon))
            {
                nieuwe_geactiveerde_staten[verbinding.doel] = true;
                te_overlopen_staten.push(verbinding.doel);
            }
        }
    }

    return nieuwe_geactiveerde_staten;
}

std::vector<bool> ZoekNA::bepaal_volgende_geactiveerde_staten(const std::vector<bool>& geactiveerde_staten,
                                                              uchar karakter) const
{
    std::vector<bool> nieuwe_geactiveerde_staten(nda.geefAantalStatenbits(), false);

    for (int i = 0; i < nda.geefAantalStatenbits(); i++)
    {
        const Verbinding& verbinding = nda[i];

        if (geactiveerde_staten[verbinding.bron] && (verbinding.karakter == karakter))
        {
            nieuwe_geactiveerde_staten[verbinding.doel] = true;
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

        // std::cout << "Voor \"" << regel[i] << "\" : ";
        // for (const auto& s : geactiveerde_staten)
        // {
        //     std::cout << (s ? "1" : "0");
        // }
        // std::cout << "\t";

        std::vector<bool> nieuwe_geactiveerde_staten =
                bepaal_volgende_geactiveerde_staten(geactiveerde_staten, regel[i]);

        if (nieuwe_geactiveerde_staten[nda.geefAantalStatenbits() - 1])
        {
            gevonden_indexen.insert(i);
        }

        geactiveerde_staten = std::move(nieuwe_geactiveerde_staten);

        // std::cout << "Na \"" << regel[i] << "\" : ";
        // for (const auto& s : geactiveerde_staten)
        // {
        //     std::cout << (s ? "1" : "0");
        // }
        // std::cout << std::endl;
    }

    return gevonden_indexen;
}

#endif
