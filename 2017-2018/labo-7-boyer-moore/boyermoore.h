#ifndef BOYERMOORE_H
#define BOYERMOORE_H

// http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/sundayen.htm
// http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/bmen.htm

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <queue>
#include <sstream>
#include <string>

using uchar = unsigned char;
constexpr size_t GROOTTE_ALFABET = (1 << (sizeof(uchar) * 8));

class BoyerMoore
{
public:
    BoyerMoore() = delete;
    BoyerMoore(const std::string& naald);
    virtual ~BoyerMoore() = default;

    BoyerMoore(const BoyerMoore& andere) = delete;
    BoyerMoore& operator=(const BoyerMoore& andere) = delete;

    BoyerMoore(BoyerMoore&& andere) = delete;
    BoyerMoore& operator=(BoyerMoore&& andere) = delete;

    std::queue<int> zoek(const std::string& hooiberg);

    std::string to_string() const;

private:
    std::string naald;
    std::vector<int> verkeerd_karakter;
    // std::vector<std::vector<int>> uitgebreid_verkeerd_karakter;
    // verkeerd_karakter kan met unordered map met een functie dat -1 teruggeeft als het karakter geen map entry heeft
};

BoyerMoore::BoyerMoore(const std::string& naald) : naald{naald}, verkeerd_karakter(GROOTTE_ALFABET, -1)
{
    for (int i = 0; i < naald.size(); i++)
    {
        verkeerd_karakter[static_cast<uchar>(naald[i])] = i;
        // uitgebreid_verkeerd_karakter.push_back(verkeerd_karakter);
    }
}

std::queue<int> BoyerMoore::zoek(const std::string& hooiberg)
{
    std::queue<int> resultaten;

    if (hooiberg.size() < naald.size())
    {
        return resultaten;
    }

    int hooiberg_i = 0;

    while (hooiberg_i <= (hooiberg.size() - naald.size()))
    {
        int naald_i = (naald.size() - 1);
        while ((naald_i >= 0) && (naald[naald_i] == hooiberg[hooiberg_i + naald_i]))
        {
            naald_i--;
        }

        int verschuiving = 0;

        if (naald_i < 0)
        {
            resultaten.push(hooiberg_i);

            if ((hooiberg_i + naald.size()) < hooiberg.size())
            {
                uchar gewenst_karakter = hooiberg[hooiberg_i + naald.size()];
                verschuiving = (naald.size() - verkeerd_karakter[gewenst_karakter]);
            }
        }
        else
        {
            uchar gewenst_karakter = hooiberg[hooiberg_i + naald_i];
            verschuiving = (naald_i - verkeerd_karakter[gewenst_karakter]);
        }

        hooiberg_i += std::max(verschuiving, 1);
    }

    return resultaten;
}

std::string BoyerMoore::to_string() const
{
    std::stringstream out;
    constexpr int VELD_BREEDTE = 4;
    constexpr int HOOFD_BREEDTE = 40;

    out << std::setw(HOOFD_BREEDTE) << "i";
    for (int i = 0; i < naald.size(); i++)
    {
        out << std::setw(VELD_BREEDTE) << i;
    }
    out << std::endl;

    out << std::setw(HOOFD_BREEDTE) << "naald";
    for (char c : naald)
    {
        out << std::setw(VELD_BREEDTE) << c;
    }
    out << std::endl;

    // for (int i = 0; i < GROOTTE_ALFABET; i++)
    // {
    //     char c = static_cast<char>(i);
    //
    //     if (isprint(c))
    //     {
    //         out << std::setw(HOOFD_BREEDTE) << c;
    //         for (int j = 0; j < naald.size(); j++)
    //         {
    //             out << std::setw(VELD_BREEDTE) << uitgebreid_verkeerd_karakter[j][i];
    //         }
    //         out << " | " << verkeerd_karakter[i] << std::endl;
    //     }
    // }
    // out << std::endl;

    return out.str();
}

#endif
