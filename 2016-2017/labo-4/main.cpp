
#include "btree.h"

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char** argv)
{
    try
    {
        const int KNOOP_GROOTTE = 50;

        Schijf<BKnoop < std::string, int, KNOOP_GROOTTE>> s;
        BTree<std::string, int, KNOOP_GROOTTE> bt{s};

        std::ifstream in("donquijote.txt");

        int word_index = 1;
        
        std::string word;
        while (in >> word)
        {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            std::cout << word_index << " | \"" << word << "\" (";

            /*if (word == "mancha")
            {
                std::cout << "HIER";
            }*/

            try
            {
                int aantal = bt.zoek(word);
                aantal++;
                bt.voegtoe(word, aantal);
                std::cout << "+1";
            }
            catch (const char* e)
            {
                bt.voegtoe(word, 1);
                std::cout << "toegevoegd";
            }

            std::cout << ")" << std::endl;

            // std::cout << bt.to_string() << std::endl << std::endl;
            
            word_index++;
        }

        std::cout << bt.to_string() << std::endl;
    }
    catch (const char* ex)
    {
        std::cout << "ERROR: " << ex << std::endl;
    }

    std::cout << "Done..." << std::endl;

    return 0;
}

