
#include "boyermoore.h"

#include <cassert>
#include <iostream>

int main()
{
    BoyerMoore bm{"ababdab"};
    std::cout << bm.to_string() << std::endl;

    std::queue<int> resultaten = BoyerMoore{"abc"}.search("abcabc");

    while (!resultaten.empty())
    {
        std::cout << resultaten.front() << ", ";
        resultaten.pop();
    }
    std::cout << std::endl;

    std::cout << "Done..." << std::endl;

    return 0;
}
