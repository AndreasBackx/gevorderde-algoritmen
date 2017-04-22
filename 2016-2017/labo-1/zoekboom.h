
#ifndef ZOEKBOOM_H
#define ZOEKBOOM_H

#include <string>
#include <sstream>
#include <ostream>
#include <memory>
#include <map>
#include <stack>
#include <algorithm>

template <class Sleutel, class Data>
class Zoekknoop;

enum class Richting
{
    LINKS,
    RECHTS
};

inline Richting andere_richting(Richting t)
{
    if (t == Richting::LINKS)
    {
        return Richting::RECHTS;
    }
    else if (t == Richting::RECHTS)
    {
        return Richting::LINKS;
    }
    else
    {
        throw "! wordt niet ondersteund voor andere richtingen";
    }
}

template <class Sleutel, class Data>
class Zoekboom : public std::unique_ptr<Zoekknoop<Sleutel, Data> >
{
public:

    Zoekboom() : std::unique_ptr<Zoekknoop<Sleutel, Data >> (nullptr) { }

    Zoekboom(Zoekknoop<Sleutel, Data>* knoop)
    : std::unique_ptr<Zoekknoop<Sleutel, Data >> (knoop) { }

    virtual void voeg_toe(const Sleutel& sleutel, const Data& data);
    int diepte() const;
    double gemiddelde_diepte() const;
    bool is_leeg() const;

    void roteer(const Richting& richting);

    void maak_onevenwichtig();
    void maak_onevenwichtig_weer_evenwichtig();

    Zoekboom<Sleutel, Data>& vind_zoekboom_van_wortel(const Zoekknoop<Sleutel, Data>& knoop);

    virtual std::string to_string() const;
    std::string generate_dot_code() const;

protected:

    //zoekfunctie zoekt sl en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
    //ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
    //noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
    void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);

private:

    void gemiddelde_diepte(int diepte, int& som_dieptes, int& aantal_knopen) const;
    void roteer_traag_maar_leesbaar(Zoekboom<Sleutel, Data>& p, Zoekboom<Sleutel, Data>& c, Zoekboom<Sleutel, Data>& beta);

    friend std::ostream& operator<<(std::ostream& out, const Zoekboom<Sleutel, Data>& boom)
    {
        if (boom)
        {
            out << *boom;
        }
        else
        {
            out << "NULL" << std::endl;
        }
        return out;
    }
};

/******************************************************************************/

template <class Sleutel, class Data>
class Zoekknoop
{
public:

    Zoekknoop(const Sleutel& sl, const Data& data)
    : sleutel{sl}, data{data}, ouder{nullptr}, links{nullptr}, rechts{nullptr}
    {
    }

    Zoekknoop(const Sleutel& sl, const Data& data, Zoekknoop<Sleutel, Data>* ouder)
    : sleutel{sl}, data{data}, ouder{ouder}, links{nullptr}, rechts{nullptr}
    {
    }

    Zoekknoop<Sleutel, Data>* get_ouder();
    Zoekboom<Sleutel, Data>& get_links();
    Zoekboom<Sleutel, Data>& get_rechts();
    std::string to_string() const;
    Richting is_linker_of_rechter_kind() const;

protected:
    Sleutel sleutel;
    Data data;
    Zoekknoop<Sleutel, Data>* ouder;
    Zoekboom<Sleutel, Data> links, rechts;

    Zoekknoop() : ouder(0) { }

private:
    friend class Zoekboom<Sleutel, Data>;

    friend std::ostream& operator<<(std::ostream& out, const Zoekknoop<Sleutel, Data>& knoop)
    {
        out << "{ S: " << knoop.sleutel
            << ", D: " << knoop.data
            << ", P: ";
        if (knoop.ouder)
        {
            out << knoop.ouder->sleutel;
        }
        else
        {
            out << "NULL";
        }
        out << ", L: ";
        if (knoop.links)
        {
            out << knoop.links->sleutel;
        }
        else
        {
            out << "NULL";
        }
        out << ", R: ";
        if (knoop.rechts)
        {
            out << knoop.rechts->sleutel;
        }
        else
        {
            out << "NULL";
        }
        //out << ", " << &knoop;
        out << " }" << std::endl;

        out << knoop.links;
        out << knoop.rechts;

        //		out << "{ S: " << knoop.sleutel
        //				<< ", D: " << knoop.data
        //				<< ", P: " << (knoop.ouder ? std::to_string(knoop.ouder->sleutel).c_str() : "NULL")
        //				<< ", L: " << (knoop.links ? std::to_string(knoop.links->sleutel).c_str()  : "NULL")
        //				<< ", R: " << (knoop.rechts ? std::to_string(knoop.rechts->sleutel).c_str()  : "NULL")
        //				<< ", " << &knoop << " }" << std::endl
        //				<< knoop.links
        //				<< knoop.rechts;
        return out;
    }
};

/******************************************************************************/

/* http://graphs.grevian.org/example */
template <class Sleutel, class Data>
std::string Zoekboom<Sleutel, Data>::generate_dot_code() const
{
    std::stringstream ss;

    ss << "digraph {" << std::endl;

    std::stack<const Zoekboom<Sleutel, Data>*> te_bezoeken_bomen;
    te_bezoeken_bomen.push(&(*this));

    int null_id = 0; // nodig zodat er meerdere NULL knopen kunnen bestaan */
    while (!te_bezoeken_bomen.empty())
    {
        const Zoekboom<Sleutel, Data>* huidige_boom = te_bezoeken_bomen.top();
        te_bezoeken_bomen.pop();

        if ((*huidige_boom)->links)
        {
            ss << "\t" << (*huidige_boom)->sleutel << " -> " << (*huidige_boom)->links->sleutel << ";" << std::endl;
            te_bezoeken_bomen.push(&(*huidige_boom)->links);
        }
        else
        {
            ss << "\t" << (*huidige_boom)->sleutel << " -> {NULL" << null_id << "[label=\"NULL\"]};" << std::endl;
            null_id++;
        }

        if ((*huidige_boom)->rechts)
        {
            ss << "\t" << (*huidige_boom)->sleutel << " -> " << (*huidige_boom)->rechts->sleutel << ";" << std::endl;
            te_bezoeken_bomen.push(&(*huidige_boom)->rechts);
        }
        else
        {
            ss << "\t" << (*huidige_boom)->sleutel << " -> {NULL" << null_id << "[label=\"NULL\"]};" << std::endl;
            null_id++;
        }
    }

    ss << "}" << std::endl;

    return ss.str();
}

template <class Sleutel, class Data>
std::string Zoekknoop<Sleutel, Data>::to_string() const
{
    std::stringstream ss;

    ss << "(" << this->links.to_string()
        << " <- " << this->sleutel << " -> "
        << this->rechts.to_string() << ")";

    return ss.str();
}

template <class Sleutel, class Data>
Richting Zoekknoop<Sleutel, Data>::is_linker_of_rechter_kind() const
{
    if (!ouder)
    {
        throw "Er is geen ouder.";
    }

    if ((ouder->links).get() == this)
    {
        return Richting::LINKS;
    }
    else
    {
        return Richting::RECHTS;
    }
}

template <class Sleutel, class Data>
Zoekknoop<Sleutel, Data>* Zoekknoop<Sleutel, Data>::get_ouder()
{
    return ouder;
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekknoop<Sleutel, Data>::get_links()
{
    return links;
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekknoop<Sleutel, Data>::get_rechts()
{
    return rechts;
}

/******************************************************************************/

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voeg_toe(const Sleutel& sleutel, const Data& data)
{
    Zoekboom<Sleutel, Data>* plaats;
    Zoekknoop<Sleutel, Data>* ouder;

    zoek(sleutel, ouder, plaats);

    if (!*plaats)
    {
        Zoekboom<Sleutel, Data> nieuw{new Zoekknoop<Sleutel, Data>(sleutel, data)};
        nieuw->ouder = ouder;
        *plaats = std::move(nieuw);
    }
}

template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::diepte() const
{
    if (is_leeg())
    {
        // De diepte van een knoop is het aantal verbindingen tussen root en de node, NIET het aantal knopen!
        // diepte(root) = 0, dus moet bij een volledige deelboom max(-1, -1) + 1;
        return -1;
    }

    return std::max((*this)->links.diepte(), (*this)->rechts.diepte()) + 1;
}

template <class Sleutel, class Data>
double Zoekboom<Sleutel, Data>::gemiddelde_diepte() const
{
    int som_dieptes = 0;
    int aantal_knopen = 0;

    gemiddelde_diepte(0, som_dieptes, aantal_knopen);

    if (aantal_knopen)
    {
        return (static_cast<double> (som_dieptes) / static_cast<double> (aantal_knopen));
    }
    else
    {

        return 0;
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::gemiddelde_diepte(int diepte, int& som_dieptes, int& aantal_knopen) const
{
    if (is_leeg())
    {
        return;
    }

    som_dieptes += diepte;
    aantal_knopen++;

    ((*this)->links).gemiddelde_diepte(diepte + 1, som_dieptes, aantal_knopen);
    ((*this)->rechts).gemiddelde_diepte(diepte + 1, som_dieptes, aantal_knopen);
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteer(const Richting& richting)
{
    if (is_leeg())
    {
        return;
    }

    if (richting == Richting::LINKS)
    {
        if (!((*this)->rechts))
        {
            throw "Er is geen rechterkind.";
        }

        Zoekboom<Sleutel, Data> temp = std::move(*this);
        (*this) = std::move(temp->rechts);
        temp->rechts = std::move((*this)->links);
        (*this)->links = std::move(temp);

        (*this)->ouder = (*this)->links->ouder;
        (*this)->links->ouder = this->get();
        if ((*this)->links->rechts)
        {
            (*this)->links->rechts->ouder = ((*this)->links).get();
        }
    }
    else if (richting == Richting::RECHTS)
    {
        if (!((*this)->links))
        {
            throw "Er is geen linkerkind.";
        }

        Zoekboom<Sleutel, Data> temp = std::move(*this);
        (*this) = std::move(temp->links);
        temp->links = std::move((*this)->rechts);
        (*this)->rechts = std::move(temp);

        (*this)->ouder = (*this)->rechts->ouder;
        (*this)->rechts->ouder = this->get();
        if ((*this)->rechts->links)
        {
            (*this)->rechts->links->ouder = ((*this)->rechts).get();
        }
    }
    else
    {
        throw "Rotatierichting moet links of rechts zijn.";
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats)
{
    plaats = this;
    ouder = nullptr;

    while (*plaats && (*plaats)->sleutel != sleutel)
    {
        ouder = plaats->get();
        if ((*plaats)->sleutel < sleutel)
        {
            plaats = &(*plaats)->rechts;
        }
        else
        {
            plaats = &(*plaats)->links;
        }
    }
}

template <class Sleutel, class Data>
std::string Zoekboom<Sleutel, Data>::to_string() const
{
    if (!is_leeg())
    {
        return (*this)->to_string();
    }
    else
    {
        return "NULL";
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maak_onevenwichtig()
{
    if (is_leeg())
    {
        return;
    }

    std::map<Sleutel, Data> waarden;
    std::stack<Zoekboom<Sleutel, Data>*> te_bezoeken_bomen;
    te_bezoeken_bomen.push(&(*this));

    while (!te_bezoeken_bomen.empty())
    {
        Zoekboom<Sleutel, Data>* huidige_knoop = te_bezoeken_bomen.top();
        te_bezoeken_bomen.pop();

        if (*huidige_knoop)
        {
            waarden[(*huidige_knoop)->sleutel] = (*huidige_knoop)->data;
            te_bezoeken_bomen.push(&((*huidige_knoop)->links));
            te_bezoeken_bomen.push(&((*huidige_knoop)->rechts));
        }
    }

    // std::sort(waarden.begin(), waarden.end());
    // Niet nodig, aangezien een map al gesorteerd wordt (bevat een boomstructuur)

    this->reset();

    for (const std::pair<Sleutel, Data>& element : waarden)
    {
        this->voeg_toe(element.first, element.second);
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maak_onevenwichtig_weer_evenwichtig()
{
    if (is_leeg())
    {
        return;
    }

    int rotatie_index = (diepte() / 2);
    // Gebruik geen "for (int i = 0; i < (diepte() / 2); i++)", want de waarde van diepte()
    // verandert na elke rotatie

    for (int i = 0; i < rotatie_index; i++)
    {
        roteer(Richting::LINKS);
        // Links, want we voegden in maak_onevenwichtig() alles toe in stijgende volgorde
    }
}

template <class Sleutel, class Data>
bool Zoekboom<Sleutel, Data>::is_leeg() const
{
    return !(*this);
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekboom<Sleutel, Data>::vind_zoekboom_van_wortel(const Zoekknoop<Sleutel, Data>& knoop)
{
    if (&knoop == this->get())
    {
        return (*this);
    }
    else
    {
        if (!knoop.ouder)
        {
            throw "De knoop heeft geen ouder";
        }

        if (knoop.is_linker_of_rechter_kind() == Richting::LINKS)
        {
            return knoop.ouder->links;
        }
        else if(knoop.is_linker_of_rechter_kind() == Richting::RECHTS)
        {
            return knoop.ouder->rechts;
        }
        else
        {
            throw "Andere richtingen worden niet ondersteund";
        }
    }
}

#endif