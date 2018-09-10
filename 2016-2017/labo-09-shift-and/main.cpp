#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "../libraries/termcolor.hpp"
#include "shiftand.h"

// TODO
// queue omzetten naar circulaire datastructuur voor performantie
// leestekens, cijfers en newlines overzetten:
//
//      char* left = 0;
//      while(c)
//          char* right = left;
//          while (isalfa(c))
//          {
//              right++;
//          }
//          if (right - left)
//          {
//              // store in str
//          }
//          else
//          {
//              // write c
//          }
//      }
//
//           111 111111
// 0123456789012 345678
// Test, hallo!\nTest.\n
//
// 0 -> 3   lengte 4 ("Test")
// 4 -> 4   lengte 0 (",")
// 5 -> 5   lengte 0 (" ")
// 6 -> 10  lengte 5 ("hallo")
// ...

void display_results(ostream& out, std::queue<const uchar*> gevonden, const uchar* hooiberg)
// geen by reference, want willen de resultaten niet wegsmijten, enkel uitlezen
{
    if (gevonden.empty())
    {
        out << "Er zijn geen matchen gevonden" << std::endl;
    }
    else
    {
        while (!gevonden.empty())
        {
            size_t index_gevonden = (gevonden.front() - hooiberg);
            gevonden.pop();
            out << "Gevonden op plaats " << index_gevonden << std::endl;
        }
    }
}

void test_zoek()
{
    const uchar naald[] = {'a', 'b', 'a', 'b'};
    const uint naald_lengte = static_cast<uint>(sizeof(naald) / sizeof(naald[0]));

    ShiftAnd sa(naald, naald_lengte);

    // std::cout << sa.to_string() << std::endl;

    const uchar hooiberg[] = {'d', 'd', 'a', 'b', 'd', 'd', 'a', 'b', 'a', 'b', 'c',
                              'd', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'b'};
    const uint hooiberg_lengte = static_cast<uint>(sizeof(hooiberg) / sizeof(hooiberg[0]));

    std::queue<const uchar*> gevonden = sa.zoek_exact(hooiberg, hooiberg_lengte);
    assert(gevonden.size() == 3);

    // std::cout << termcolor::blue << sa.to_string_debug_zonder_fouten() << termcolor::reset << std::endl;

    std::cout << "Zonder overlap: " << std::endl;
    display_results(std::cout, gevonden, hooiberg);
    std::cout << std::endl;

    gevonden = sa.zoek_exact(hooiberg, hooiberg_lengte, true);
    assert(gevonden.size() == 5);

    // std::cout << termcolor::blue << sa.to_string_debug_zonder_fouten() << termcolor::reset << std::endl;

    std::cout << "Met overlap: " << std::endl;
    display_results(std::cout, gevonden, hooiberg);
    std::cout << std::endl;

    // Zonder overlap
    //
    //                     1 1 1 1 1 1 1 1 1 1 2 2
    // 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    // d d a b d d a b a b c d a b a b a b a b a b
    //             ------^
    //                         ------^
    //                                 ------^
    //
    // Met overlap
    //
    //                     1 1 1 1 1 1 1 1 1 1 2 2
    // 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    // d d a b d d a b a b c d a b a b a b a b a b
    //             ------^
    //                         ------^
    //                             ------^
    //                                 ------^
    //                                     ------^
}

void test_zoek_met_1_vervanging()
{
    const uchar naald[] = {'a', 'b', 'c', 'd'};
    const uint naald_lengte = static_cast<uint>(sizeof(naald) / sizeof(naald[0]));

    ShiftAnd sa(naald, naald_lengte);

    // std::cout << sa.to_string() << std::endl;

    const uchar hooiberg[] = {'d', 'd', 'a', 'b', 'd', 'd', 'a', 'b', 'a', 'd', 'c', 'd'};
    const uint hooiberg_lengte = static_cast<uint>(sizeof(hooiberg) / sizeof(hooiberg[0]));

    std::queue<const uchar*> gevonden = sa.zoek_1_vervanging(hooiberg, hooiberg_lengte);
    assert(gevonden.size() == 2);

    // std::cout << termcolor::blue << sa.to_string_debug_zonder_fouten() << termcolor::reset << std::endl;
    // std::cout << termcolor::blue << sa.to_string_debug_1_vervanging() << termcolor::reset << std::endl;

    // std::cout << "Zonder overlap: " << std::endl;
    // display_results(std::cout, gevonden, hooiberg);
    // std::cout << std::endl;

    gevonden = sa.zoek_1_vervanging(hooiberg, hooiberg_lengte, true);
    assert(gevonden.size() == 3);

    // std::cout << termcolor::blue << sa.to_string_debug_zonder_fouten() << termcolor::reset << std::endl;
    // std::cout << termcolor::blue << sa.to_string_debug_1_vervanging() << termcolor::reset << std::endl;

    // std::cout << "Met overlap: " << std::endl;
    // display_results(std::cout, gevonden, hooiberg);
    // std::cout << std::endl;

    // Zonder overlap
    //
    //                     1 1
    // 0 1 2 3 4 5 6 7 8 9 0 1
    // d d a b d d a b a d c d
    //     ------^
    //             ------^
    //
    // Met overlap
    //
    //                     1 1
    // 0 1 2 3 4 5 6 7 8 9 0 1
    // d d a b d d a b a d c d
    //     ------^
    //             ------^
    //                 ------^
}

void print_error(const char* error)
{
    std::cerr << std::endl << termcolor::red << error << termcolor::reset << std::endl;
}

std::string retrieve_replacement(std::string woord,
                                 std::map<std::string, std::queue<const uchar*>>& vorige_matches,
                                 const uchar* hooiberg,
                                 const uint hooiberg_lengte)
{
    const uint naald_lengte = static_cast<uint>(woord.size());

    if (vorige_matches.find(woord) == vorige_matches.end())
    {
        const uchar* naald = reinterpret_cast<const uchar*>(woord.c_str());

        try
        {
            ShiftAnd sa(naald, naald_lengte);

            // std::queue<const uchar*> gevonden_matches = sa.zoek_exact(hooiberg, hooiberg_lengte);
            std::queue<const uchar*> gevonden_matches = sa.zoek_1_vervanging(hooiberg, hooiberg_lengte);

            vorige_matches[woord] = gevonden_matches;
        }
        catch (const char* ex)
        {
            print_error(ex);
        }
    }

    if (vorige_matches[woord].empty())
    {
        return std::string();
    }

    std::stringstream buffer;
    const uchar* end_match = vorige_matches[woord].front();
    const uchar* match = end_match - (naald_lengte - 1);
    for (int i = 0; i < naald_lengte; i++)
    {
        buffer << match[i];
    }

    // TODO beter kijken of queue niet vervangen kan worden door circulaire datastructuur
    vorige_matches[woord].pop();
    vorige_matches[woord].push(end_match);

    return buffer.str();
}

int main()
{
    test_zoek();
    test_zoek_met_1_vervanging();

    std::map<std::string, std::queue<const uchar*>> vorige_matches;

    std::ifstream in("donquijote.txt");
    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();
    std::string donquijote = buffer.str();

    const uchar* hooiberg = reinterpret_cast<const uchar*>(donquijote.c_str());
    const uint hooiberg_lengte = static_cast<uint>(donquijote.size());

    std::ofstream out("output.txt");
    std::ofstream debug_out("output_with_debug.txt");
    if (!out.is_open())
    {
        throw "Could not write to output";
    }

    in.open("capital.txt");
    //    in.open("capital-test.txt");
    char c;
    std::string woord;
    while (in.get(c))
    {
        if (isalpha(c))
        {
            woord += c;
        }
        else
        {
            if (!woord.empty())
            {
                if (woord.size() <= 2)
                {
                    std::cout << termcolor::green << woord << termcolor::reset;
                    out << woord;
                    debug_out << woord;
                }
                else
                {
                    std::string match = retrieve_replacement(woord, vorige_matches, hooiberg, hooiberg_lengte);

                    std::cout << "(\"" << woord << "\" -> \"" << termcolor::green << match << termcolor::reset << "\")";
                    out << match;
                    debug_out << "(\"" << woord << "\" -> \"" << match << "\")";
                }

                woord.clear();
            }
            std::cout << termcolor::green << c << termcolor::reset << std::flush;
            out << c;
            debug_out << c;
        }
    }

    // DIT WERKT
    // Maar houdt geen rekening met leestekens, cijfers, whitespace, ...
    //    in.open("capital.txt");
    //    std::string woord;
    //    while (in >> woord)
    //    {
    //        if (woord.size() <= 2)
    //        {
    //            std::cout << termcolor::green << woord << termcolor::reset;
    //            out << woord;
    //            debug_out << woord;
    //        }
    //        else
    //        {
    //            std::string match = retrieve_replacement(woord, vorige_matches, hooiberg, hooiberg_lengte);
    //
    //            std::cout << "(\"" << woord << "\" -> \"" << termcolor::green << match << termcolor::reset << "\")";
    //            out << match;
    //            debug_out << "(\"" << woord << "\" -> \"" << match << "\")";
    //        }
    //        std::cout << " " << std::flush;
    //        out << " ";
    //        debug_out << " ";
    //    }
    //    in.close();
    //    debug_out.close();
    //    out.close();

    return 0;
}

// SHIFT AND

// P = ababc
// T = ddabddababd
//                         1
//    T| 0 1 2 3 4 5 6 7 8 9 0
// P   | d d a b d d a b a b d
// ----+--------------------
// 0 a | 0 0 1 0 0 0 1 0 1 0 0
// 1 b | 0 0 0 1 0 0 0 1 0 1 0
// 2 a | 0 0 0 0 0 0 0 0 1 0 0
// 3 b | 0 0 0 0 0 0 0 0 0 1 0
// 4 c | 0 0 0 0 0 0 0 0 0 0 0

//    S| 0 1 2 3 4 5 ...
// P   | a b c d e f ...
// ----+----------------
// 0 a | 1 0 0 0 0 0 ...
// 1 b | 0 1 0 0 0 0 ...
// 2 a | 1 0 0 0 0 0 ...
// 3 b | 0 1 0 0 0 0 ...
// 4 c | 0 0 1 0 0 0 ...

// SHIFT OR

// A B && | !A !B OR
// -------+---------
// 0 0 0  | 1  1  1
// 0 1 0  | 1  0  1
// 1 0 0  | 0  1  1
// 1 1 1  | 0  0  0

// P = ababc
// T = ddabddababd
//                         1
//    T| 0 1 2 3 4 5 6 7 8 9 0
// P   | d d a b d d a b a b d
// ----+--------------------
// 0 a | 1 1 0 1 1 1 0 1 0 1 1
// 1 b | 1 1 1 0 1 1 1 0 1 0 1
// 2 a | 1 1 1 1 1 1 1 1 0 1 1
// 3 b | 1 1 1 1 1 1 1 1 1 0 1
// 4 c | 1 1 1 1 1 1 1 1 1 1 1

//    S| 0 1 2 3 4 5 ...
// P   | a b c d e f ...
// ----+----------------
// 0 a | 0 1 1 1 1 1 ...
// 1 b | 1 0 1 1 1 1 ...
// 2 a | 0 1 1 1 1 1 ...
// 3 b | 1 0 1 1 1 1 ...
// 4 c | 1 1 0 1 1 1 ...
