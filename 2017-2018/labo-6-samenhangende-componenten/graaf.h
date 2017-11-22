// Deze header bevat de code voor de klassen:
//
//
//    Graaf<RichtType>: basisklasse met grafen zonder tak- of knoopdata
//    GraafMetTakdata<RichtType,TakDatatype>
//    GraafMetKnoopdata<RichtType,KnoopDatatype>
//    GraafMetKnoopEnTakdata<RichtType,KnoopDatatype,Takdatatype>
//
//
// Evenals de bijbehorende klasse van GraafExcepties.
/**********************************************************************

   Class: Graaf

   beschrijving: Graaf is de basisklasse voor een hierarchie van ijlegraafklassen.
                  Deze hierarchie zal klassen bevatten voor grafen met data behorend bij
                  knopen en/of takken. Deze data worden opgeslagen in vectoren,
                  zodat kan worden opgezocht op index.
        Ad knopen:      Deze zijn opeenvolgend genummerd. Knopen kunnen niet individueel verwijderd worden.
                        Wel kunnen alle knopen samen verwijderd worden (door wis()), en kunnen knopen
                        een voor een toegevoegd worden.
        Ad verbindingen: Deze kunnen naar hartenlust toegevoegd en verwijderd worden. Als een
                        verbinding wordt verwijderd, wordt het nummer op de stack van
                        vrijgekomenVerbindingsnummers gezet. Worden verbindingen toegevoegd,
                        dan worden eerst deze vrijgekomen nummers opnieuw gebruikt, alvorens nieuwe nummers
                        worden toegevoegd. hoogsteVerbindingsnummer kan nooit verkleinen, en de
                        vector met takdata zal altijd hoogsteVerbindingsnummer elementen bevatten.


 ***************************************************************************/
/**********************************************************************

   Class: GraafMetTakdata<RichtType, Takdata>

   beschrijving: Deelklasse van vorigen, met data bij elke tak.

 ***************************************************************************/

#ifndef __GRAAF_H
#define __GRAAF_H

#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

enum RichtType
{
    GERICHT,
    ONGERICHT
};

class GraafExceptie : public std::logic_error
{
public:
    GraafExceptie(const std::string& boodschap_) : std::logic_error(boodschap_)
    {
    }
};

std::ostream& operator<<(std::ostream& os, const GraafExceptie& exc)
{
    return os << exc.what();
}

template <RichtType RT>
class Graaf
{
public:
    typedef std::map<int, int> Knoop; // beeldt knoopnummer (van buur) af op verbindingsnummer

    // Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
    Graaf(int n = 0);

    // Geeft true indien het richttype GERICHT is, false indien het ONGERICHT is.
    bool isGericht() const;

    // Voegt een nieuwe 'lege' knoop toe, d.w.z. zonder verbindingen.
    // Geeft knoopnummer van toegevoegde knoop terug (begint bij 0).
    virtual int voegKnoopToe();

    // Voegt verbinding toe tussen knoopnummers 'van' en 'naar'.
    // Gooit GraafExceptie indien verbinding al bestaat of knoopnummers ongeldig zijn.
    // Geeft verbindingsnummer van toegevoegde verbinding terug (begint bij 0).
    // Bij ongerichte graaf wordt terugverbinding ook toegevoegd (met zelfde verbindingnummer!)
    virtual int voegVerbindingToe(int van, int naar);

    // Verwijdert de verbinding tussen knoopnummers 'van' en 'naar', incl. de terugverbinding indien ongericht.
    // Gooit GraafExceptie indien knoopnummers ongeldig zijn.
    // Geen exceptie indien de verbinding niet bestond.
    // Andere verbindingen worden niet 'hernummerd'. Er komen dus mogelijks "gaten" in de verbindingsnummers.
    virtual void verwijderVerbinding(int van, int naar);

    // Geeft het aantal knopen van de graaf.
    int aantalKnopen() const;

    // Geeft het aantal verbindingen van de graaf.
    // Bij ongerichte graaf worden verbindingen NIET dubbel geteld!
    int aantalVerbindingen() const;

    // Geeft het verbindingsnummer van de verbinding tussen 'van' en 'naar'.
    // Geeft -1 indien die verbinding niet bestaat.
    // Gooit een GraafExceptie indien knoopnummers ongeldig zijn.
    // Opgelet: performantie is O(log(v)) waarbij v aantal verbindingen vanuit 'van' is.
    int verbindingsnummer(int van, int naar) const;

    // Verwijdert alle knopen en verbindingen en herstart de verbindingsnummer
    virtual void wis();

    // Toegang tot de knopen:

    const Knoop& operator[](int i) const
    {
        return knopen[i];
    }

    Knoop& operator[](int i)
    {
        return knopen[i];
    } // deze kan als lvalue gebruikt worden

    // Schrijft de gegevens van de volledige graaf naar outputstream os.
    virtual void schrijf(std::ostream& os) const;

    // Schrijft de gegevens van de knoop met knoopnummer k naar outputstream os.
    virtual void schrijfKnoop(std::ostream& os, int k) const;

    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream& os, int v) const;

    void wordt_componentengraaf_van(const Graaf& andere);

    std::string genereer_dot_code() const;

protected:
    // hulpfuncties
    void controleerKnoopnummer(int k) const; // throw indien k ongeldig
    void voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer);
    void verwijderVerbindingUitDatastructuur(int van, int naar);
    void diepte_eerst_zoeken(const Graaf<RT>& graaf,
                             int knoop_nr,
                             std::vector<bool>& is_al_bezocht,
                             std::function<void(int knoop_nr)> prefix_function,
                             std::function<void(int knoop_nr)> postfix_function);

protected:
    // datavelden
    std::vector<Knoop> knopen;
    int hoogsteVerbindingsnummer;
    RichtType richttype;
    std::stack<int> vrijgekomenVerbindingsnummers;

    std::vector<int> componentnummers; // vector[knoopnr] = componentnr
};

template <RichtType RT>
std::ostream& operator<<(std::ostream& os, const Graaf<RT>& g);

// --- implementatie ---

template <RichtType RT>
std::string Graaf<RT>::genereer_dot_code() const
{
    throw "Not supported";
}

template <>
std::string Graaf<GERICHT>::genereer_dot_code() const
{
    std::stringstream out;

    out << "digraph graaf {" << std::endl;

    for (int i = 0; i < aantalKnopen(); i++)
    {
        const auto& knoop = (*this)[i];

        for (const auto& it : knoop)
        {
            out << "\t" << i << " -> " << it.first << std::endl;
        }
    }

    out << "}" << std::endl;

    return out.str();
}

template <RichtType RT>
void Graaf<RT>::wordt_componentengraaf_van(const Graaf& andere)
{
    throw "Not supported";
}

template <>
void Graaf<GERICHT>::wordt_componentengraaf_van(const Graaf& andere)
{
    wis();
    componentnummers = std::vector<int>(andere.aantalKnopen(), -1);

    if (andere.aantalKnopen() == 0)
    {
        return;
    }

    Graaf<GERICHT> omgekeerde_graaf{andere.aantalKnopen()};
    for (int i = 0; i < andere.aantalKnopen(); i++)
    {
        for (const auto& buur : andere[i])
        {
            omgekeerde_graaf.voegVerbindingToe(buur.first, i);
        }
    }

    std::vector<bool> is_al_bezocht(omgekeerde_graaf.aantalKnopen(), false);
    std::stack<int> postorder_volgorde;

    for (int knoop_nr = 0; knoop_nr < omgekeerde_graaf.aantalKnopen(); knoop_nr++)
    // Voor als de graaf uit meerdere bomen bestaat
    {
        if (!is_al_bezocht[knoop_nr])
        {
            // bepaal_postorder_volgorde(omgekeerde_graaf, i, is_al_bezocht, postorder_volgorde);

            diepte_eerst_zoeken(omgekeerde_graaf,
                                knoop_nr,
                                is_al_bezocht,
                                [](int knoop_nr) {},
                                [&postorder_volgorde](int knoop_nr) { postorder_volgorde.push(knoop_nr); });
        }
    }

    // https://stackoverflow.com/questions/8399417/why-vectorboolreference-doesnt-return-reference-to-bool
    is_al_bezocht = std::vector<bool>(is_al_bezocht.size(), false);

    while (!postorder_volgorde.empty())
    {
        int knoop_nr = postorder_volgorde.top();
        postorder_volgorde.pop();

        if (!is_al_bezocht[knoop_nr])
        {
            int component_nr = this->voegKnoopToe();

            diepte_eerst_zoeken(
                    andere,
                    knoop_nr,
                    is_al_bezocht,
                    [this, &component_nr](int knoop_nr) { componentnummers[knoop_nr] = component_nr; },
                    [](int knoop_nr) {});
        }
    }

    for (int knoop_nr = 0; knoop_nr < andere.aantalKnopen(); knoop_nr++)
    {
        for (const auto& buur : andere[knoop_nr])
        {
            int buur_nr = buur.first;
            if ((componentnummers[knoop_nr] != componentnummers[buur_nr])
                && (verbindingsnummer(componentnummers[knoop_nr], componentnummers[buur_nr]) == -1))
            {
                voegVerbindingToe(componentnummers[knoop_nr], componentnummers[buur_nr]);
            }
        }
    }

    for (int comp : componentnummers)
    {
        std::cout << comp << "   ";
    }
}

template <RichtType RT>
void Graaf<RT>::diepte_eerst_zoeken(const Graaf<RT>& graaf,
                                    int knoop_nr,
                                    std::vector<bool>& is_al_bezocht,
                                    std::function<void(int knoop_nr)> prefix_function,
                                    std::function<void(int knoop_nr)> postfix_function)
{
    if (is_al_bezocht[knoop_nr])
    {
        return;
    }

    prefix_function(knoop_nr);

    is_al_bezocht[knoop_nr] = true;
    for (const auto& buur : graaf[knoop_nr])
    {
        diepte_eerst_zoeken(graaf, buur.first, is_al_bezocht, prefix_function, postfix_function);
    }

    postfix_function(knoop_nr);
}

template <RichtType RT>
void Graaf<RT>::controleerKnoopnummer(int k) const
{
    if (k < 0 || (size_t) k >= knopen.size())
        throw GraafExceptie("ongeldig knoopnummer");
}

template <RichtType RT>
Graaf<RT>::Graaf(int n) : knopen(n), hoogsteVerbindingsnummer(0)
{
}

template <RichtType RT>
bool Graaf<RT>::isGericht() const
{
    return true;
} // voor gerichte graaf

template <>
bool Graaf<ONGERICHT>::isGericht() const
{
    return false;
} // voor ongerichte graaf

template <RichtType RT>
int Graaf<RT>::voegKnoopToe()
{
    int n = knopen.size();
    knopen.resize(n + 1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
    return n;
}

template <RichtType RT>
int Graaf<RT>::voegVerbindingToe(int van, int naar)
{
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);

    if (knopen[van].count(naar) > 0)
    {
        std::ostringstream out;
        out << "verbinding " << van << "-" << naar << " bestaat al";
        throw GraafExceptie(out.str());
    }
    else
    {
        int verbindingsnummer;
        if (!vrijgekomenVerbindingsnummers.empty())
        {
            verbindingsnummer = vrijgekomenVerbindingsnummers.top();
            vrijgekomenVerbindingsnummers.pop();
        }
        else
            verbindingsnummer = hoogsteVerbindingsnummer++;
        voegVerbindingToeInDatastructuur(van, naar, verbindingsnummer);
        return verbindingsnummer;
    }
}

template <RichtType RT> // voor gerichte graaf
void Graaf<RT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer)
{
    knopen[van][naar] = verbindingsnummer;
}

template <>
void Graaf<ONGERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer)
{
    knopen[van][naar] = verbindingsnummer;
    knopen[naar][van] = verbindingsnummer;
}

template <RichtType RT>
void Graaf<RT>::verwijderVerbinding(int van, int naar)
{
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    if (knopen[van].find(naar) != knopen[van].end())
    { // verbinding bestaat
        vrijgekomenVerbindingsnummers.push(knopen[van][naar]);
        verwijderVerbindingUitDatastructuur(van, naar);
    }
    // (geen exception indien verbinding niet bestaat)
}

template <RichtType RT>
void Graaf<RT>::verwijderVerbindingUitDatastructuur(int van, int naar)
{
    knopen[van].erase(naar);
}

template <>
void Graaf<ONGERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar)
{
    knopen[van].erase(naar);
    knopen[naar].erase(van);
}

template <RichtType RT>
int Graaf<RT>::aantalKnopen() const
{
    return knopen.size();
}

template <RichtType RT>
int Graaf<RT>::aantalVerbindingen() const
{
    return hoogsteVerbindingsnummer - vrijgekomenVerbindingsnummers.size();
}

template <RichtType RT>
int Graaf<RT>::verbindingsnummer(int van, int naar) const
{
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    Knoop::const_iterator verbindingit = knopen[van].find(naar);

    if (verbindingit == knopen[van].end())
        return -1;
    else
        return (*verbindingit).second;
}

template <RichtType RT>
void Graaf<RT>::wis()
{
    knopen.clear();
    hoogsteVerbindingsnummer = 0;
    while (!vrijgekomenVerbindingsnummers.empty())
        vrijgekomenVerbindingsnummers.pop();
}

template <RichtType RT>
void Graaf<RT>::schrijf(std::ostream& os) const
{
    os << "Graaf: " << aantalKnopen() << " knopen en " << aantalVerbindingen() << " verbindingen:" << std::endl;
    for (int k = 0; k < aantalKnopen(); k++)
        schrijfKnoop(os, k);
}

template <RichtType RT>
void Graaf<RT>::schrijfKnoop(std::ostream& os, int k) const
{
    os << "knoop " << k << ":" << std::endl;
    for (Knoop::const_iterator it = knopen[k].begin(); it != knopen[k].end(); ++it)
    {
        os << "  ->" << it->first;
        schrijfVerbinding(os, it->second);
    }
}

template <RichtType RT>
void Graaf<RT>::schrijfVerbinding(std::ostream& os, int v) const
{
    os << " via " << v << std::endl;
}

template <RichtType RT>
std::ostream& operator<<(std::ostream& os, const Graaf<RT>& g)
{
    g.schrijf(os);
    return os;
}

template <RichtType RT, class Takdata>
class GraafMetTakdata : public virtual Graaf<RT>
{
public:
    // Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.

    GraafMetTakdata(int n = 0) : Graaf<RT>(n){};
    // Noot: toevoegfunctie zonder takdata op te geven kan alleen gebruikt als de klasse
    //      Takdata een defaultconstructor heeft.
    virtual int voegVerbindingToe(int van, int naar);
    virtual int voegVerbindingToe(int van, int naar, const Takdata&);
    // Noot: verwijderVerbinding wordt ongewijzigd overgenomen van Graaf!

    // TakData vrijgeven (eventueel voor wijziging). Geeft nullpointer als tak niet bestaat
    // Noot: pointers teruggegeven door geefTakdata worden ongeldig
    // door toevoegen van een tak.
    const Takdata* geefTakdata(int van, int naar) const;
    Takdata* geefTakdata(int van, int naar);

    virtual void wis();

    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream& os, int v) const;

protected:
    std::vector<Takdata> takdatavector;
};

template <RichtType RT, class Takdata>
int GraafMetTakdata<RT, Takdata>::voegVerbindingToe(int van, int naar)
{
    return this->voegVerbindingToe(van, naar, Takdata());
}

template <RichtType RT, class Takdata>
int GraafMetTakdata<RT, Takdata>::voegVerbindingToe(int van, int naar, const Takdata& td)
{
    bool isnieuwtaknummer = this->vrijgekomenVerbindingsnummers.empty();
    int taknummer = Graaf<RT>::voegVerbindingToe(van, naar);
    if (isnieuwtaknummer)
    {
        takdatavector.push_back(td);
    }
    else
    {
        takdatavector[taknummer] = td;
    }

    return taknummer;
}

template <RichtType RT, class Takdata>
const Takdata* GraafMetTakdata<RT, Takdata>::geefTakdata(int van, int naar) const
{
    int taknummer = this->verbindingsnummer(van, naar);
    if (taknummer != -1)
        return &takdatavector[taknummer];
    else
        return 0;
}

template <RichtType RT, class Takdata>
Takdata* GraafMetTakdata<RT, Takdata>::geefTakdata(int van, int naar)
{
    int taknummer = this->verbindingsnummer(van, naar);
    if (taknummer != -1)
        return &takdatavector[taknummer];
    else
        return 0;
}

template <RichtType RT, class Takdata>
void GraafMetTakdata<RT, Takdata>::wis()
{
    Graaf<RT>::wis();
    takdatavector.clear();
}

template <RichtType RT, class Takdata>
void GraafMetTakdata<RT, Takdata>::schrijfVerbinding(std::ostream& os, int v) const
{
    os << " via " << v << "(Data: " << takdatavector[v] << ")" << std::endl;
}

template <RichtType RT, class Knoopdata>
class GraafMetKnoopdata : public virtual Graaf<RT>
{
public:
    // Construeert een graaf met gegeven RichtType, en lijst van Knoopdata;
    template <class InputIterator>
    GraafMetKnoopdata(InputIterator start, InputIterator eind);

    GraafMetKnoopdata() : Graaf<RT>(){};

    virtual int voegKnoopToe();
    virtual int voegKnoopToe(const Knoopdata&);

    const Knoopdata* geefKnoopdata(int knoopnr) const;
    Knoopdata* geefKnoopdata(int knoopnr);
    virtual void wis();
    virtual void schrijfKnoop(std::ostream& os, int k) const;

protected:
    // datavelden
    std::vector<Knoopdata> knoopdatavector;
};

template <RichtType RT, class Knoopdata>
template <class InputIterator>
GraafMetKnoopdata<RT, Knoopdata>::GraafMetKnoopdata(InputIterator start, InputIterator eind) : Graaf<RT>(0)
{
    for (; start != eind; start++)
        voegKnoopToe(*start);
}

template <RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT, Knoopdata>::voegKnoopToe()
{
    return this->voegKnoopToe(Knoopdata());
}

template <RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT, Knoopdata>::voegKnoopToe(const Knoopdata& kd)
{
    int ret = Graaf<RT>::voegKnoopToe();
    knoopdatavector.push_back(kd);
    return ret;
}

template <RichtType RT, class Knoopdata>
const Knoopdata* GraafMetKnoopdata<RT, Knoopdata>::geefKnoopdata(int knoopnummer) const
{
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}

template <RichtType RT, class Knoopdata>
Knoopdata* GraafMetKnoopdata<RT, Knoopdata>::geefKnoopdata(int knoopnummer)
{
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}

template <RichtType RT, class Knoopdata>
void GraafMetKnoopdata<RT, Knoopdata>::wis()
{
    Graaf<RT>::wis();
    knoopdatavector.clear();
}

template <RichtType RT, class Knoopdata>
void GraafMetKnoopdata<RT, Knoopdata>::schrijfKnoop(std::ostream& os, int k) const
{
    os << "knoop " << k << "(Data: " << knoopdatavector[k] << "):" << std::endl;
    for (std::map<int, int>::const_iterator it = this->knopen[k].begin(); it != this->knopen[k].end(); ++it)
    {
        os << "  ->" << it->first;
        this->schrijfVerbinding(os, it->second);
    }
}

template <RichtType RT, class Knoopdata, class Takdata>
class GraafMetKnoopEnTakdata : public GraafMetKnoopdata<RT, Knoopdata>, public GraafMetTakdata<RT, Takdata>
{
public:
    template <class InputIterator>
    GraafMetKnoopEnTakdata(InputIterator start, InputIterator eind) : GraafMetKnoopdata<RT, Knoopdata>(start, eind){};

    GraafMetKnoopEnTakdata() : GraafMetKnoopdata<RT, Knoopdata>(){};

    virtual void wis()
    {
        GraafMetKnoopdata<RT, Knoopdata>::wis();
        this->takdatavector.clear();
    }
};

#endif // __GRAAF_H
