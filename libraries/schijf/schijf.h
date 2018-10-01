
#ifndef SCHIJF_H
#define SCHIJF_H

#include <map>

using blokindex = unsigned int;

// Naamgeving gaat uit van de gebruiker: schrijf is een schrijfoperatie naar de schijf, enzovoorts
template <class T>
class Schijf : private std::map<blokindex, T>
{
public:
    const static blokindex NULL_BLOKINDEX = 0;

    Schijf();

    blokindex schrijf(const T& b); // schrijf: voegt een T toe aan de schijf. Geeft de index terug
    void verwijder(blokindex bi);
    void herschrijf(const T& b, const blokindex bi);
    void lees(T& b, const blokindex bi);

private:
    blokindex indexteller = 666;
};

template <class T>
Schijf<T>::Schijf() : std::map<blokindex, T>()
{
}

template <class T>
blokindex Schijf<T>::schrijf(const T& b)
{
    ++indexteller;
    // this-> is nodig (bij sommige compilers) omdat de parametrisatie niet afgeleid kan worden uit de argumenten.
    this->operator[](indexteller) = b;
    return indexteller;
}

template <class T>
void Schijf<T>::verwijder(blokindex bi)
{
    this->erase(bi);
}

template <class T>
void Schijf<T>::herschrijf(const T& b, const blokindex bi)
{
    this->operator[](bi) = b;
}

template <class T>
void Schijf<T>::lees(T& b, const blokindex bi)
{
    b = this->operator[](bi);
}

#endif
