#include "regexp11.h"
#include "thompsonna.h"
#include "zoekda.h"
#include "zoekna.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

void print_gevonden_regels(const std::vector<std::pair<int, std::string>>& regels)
{
    std::cout << "lines found (amount: " << regels.size() << "):" << std::endl;
    for (const auto& regel : regels)
    {
        std::cout << std::setw(5) << regel.first << ": " << regel.second << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::cout << __DATE__ << " " << __TIME__ << std::endl;

    if (argc != 3)
    {
        std::cout << "usage:" << std::endl;
        std::cout << "\t mygrep <regexp> <file>" << std::endl;
        return 1;
    }

    Regexp regexp{argv[1]};
    std::string bestandsnaam{argv[2]};

    std::cout << "Searching file \"" << bestandsnaam << "\" with regexp \"" << regexp << "\"" << std::endl;

    ThompsonNA na_thompson{regexp};
    // std::cout << na_thompson.to_string() << std::endl;

    ZoekNA na_zoeker{na_thompson};
    // std::cout << na_zoeker.to_string() << std::endl;
    print_gevonden_regels(na_zoeker.grep_bestand(bestandsnaam));

    ZoekDA da_zoeker{na_zoeker};
    // std::cout << da_zoeker.to_string() << std::endl;
    print_gevonden_regels(da_zoeker.grep_bestand(bestandsnaam));

    std::cout << "Done..." << std::endl;

    return 0;
}
