
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

typedef unsigned int uint;
typedef unsigned char uchar;

class KnuthMorrisPratt
{
private:

    const uchar* naald;
    uint naaldlengte;
    std::vector<int> prefix;
    std::vector<int> kmp_prefix; // TODO

public:
    KnuthMorrisPratt(const uchar* naald, uint naaldlengte);
    std::vector<int> get_prefix() const;
    void zoek_eenvoudig(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte) const;
    void zoek(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte) const;
    std::string to_string() const;
};

KnuthMorrisPratt::KnuthMorrisPratt(const uchar* naald, uint naaldlengte)
: prefix(naaldlengte + 1), kmp_prefix(naaldlengte + 1), naaldlengte(naaldlengte), naald(naald)
{
    prefix[0] = -1;
    for (int i = 1; i <= naaldlengte; i++)
    {
        int vorige_prefix = prefix[i - 1];
        while (vorige_prefix >= 0 && naald[vorige_prefix] != naald[i - 1])
        {
            vorige_prefix = prefix[vorige_prefix];
        }
        prefix[i] = vorige_prefix + 1;
    }
}

std::vector<int> KnuthMorrisPratt::get_prefix() const
{
    return prefix;
}

void KnuthMorrisPratt::zoek_eenvoudig(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte) const
{
    // AAAAAXBBBBBBB...
    int vorige_prefix_lengte = 0;
    for (int i = 1; i <= hooiberglengte; i++)
    {
        while (vorige_prefix_lengte >= 0 && naald[vorige_prefix_lengte] != hooiberg[i - 1])
        {
            vorige_prefix_lengte = prefix[vorige_prefix_lengte];
        }
        vorige_prefix_lengte++;

        if (vorige_prefix_lengte == naaldlengte)
        {
            // 0  1  2  3  4  5  6  7  8  9  
            // x  x  x  a  b  c  d  x  x  x
            // 4  5  6  7  8  9  10 11 12 13
            //                      i
            // (11 - 1) - (4 - 1) = 11 - 4 = 7

            plaats.push(&hooiberg[i - naaldlengte]);
        }
    }
}

void KnuthMorrisPratt::zoek(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte) const 
{
    // TODO
    //    int naald_index = 0;
    //    for (int i = 0; i < hooiberglengte; i++)
    //    {
    //        while (naald_index >=0 && hooiberg[i] != naald[naald_index])
    //        {
    //            naald_index = naaldlengte - prefix[naald_index];
    //        }
    //    }
}

std::string KnuthMorrisPratt::to_string() const
{
    const int HEADER_WIDTH = 10;
    const int COLUMN_WIDTH = 3;

    std::stringstream ss;
    ss << std::setw(HEADER_WIDTH) << std::left << "naald:" << std::right;
    for (int i = 0; i < naaldlengte; i++)
    {
        ss << std::setw(COLUMN_WIDTH) << naald[i];
    }
    ss << std::endl;
    ss << std::setw(HEADER_WIDTH) << std::left << "prefix:" << std::right;
    for (const auto& p : prefix)
    {
        ss << std::setw(COLUMN_WIDTH) << p;
    }
    ss << std::endl;
    return ss.str();
}
