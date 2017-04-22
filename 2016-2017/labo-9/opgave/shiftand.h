#include "bitpatroon.h"
#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;
class ShiftAnd{
public:
//naaldlengte moet kleiner dan patroonlengte zijn,
//anders wordt een exceptie opgegooid.
    ShiftAnd(const uchar* naald,uint _naaldlengte);
    std::queue<const uchar*> zoek(
                     const uchar* hooiberg, uint hooiberglengte);
protected:
    Bitpatroon letterpatroon[256];
    uint naaldlengte;
};
    
