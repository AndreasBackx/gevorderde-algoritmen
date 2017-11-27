#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H

#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;

class BoyerMoore{
public:
    BoyerMoore(const string&  _naald);
//de zoekfunctie geeft in teller het aantal karaktervergelijkingen terug
    std::queue<int> zoek(const string&  hooiberg,long int& teller);
};
#endif
    
