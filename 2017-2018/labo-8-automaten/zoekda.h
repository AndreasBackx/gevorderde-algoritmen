#ifndef ZOEKDA_H
#define ZOEKDA_H

#include "zoekna.h"

#include <cassert>
#include <cctype>
#include <fstream>
#include <limits>
#include <sstream>

class ZoekDA
{
public:
    ZoekDA(const ZoekNA& na);
    ZoekDA(const ZoekDA& andere) = default;
    ZoekDA& operator=(const ZoekDA& andere) = default;
    ZoekDA(ZoekDA&& andere) = default;
    ZoekDA& operator=(ZoekDA&& andere) = default;
    virtual ~ZoekDA() = default;

    std::set<int> bevat_regexp(const std::string& regel) const;
    std::vector<std::pair<int, std::string>> grep_bestand(const std::string& bestandsnaam) const;
    std::vector<std::map<uchar, int>> geef_overgangstabel() const;
    std::string to_string() const;

protected:
    std::vector<std::map<uchar, int>> overgangstabel;
    std::set<int> eindstaten;
};

ZoekDA::ZoekDA(const ZoekNA& na)
{
    std::set<int> toestand;
    toestand.insert(0);
    toestand = na.bepaal_epsilon_sluiting(toestand);

    int volgnummer = 0;
    std::map<std::set<int>, int> volgnummers;
    volgnummers[toestand] = volgnummer;
    volgnummer++;
    overgangstabel.resize(volgnummer);

    std::stack<std::set<int>> te_bezoeken_toestanden;
    te_bezoeken_toestanden.push(toestand);

    while (!te_bezoeken_toestanden.empty())
    {
        std::set<int> toestand = te_bezoeken_toestanden.top();
        te_bezoeken_toestanden.pop();

        for (int i = 0; i < std::numeric_limits<uchar>::max(); i++)
        {
            uchar karakter = static_cast<uchar>(i);
            assert(karakter != epsilon);

            std::set<int> volgende_toestand =
                    na.bepaal_epsilon_sluiting(na.bepaal_volgende_geactiveerde_staten(toestand, karakter));

            if (!volgende_toestand.empty())
            {
                if (volgnummers.find(volgende_toestand) == volgnummers.end())
                {
                    volgnummers[volgende_toestand] = volgnummer;

                    if (volgende_toestand.find(na.geef_eindstaat_index()) != volgende_toestand.end())
                    {
                        eindstaten.insert(volgnummer);
                    }

                    volgnummer++;
                    overgangstabel.resize(volgnummer);
                    te_bezoeken_toestanden.push(volgende_toestand);
                }

                overgangstabel[volgnummers[toestand]][karakter] = volgnummers[volgende_toestand];
            }
            else
            {
                overgangstabel[volgnummers[toestand]][karakter] = 0;
            }
        }
    }
}

std::vector<std::map<uchar, int>> ZoekDA::geef_overgangstabel() const
{
    return overgangstabel;
}

std::set<int> ZoekDA::bevat_regexp(const std::string& regel) const
{
    if (regel.empty())
    {
        return std::set<int>{};
    }

    std::set<int> gevonden_indexen;

    int huidige_staat = 0;

    for (int i = 0; i < regel.size(); i++)
    {
        uchar huidig_karakter = static_cast<uchar>(regel[i]);

        int vorige_staat = huidige_staat;
        assert(huidig_karakter < 128); // ASCII

        huidige_staat = overgangstabel[huidige_staat].at(huidig_karakter);

        if (eindstaten.find(huidige_staat) != eindstaten.end())
        {
            gevonden_indexen.insert(i);
        }
    }

    return gevonden_indexen;
}

std::vector<std::pair<int, std::string>> ZoekDA::grep_bestand(const std::string& bestandsnaam) const
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
            gevonden_regels.emplace_back(regelnr, regel);
        }

        regelnr++;
    }

    in.close();

    return gevonden_regels;
}

std::string ZoekDA::to_string() const
{
    std::stringstream out;

    out << "digraph zoekda {" << std::endl;
    out << "\trankdir=\"LR\";" << std::endl;

    for (int i = 0; i < overgangstabel.size(); i++)
    {
        for (const auto& overgang : overgangstabel[i])
        {
            out << "\t" << i << " -> " << overgang.second << " [label=\"";

            if (overgang.first != epsilon)
            {
                if (isprint(overgang.first))
                {
                    out << overgang.first;
                }
                else
                {
                    out << "unprintable";
                }
            }
            else
            {
                out << "epsilon";
            }

            out << "\"];" << std::endl;
        }
    }

    out << "}" << std::endl;

    return out.str();
}

#endif
