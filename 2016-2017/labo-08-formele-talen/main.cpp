
#include "regexp11.h"
#include "RegexNaarGrammatica.h"

#include <iostream>
#include <iomanip>
#include <string>

int main()
{
    std::string regexp = "(c|b)(c|b)(a|c)*(a|c)";

    try
    {
        Regexp parsed_regexp(regexp);
        std::cout << regexp << std::endl
            << parsed_regexp << std::endl;

        RegexNaarGrammatica rng;
        rng.parse(&parsed_regexp);
        std::cout << std::left << std::setw(80) << std::setfill('-') << "Grammatica " << std::endl
            << rng.to_string() << std::endl;
    }
    catch (Taalexceptie &te)
    {
        std::cerr << te.what() << std::endl;
    }

    return 0;
}