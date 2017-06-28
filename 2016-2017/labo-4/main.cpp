
#include "btree.h"

#include <iostream>
#include <cassert>

int main(int argc, char** argv)
{
    const int KNOOP_GROOTTE = 8;

    Schijf<BKnoop<int, int, KNOOP_GROOTTE>> s;
    BTree<int, int, KNOOP_GROOTTE> bt{s};

    std::cout << "Done..." << std::endl;

    return 0;
}

