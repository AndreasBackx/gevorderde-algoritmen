
// Labo 6 2016-2017 heeft een gerefactorde versie voor stroomnetwerken

#ifndef STROOMNETWERK_H
#define STROOMNETWERK_H

#include <cassert>
#include <sstream>
#include <string>

#include "graaf.h"
#include "langpadzoeker.h"
#include "vergrotendpad.h"

/**********************************************************************

   Class: Stroomnetwerk

   beschrijving: Een stroomnetwerk gaat uit van een gewogen gerichte graaf
                 die in de constructor wordt opgegeven

***************************************************************************/

template <class T> // T = takdata
class Stroomnetwerk : public GraafMetTakdata<GERICHT, T>
{
public:
    // Graaf<GERICHT>(gr) moet toegevoegd, anders roept de copyconstructor van GraafMetTakdata de defaultconstructor van
    // Graaf op en krijgen we een lege graaf.
    Stroomnetwerk<T>(const GraafMetTakdata<GERICHT, T>& graaf);
    Stroomnetwerk<T>(const GraafMetTakdata<GERICHT, T>& graaf, int producer, int verbruiker);

    Stroomnetwerk<T>() = delete;
    virtual ~Stroomnetwerk<T>() = default;

    Stroomnetwerk<T>(const Stroomnetwerk<T>& ander) = default;
    Stroomnetwerk<T>& operator=(const Stroomnetwerk<T>& ander) = default;

    Stroomnetwerk<T>(Stroomnetwerk<T>&& ander) = default;
    Stroomnetwerk<T>& operator=(Stroomnetwerk<T>&& ander) = default;

    Stroomnetwerk<T>& operator+=(const Pad<T>& vergrotendpad);
    Stroomnetwerk<T>& operator-=(const Pad<T>& vergrotendpad);

    std::string genereer_dot_code() const;
    T geef_capaciteit() const;

protected:
    int producent;
    int verbruiker;
};

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& graaf)
    : Graaf<GERICHT>{graaf}, GraafMetTakdata<GERICHT, T>{graaf}
{
}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& graaf, int producent, int verbruiker)
    : Graaf<GERICHT>{graaf.aantalKnopen()}, producent{producent}, verbruiker{verbruiker}
{
    Stroomnetwerk<T> restnetwerk{graaf};

    Pad<T> vergrotendpad;
    LangPadZoeker<T> vg{restnetwerk, producent, verbruiker, vergrotendpad};
    while (vergrotendpad.size() > 0)
    {
        // += en -= hebben niets met elkaar te maken. Het een is voor het stroomnetwerk (enkel aanpassen takdata), het
        // ander voor het restnetwerk (aanpassen takdata en verbindingen). Je kan eventueel ook andere functienamen
        // gebruiken zoals updateStroomnetwerk en updateRestnetwerk
        restnetwerk -= vergrotendpad;
        *this += vergrotendpad;
        LangPadZoeker<T> vg{restnetwerk, producent, verbruiker, vergrotendpad};
    }
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator+=(const Pad<T>& vergrotendpad)
{
    // TODO als er al een verbinding in de andere richting is, die eerst reduceren alvorens in de juiste richting op te
    // tellen
    // Zie 1e voorbeeld in de main

    if (!vergrotendpad.empty())
    {
        assert(vergrotendpad.size() < std::numeric_limits<int>::max());
        for (int i = 1; i < static_cast<int>(vergrotendpad.size()); i++)
        {
            int van = vergrotendpad[i - 1];
            int naar = vergrotendpad[i];

            if (this->verbindingsnummer(van, naar) == -1)
            {
                this->voegVerbindingToe(van, naar, vergrotendpad.geef_capaciteit());
            }
            else
            {
                T* stroom = this->geefTakdata(van, naar);
                *stroom += vergrotendpad.geef_capaciteit();
            }
        }
    }

    return (*this);
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator-=(const Pad<T>& vergrotendpad)
{
    if (!vergrotendpad.empty())
    {
        assert(vergrotendpad.size() < std::numeric_limits<int>::max());
        for (int i = 1; i < static_cast<int>(vergrotendpad.size()); i++)
        {
            int van = vergrotendpad[i - 1];
            int naar = vergrotendpad[i];

            assert(this->verbindingsnummer(van, naar) != -1);

            T* stroom = this->geefTakdata(van, naar);
            *stroom -= vergrotendpad.geef_capaciteit();

            if (*stroom == 0)
            {
                this->verwijderVerbinding(van, naar);
            }

            if (this->verbindingsnummer(naar, van) == -1)
            {
                this->voegVerbindingToe(naar, van, vergrotendpad.geef_capaciteit());
            }
            else
            {
                T* stroom = this->geefTakdata(naar, van);
                *stroom += vergrotendpad.geef_capaciteit();
            }
        }
    }

    return (*this);
}

template <class T>
std::string Stroomnetwerk<T>::genereer_dot_code() const
{
    std::stringstream out;

    out << "digraph stroomnetwerk {" << std::endl;

    for (int i = 0; i < this->aantalKnopen(); i++)
    {
        const auto& knoop = (*this)[i];

        for (const auto& it : knoop)
        {
            out << "\t" << i << " -> " << it.first << " [label=\"" << *(this->geefTakdata(i, it.first)) << "\"];"
                << std::endl;
        }
    }

    out << "}" << std::endl;

    return out.str();
}

template <class T>
T Stroomnetwerk<T>::geef_capaciteit() const
{
    T som_capaciteiten = static_cast<T>(0);

    for (const auto& it : (*this)[producent])
    {
        const T* capaciteit = this->geefTakdata(producent, it.first);

        if (capaciteit)
        {
            som_capaciteiten += *capaciteit;
        }
    }

    return som_capaciteiten;
}

#endif
