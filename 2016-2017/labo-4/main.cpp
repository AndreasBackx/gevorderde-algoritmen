
#include "btree.h"

#include <iostream>
#include <cassert>

void test_bknoop()
{
    BKnoop<int, int, 8> b{true};
    
    b.voegtoe(1, 111, 0);
    
    assert(!b.is_vol());
    assert(b.is_aanwezig(1));
    
    b.voegtoe(5, 555, 0);
    b.voegtoe(2, 222, 0);
    b.voegtoe(8, 888, 0);
    b.voegtoe(7, 777, 0);
    b.voegtoe(4, 444, 0);
    b.voegtoe(3, 333, 0);
    assert(!b.is_vol());
    b.voegtoe(6, 666, 0);
    assert(b.is_vol());
    
    assert(b.is_aanwezig(1));
    assert(b[1] == 1);
    assert(b.geef_data(1) == 111);
    assert(b.is_aanwezig(2));
    assert(b[2] == 2);
    assert(b.geef_data(2) == 222);
    assert(b.is_aanwezig(3));
    assert(b[3] == 3);
    assert(b.geef_data(3) == 333);
    assert(b.is_aanwezig(4));
    assert(b[4] == 4);
    assert(b.geef_data(4) == 444);
    assert(b.is_aanwezig(5));
    assert(b[5] == 5);
    assert(b.geef_data(5) == 555);
    assert(b.is_aanwezig(6));
    assert(b[6] == 6);
    assert(b.geef_data(6) == 666);
    assert(b.is_aanwezig(7));
    assert(b[7] == 7);
    assert(b.geef_data(7) == 777);
    assert(b.is_aanwezig(8));
    assert(b[8] == 8);
    assert(b.geef_data(8) == 888);
    
    assert(b.aantal_kinderen() == 8);
    
    std::cout << b.to_string() << std::endl;
}

int main(int argc, char** argv)
{
    test_bknoop();
    
    std::cout << "Done..." << std::endl;
    
    return 0;
}

