
#include "kmp.h"

#include <cassert>
#include <iostream>

int main()
{
    KnuthMorrisPratt kmp{"abacabab"};
    std::cout << kmp.to_string() << std::endl;

    kmp = KnuthMorrisPratt{"ababaca"};
    std::cout << kmp.to_string() << std::endl;

    kmp = KnuthMorrisPratt{"aba"};
    std::cout << kmp.to_string() << std::endl;
    std::queue<int> resultaten = kmp.zoek_lineair("ababa");

    assert(resultaten.front() == 0);
    resultaten.pop();
    assert(resultaten.front() == 2);
    resultaten.pop();

    std::cout << "Done..." << std::endll;

    return 0;
}
