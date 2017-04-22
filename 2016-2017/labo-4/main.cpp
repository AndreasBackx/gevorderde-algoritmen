
#include "btree.h"

#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>

void test_bknoop_voegtoe()
{
    Bknoop<int, char, 5> k{};

    k.voeg_toe(1, 'A', 111);
    k.voeg_toe(3, 'B', 222);
    k.voeg_toe(5, 'C', 333);
    k.voeg_toe(2, 'D', 444);
    k.voeg_toe(0, 'E', 555);

    //    std::cout << k << std::endl;

    assert(k.sleutel[0] == 0);
    assert(k.sleutel[1] == 1);
    assert(k.sleutel[2] == 2);
    assert(k.sleutel[3] == 3);
    assert(k.sleutel[4] == 5);

    assert(k.data[0] == 'E');
    assert(k.data[1] == 'A');
    assert(k.data[2] == 'D');
    assert(k.data[3] == 'B');
    assert(k.data[4] == 'C');

    assert(k.index[0] == 0);
    assert(k.index[1] == 555);
    assert(k.index[2] == 111);
    assert(k.index[3] == 444);
    assert(k.index[4] == 222);
    assert(k.index[5] == 333);
}

int main()
{
    const int ORDER = 4;

    test_bknoop_voegtoe();

    //    Schijf<Bknoop<std::string, unsigned int, 5>> s{};
    //    Btree<std::string, unsigned int, 5> bt{s};

    Schijf<Bknoop<int, char, ORDER >> s{};
    Btree<int, char, ORDER> bt{s};
    bt.voeg_toe(10, 'A');
    std::cout << bt << std::endl;
    bt.voeg_toe(20, 'B');
    std::cout << bt << std::endl;
    bt.voeg_toe(30, 'C');
    std::cout << bt << std::endl;
    bt.voeg_toe(40, 'D');
    std::cout << bt << std::endl;
//    bt.voeg_toe(50, 'E');
//    std::cout << bt << std::endl;
//    bt.voeg_toe(60, 'F');
//    std::cout << bt << std::endl;

    //    std::string word;
    //    int word_counter = 0;
    //    std::ifstream file("donquijote.txt");
    //    
    //    while (file >> word)
    //    {
    //        std::cout << word << std::endl;
    //        word_counter++;
    //    }
    //    std::cout << "Read " << word_counter << " words from file." << std::endl;

    std::cout << std::endl << "--- Done... ---" << std::endl;
    return 0;
}