#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

#include "regexp11.h"
#include "thompsonna.h"
#include "zoekna.h"

std::vector<std::pair<int, std::string>> grep_file(const std::string& bestandsnaam, const ZoekNA& zoeker)
{
    std::vector<std::pair<int, std::string>> gevonden_regels;

    std::ifstream in(bestandsnaam);

    int regelnr = 1;
    std::string regel;
    while (getline(in, regel))
    {
        std::set<int> gevonden_indexen = zoeker.bevat_regexp(regel);
        if (!gevonden_indexen.empty())
        {
            gevonden_regels.push_back(std::make_pair(regelnr, regel));
        }

        regelnr++;
    }

    in.close();

    return gevonden_regels;
}

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
    std::cout << "NDA created by Thompson:" << std::endl;
    std::cout << nda.to_string();

    ZoekNA zoeker{nda};
    std::vector<std::pair<int, std::string>> regels = grep_file(bestandsnaam, zoeker);

    std::cout << "lines found (amount: " << regels.size() << "):" << std::endl;
    for (const auto& regel : regels)
    {
        std::cout << std::setw(5) << regel.first << ": " << regel.second << std::endl;
    }

    std::cout << "Done..." << std::endl;

    return 0;
}
