
#include "graaf.h"
#include "stroom.h"

#include <iostream>

void test_vergrotend_pad()
{
    GraafMetTakdata<GERICHT, int> capaciteiten{};
    
    for(int i = 0; i < 8; i++)
    {
        capaciteiten.voegKnoopToe();
    }
    
    (void) capaciteiten.voegVerbindingToe(0, 1, 7);
    (void) capaciteiten.voegVerbindingToe(0, 2, 10);
    (void) capaciteiten.voegVerbindingToe(1, 4, 4);
    (void) capaciteiten.voegVerbindingToe(2, 1, 3);
    (void) capaciteiten.voegVerbindingToe(2, 3, 5);
    (void) capaciteiten.voegVerbindingToe(2, 5, 6);
    (void) capaciteiten.voegVerbindingToe(3, 0, 2);
    (void) capaciteiten.voegVerbindingToe(3, 6, 6);
    (void) capaciteiten.voegVerbindingToe(4, 5, 6);
    (void) capaciteiten.voegVerbindingToe(5, 7, 8);
    (void) capaciteiten.voegVerbindingToe(6, 5, 2);
    (void) capaciteiten.voegVerbindingToe(6, 7, 7);
    (void) capaciteiten.voegVerbindingToe(7, 4, 1);
    
    Stroomnetwerk sn{capaciteiten, 0, 7};

    int max_stroom = sn.BepaalMaximumFlow();
    
    std::cout << "Max stroom: " << max_stroom << std::endl;
}

void toepassing()
{
    
}

int main()
{
    test_vergrotend_pad();
    
    toepassing();
    
    return 0;
}