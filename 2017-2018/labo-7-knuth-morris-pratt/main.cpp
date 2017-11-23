
#include "kmp.h"

#include <cassert>
#include <iostream>

// TODO fout in KMP tabel: nana

int main()
{
    KnuthMorrisPratt kmp{"babac"};
    std::cout << kmp.to_string() << std::endl;

    std::string woord;
    std::cout << "> ";
    while (std::cin >> woord)
    {
        kmp = KnuthMorrisPratt{woord};
        std::cout << kmp.to_string() << std::endl << "> ";
    }

    std::cout << "Done..." << std::endl;

    return 0;
}
