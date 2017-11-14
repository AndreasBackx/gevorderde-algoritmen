
#include "kmp.h"

#include <cassert>
#include <iostream>

int main()
{
    KnuthMorrisPratt kmp{"abab"};
    std::cout << kmp.to_string() << std::endl;

    kmp = KnuthMorrisPratt{"abcabc"};
    std::cout << kmp.to_string() << std::endl;

    // Voorbeeld volgens paper Knuth
    //
    // j            1 2 3 4 5 6 7 8 9 10
    // pattern[j]   a b c a b c a c a b
    // next[j]     -1 0 1 1 1 2 3 4 5 1 2
    kmp = KnuthMorrisPratt{"abcabcacab"};
    std::cout << kmp.to_string() << std::endl;

    kmp = KnuthMorrisPratt{"abacabab"};
    std::cout << kmp.to_string() << std::endl;

    kmp = KnuthMorrisPratt{"ababaca"};
    std::cout << kmp.to_string() << std::endl;

    kmp = KnuthMorrisPratt{"aba"};
    std::cout << kmp.to_string() << std::endl;

    std::queue<int> resultaten = kmp.zoek_prefixmethode("ababa");
    assert(resultaten.front() == 0);
    resultaten.pop();
    assert(resultaten.front() == 2);
    resultaten.pop();

    resultaten = kmp.zoek_kmp("ababa");
    assert(resultaten.front() == 0);
    resultaten.pop();
    assert(resultaten.front() == 2);
    resultaten.pop();

    std::cout << "Done..." << std::endl;

    return 0;
}
