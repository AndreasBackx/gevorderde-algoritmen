
#ifndef KMP_H
#define KMP_H

#include <cassert>
#include <iomanip>
#include <queue>
#include <sstream>
#include <vector>

class KnuthMorrisPratt
{
public:
    KnuthMorrisPratt() = delete;
    KnuthMorrisPratt(const std::string& naald);
    virtual ~KnuthMorrisPratt() = default;

    KnuthMorrisPratt(const KnuthMorrisPratt& andere) = default;
    KnuthMorrisPratt& operator=(const KnuthMorrisPratt& andere) = default;

    KnuthMorrisPratt(KnuthMorrisPratt&& andere) = default;
    KnuthMorrisPratt& operator=(KnuthMorrisPratt&& andere) = default;

    std::queue<int> zoek(const char* hooiberg, int hooiberglengte) const;
    std::queue<int> zoek_lineair(const std::string& hooiberg) const;
    std::vector<int> geef_prefix_tabel() const;

    std::string to_string() const;

private:
    std::string naald;
    std::vector<int> prefix_tabel;
};

KnuthMorrisPratt::KnuthMorrisPratt(const std::string& naald) : naald{naald}, prefix_tabel((naald.size() + 1), -1)
{
    assert(!naald.empty());

    int prefix_lengte = -1; // == prefix_tabel[0]
    for (int i = 1; i < prefix_tabel.size(); i++)
    {
        while ((prefix_lengte >= 0) && (naald[i - 1] != naald[prefix_lengte]))
        {
            prefix_lengte = prefix_tabel[prefix_lengte];
        }

        prefix_lengte++;
        prefix_tabel[i] = prefix_lengte;
    }
}

std::queue<int> KnuthMorrisPratt::zoek_lineair(const std::string& hooiberg) const
{
    if (hooiberg.empty())
    {
        return std::queue<int>{};
    }

    std::queue<int> gevonden;

    int prefix_lengte = 0;
    for (int i = 1; i <= hooiberg.size(); i++) // Let op de <= in de for-voorwaarde! bv. "aba" zoeken in "ababa"
    {
        while ((prefix_lengte >= 0) && (hooiberg[i - 1] != naald[prefix_lengte]))
        {
            prefix_lengte = prefix_tabel[prefix_lengte];
        }

        prefix_lengte++;

        if (prefix_lengte == naald.size())
        {
            gevonden.push(i - naald.size());
        }
    }

    return gevonden;
}

std::vector<int> KnuthMorrisPratt::geef_prefix_tabel() const
{
    return prefix_tabel;
}

std::string KnuthMorrisPratt::to_string() const
{
    std::stringstream out;
    constexpr int WIDTH = 4;

    for (int i = 0; i < prefix_tabel.size(); i++)
    {
        out << std::setw(WIDTH) << i;
    }
    out << std::endl;

    for (const auto& c : naald)
    {
        out << std::setw(WIDTH) << c;
    }
    out << std::endl;

    for (const auto& prefix : prefix_tabel)
    {
        out << std::setw(WIDTH) << prefix;
    }
    out << std::endl;

    return out.str();
}
#endif
