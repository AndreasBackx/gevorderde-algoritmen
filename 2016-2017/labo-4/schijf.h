
#ifndef SCHIJF_J
#define SCHIJF_H

#include <map>

typedef unsigned int blokindex;

template<class T>
class Schijf : public std::map<blokindex, T>
{
public:

    blokindex schrijf(const T& b)
    {
        ++indexteller;
        //this-> is nodig (bij sommige compilers) omdat de parametrisatie
        //niet afgeleid kan worden uit de argumenten.
        this->operator[](indexteller) = b;
        return indexteller;
    }

    void verwijder(blokindex bi)
    {
        this->erase(bi);
    }

    void herschrijf(const T& b, const blokindex bi)
    {
        this->operator[](bi) = b;
    }

    void lees(T& b, const blokindex bi)
    {
        b = this->operator[](bi);
    }

    Schijf() : std::map<blokindex, T>(), indexteller(666) { };

private:

    blokindex indexteller;
};


#endif
