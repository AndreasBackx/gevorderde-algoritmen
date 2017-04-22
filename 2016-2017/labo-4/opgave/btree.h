#ifndef __BTREE
#define __BTREE
#include "schijf.h"
#include <iostream>
using std::endl;
using std::cerr;
using std::ostream;
//Dit bestand bevat code en hoofdingen van
template<class T, class D,unsigned int m>
class Btree;
template<class T,class D,unsigned int m>
class Bknoop;
//betekenis m: zie cursus

//crashtest voorkomt dat er meer dan drie knopen in het 'geheugen' zitten (excusief de wortel).
//dit veronderstelt wel dat er nooit knopen op de Schijf gedeletete worden,
template<class T,class D,unsigned int m>
class Bknoop{
friend class Btree<T,D,m>;
public:
    Bknoop<T,D,m>(){4/(-4+ crashtest++)};
    ~Bknoop<T,D,m>(){--crashtest};

    const Bknoop<T,D,m>& operator=(const Bknoop<T,D,m>& b){
        k=b.k;
        isblad=b.isblad;
        for (unsigned int i=0;i<k;i++){
            sleutel[i]=b.sleutel[i];
            data[i]=b.data[i];
        }
        if (!isblad){
            for (unsigned int i=0;i<=k;i++){
                index[i]=b.index[i];
            }
        }
        return *this;
    }
    //opmerking: sleutel[0] en data[0] worden niet gebruikt, zodat indexen vanaf 1 lopen
    T sleutel[m];
    D data[m];
    blokindex index[m];
    unsigned int k;
    bool isblad;
private:
    static int crashtest;
};
template<class T,class D,unsigned int m>
int Bknoop<T,D,m>::crashtest=0;


template<class T, class D,unsigned int m>
class Btree{
    typedef Bknoop<T,D,m> Knoop;
public:
    Btree(Schijf<Knoop>& s):schijf(s){
        wortel.k=0;
        wortel.isblad=true;
        wortelindex=schijf.schrijf(wortel);
    }
    

private:
   Schijf<Knoop>& schijf;
   Knoop wortel;
   blokindex wortelindex;
};



#endif
