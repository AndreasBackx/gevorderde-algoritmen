
#ifndef BOOM2D_H
#define BOOM2D_H

#include "Punt2D.h"

#include <ostream>
#include <memory>
#include <limits>
#include <cmath>
#include <stack>
#include <string>

enum class Dimensie
{
    X,
    Y
};

class Knoop2D
{
private:

public:

    Knoop2D(const Punt2D& p, const Dimensie& d);

    Punt2D punt;
    Dimensie dim;
    std::unique_ptr<Knoop2D> links, rechts;

    void print(
        std::ostream& out, 
        std::string prefix = "", 
        bool is_last = true
    ) const;

    friend std::ostream& operator<<(std::ostream& out, const Knoop2D& k)
    {
        k.print(out);
        return out;
    }

};

class Boom2D
{
private:
    std::unique_ptr<Knoop2D> wortel;

    void zoek_dichtste_buur_recursief(
        const Punt2D& zoek_punt, 
        Punt2D& gevonden_dichtste_punt,
        unsigned int& aantal_bezochte_knopen, 
        const std::unique_ptr<Knoop2D>& huidige_knoop,
        unsigned int& kleinste_afstand
    );
    
    void zoek_dichtste_buur_zonder_pruning_recursief(
        const Punt2D& zoek_punt, 
        Punt2D& gevonden_dichtste_punt,
        unsigned int& aantal_bezochte_knopen, 
        const std::unique_ptr<Knoop2D>& huidige_knoop,
        unsigned int& kleinste_afstand
    );

public:

    bool is_aanwezig(const Punt2D& zoek_punt);
    void zoek_punt(
        const Punt2D& zoek_punt, 
        std::unique_ptr<Knoop2D>*& plaats, 
        Knoop2D*& ouder
    );
    void voeg_toe(const Punt2D& voeg_toe_punt);
    void zoek_dichtste_buur(
        const Punt2D& zoek_punt, 
        Punt2D& gevonden_dichtste_punt,
        unsigned int& aantal_bezochte_knopen, 
        unsigned int& kleinste_afstand, 
        bool gebruik_pruning
    );
    bool is_leeg() const;

    friend std::ostream& operator<<(std::ostream& out, const Boom2D& b)
    {
        if (b.wortel)
        {
            b.wortel->print(out);
        }
        else
        {
            out << "De boom is leeg.";
        }
        return out;
    }
};

/******************************************************************************/

Knoop2D::Knoop2D(const Punt2D& punt, const Dimensie& dim)
: punt{punt}, dim{dim}, links{nullptr}, rechts{nullptr}
{
}

void Knoop2D::print(std::ostream& out, std::string prefix, bool is_last) const
{
    out << prefix;
    
    if (is_last)
    {
        out << "\\---";
    }
    else
    {
        out << "+---";
    }
    
    out << punt << "(";
    
    if (dim == Dimensie::X)
    {
        out << "X";
    }
    else
    {
        out << "Y";
    }
    
    out << ")" << std::endl;

    if (links)
    {
        links->print(out, prefix + (is_last ? "    " : "|   "), false);
    }
    else
    {
        out << prefix + (is_last ? "    " : "|   ") << "+---" << std::endl;
    }

    if (rechts)
    {
        rechts->print(out, prefix + (is_last ? "    " : "|   "), true);
    }
    else
    {
        out << prefix + (is_last ? "    " : "|   ") << "\\---" << std::endl;
    }
}

/******************************************************************************/

void Boom2D::zoek_punt(
    const Punt2D& zoek_punt, 
    std::unique_ptr<Knoop2D>*& plaats, 
    Knoop2D*& ouder
)
{
    ouder = nullptr;
    plaats = &wortel;

    while ((*plaats) && (*plaats)->punt != zoek_punt)
    {
        ouder = plaats->get();
        if ((*plaats)->dim == Dimensie::X)
        {
            if (zoek_punt.x < ((*plaats)->punt).x)
            {
                plaats = &((*plaats)->links);
            }
            else
            {
                plaats = &((*plaats)->rechts);
            }
        }
        else
        {
            if (zoek_punt.y < ((*plaats)->punt).y)
            {
                plaats = &((*plaats)->links);
            }
            else
            {
                plaats = &((*plaats)->rechts);
            }
        }
    }
}

bool Boom2D::is_aanwezig(const Punt2D& te_zoeken_punt)
{
    std::unique_ptr<Knoop2D>* plaats = nullptr;
    Knoop2D* ouder = nullptr;

    zoek_punt(te_zoeken_punt, plaats, ouder);

    return ((*plaats) != nullptr);
}

void Boom2D::voeg_toe(const Punt2D& voeg_toe_punt)
{
    if (is_leeg())
    {
        wortel.reset(new Knoop2D(voeg_toe_punt, Dimensie::X));
    }
    else
    {
        std::unique_ptr<Knoop2D>* plaats = nullptr;
        Knoop2D* ouder = nullptr;

        zoek_punt(voeg_toe_punt, plaats, ouder);

        if (!*plaats)
        {
            if (ouder->dim == Dimensie::X)
            {
                plaats->reset(new Knoop2D(voeg_toe_punt, Dimensie::Y));
            }
            else
            {
                plaats->reset(new Knoop2D(voeg_toe_punt, Dimensie::X));
            }
        }
        else
        {
            throw "Het punt is al aanwezig in de boom.";
        }
    }
}

void Boom2D::zoek_dichtste_buur(
    const Punt2D& zoek_punt, 
    Punt2D& gevonden_dichtste_punt,
    unsigned int& aantal_bezochte_knopen, 
    unsigned int& kleinste_afstand, 
    bool gebruik_pruning
)
{
    kleinste_afstand = std::numeric_limits<unsigned int>::max();

    if (gebruik_pruning) {
        zoek_dichtste_buur_recursief(
            zoek_punt,
            gevonden_dichtste_punt,
            aantal_bezochte_knopen,
            wortel,
            kleinste_afstand
        );
    } else {
        zoek_dichtste_buur_zonder_pruning_recursief(
            zoek_punt,
            gevonden_dichtste_punt,
            aantal_bezochte_knopen,
            wortel,
            kleinste_afstand
        );
    }
}

void Boom2D::zoek_dichtste_buur_recursief(
    const Punt2D& zoek_punt, 
    Punt2D& gevonden_dichtste_punt,
    unsigned int& aantal_bezochte_knopen, 
    const std::unique_ptr<Knoop2D>& huidige_knoop, 
    unsigned int& kleinste_afstand
)
{
    if (!huidige_knoop)
    {
        return;
    }

    aantal_bezochte_knopen++;

    unsigned int huidige_afstand = (huidige_knoop->punt).kwadafstand(zoek_punt);

    if (huidige_afstand < kleinste_afstand)
    {
        kleinste_afstand = huidige_afstand;
        gevonden_dichtste_punt = huidige_knoop->punt;
    }

    bool zit_zoek_punt_links = 
        (huidige_knoop->dim == Dimensie::X 
            && zoek_punt.x < (huidige_knoop->punt).x)
        || (huidige_knoop->dim == Dimensie::Y 
            && zoek_punt.y < (huidige_knoop->punt).y);
    // && zoek_punt.x <= (huidige_knoop->punt).x) en && zoek_punt.y <= (huidige_knoop->punt).y) mag ook
    // De gevonden punten kunnen anders zijn, maar zullen wel dezelfde (kortste) afstand hebben als bij <
    
    if (zit_zoek_punt_links)
    {
        zoek_dichtste_buur_recursief(
            zoek_punt, 
            gevonden_dichtste_punt, 
            aantal_bezochte_knopen, 
            huidige_knoop->links,
            kleinste_afstand
        );
    }
    else
    {
        zoek_dichtste_buur_recursief(
            zoek_punt, 
            gevonden_dichtste_punt, 
            aantal_bezochte_knopen, 
            huidige_knoop->rechts,
            kleinste_afstand
        );
    }

    unsigned int afstand_splitsing = 0;
    if (huidige_knoop->dim == Dimensie::X)
    {
        afstand_splitsing = ((huidige_knoop->punt).x - zoek_punt.x);
    }
    else
    {
        afstand_splitsing = ((huidige_knoop->punt).y - zoek_punt.y);
    }
    afstand_splitsing *= afstand_splitsing;

    /* sqrt( (x1 - x2)^2 + (y1 - y2)^2 ) < sqrt( (x1 - x2)^2 + (y1 - y1)^2 ) 
     * <=> sqrt( (x1 - x2)^2 + (y1 - y2)^2 ) < sqrt( (x1 - x2)^2 ) 
     * <=> (x1 - x2)^2 + (y1 - y2)^2 < (x1 - x2)^2
     * <=> kwadafstand < (x1 - x2)^2
     */

    if (afstand_splitsing < kleinste_afstand)
    {
        if (zit_zoek_punt_links)
        {
            zoek_dichtste_buur_recursief(
                zoek_punt,
                gevonden_dichtste_punt,
                aantal_bezochte_knopen,
                huidige_knoop->rechts,
                kleinste_afstand
            );
        }
        else
        {
            zoek_dichtste_buur_recursief(
                zoek_punt,
                gevonden_dichtste_punt,
                aantal_bezochte_knopen,
                huidige_knoop->links,
                kleinste_afstand
            );
        }
    }
}

void Boom2D::zoek_dichtste_buur_zonder_pruning_recursief(
    const Punt2D& zoek_punt, 
    Punt2D& gevonden_dichtste_punt,
    unsigned int& aantal_bezochte_knopen, 
    const std::unique_ptr<Knoop2D>& huidige_knoop, 
    unsigned int& kleinste_afstand
)
{
    if (!huidige_knoop)
    {
        return;
    }

    aantal_bezochte_knopen++;

    unsigned int huidige_afstand = (huidige_knoop->punt).kwadafstand(zoek_punt);

    if (huidige_afstand < kleinste_afstand)
    {
        kleinste_afstand = huidige_afstand;
        gevonden_dichtste_punt = huidige_knoop->punt;
    }

    unsigned int afstand_splitsing = 0;
    if (huidige_knoop->dim == Dimensie::X)
    {
        afstand_splitsing = ((huidige_knoop->punt).x - zoek_punt.x);
    }
    else
    {
        afstand_splitsing = ((huidige_knoop->punt).y - zoek_punt.y);
    }
    afstand_splitsing *= afstand_splitsing;

    bool zit_zoek_punt_links = 
        (huidige_knoop->dim == Dimensie::X 
            && zoek_punt.x < (huidige_knoop->punt).x) 
        || (huidige_knoop->dim == Dimensie::Y 
            && zoek_punt.y < (huidige_knoop->punt).y);  
    // && zoek_punt.x <= (huidige_knoop->punt).x) en && zoek_punt.y <= (huidige_knoop->punt).y) mag ook
    // De gevonden punten kunnen anders zijn, maar zullen wel dezelfde (kortste) afstand hebben als bij <
    
    if (zit_zoek_punt_links || afstand_splitsing < kleinste_afstand)
    {
        zoek_dichtste_buur_zonder_pruning_recursief(
            zoek_punt, 
            gevonden_dichtste_punt, 
            aantal_bezochte_knopen, 
            huidige_knoop->links,
            kleinste_afstand
        );
    }
    
    if (!zit_zoek_punt_links || afstand_splitsing < kleinste_afstand)
    {
        zoek_dichtste_buur_zonder_pruning_recursief(
            zoek_punt, 
            gevonden_dichtste_punt, 
            aantal_bezochte_knopen, 
            huidige_knoop->rechts,
            kleinste_afstand
        );
    }
}

bool Boom2D::is_leeg() const
{
    return (!wortel);
}

#endif