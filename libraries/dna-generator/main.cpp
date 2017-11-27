
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    assert(argc == 2);

    const int LENGTH = std::stoi(argv[1]);
    const int RESOLUTION = static_cast<int>(LENGTH / 100);
    std::vector<char> dna_characters = {'C', 'G', 'T', 'A'};

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, dna_characters.size());

    std::ofstream out("dna.txt");
    for (int i = 0; i < LENGTH; i++)
    {
        char c = dna_characters[dist(mt)];
        out << c;
        std::cout << c;

        if ((i % RESOLUTION) == 0)
        {
            std::cout << std::endl << (i / RESOLUTION) << " %" << std::endl;
        }
    }

    std::cout << std::endl << "Done..." << std::endl;

    return 0;
}
