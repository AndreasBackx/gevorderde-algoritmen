#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

#include "regexp11.h"
#include "thompsonna.h"
#include "zoekna.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "usage:" << std::endl;
        std::cout << "\t mygrep <regexp> <file>" << std::endl;
        return 1;
    }

    Regexp regexp{argv[1]};
    std::string bestandsnaam{argv[2]};

    std::cout << "Searching file \"" << bestandsnaam << "\" with regexp \"" << regexp << "\"" << std::endl;

    ThompsonNA nda{regexp};
    // std::cout << "NDA created by Thompson:" << std::endl;
    // std::cout << nda.to_string();

    ZoekNA zoeker{nda};
    std::vector<std::pair<int, std::string>> regels = zoeker.grep_bestand(bestandsnaam);

    std::cout << "lines found (amount: " << regels.size() << "):" << std::endl;
    for (const auto& regel : regels)
    {
        std::cout << std::setw(5) << regel.first << ": " << regel.second << std::endl;
    }

    std::cout << "Done..." << std::endl;

    return 0;
}
