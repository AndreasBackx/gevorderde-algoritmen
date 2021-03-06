
#ifndef DOM_H
#define DOM_H

#include <vector>
#include "probprob.h"
using namespace std;

/**********************************************************************

   Class: Dom

   beschrijving: klasse die een domme implementatie geeft van Probprob
   ze probeert een getal te vinden met een gegeven aantal cijfers dat zo klein mogelijk is.

 ***************************************************************************/
class Dom : public ProbProb<char>
{
public:
    Dom(int aantal) : aantal{aantal}
    {
    };

protected:

    virtual bool isOplossing(const vector<char>& deeloplossing)
    {
        return deeloplossing.size() == aantal;
    };

    virtual double f(const vector<char>& oplossing)
    {
        double totaal = oplossing[0] - '0';
        for (int i = 1; i < oplossing.size(); i++)
        {
            totaal *= 10;
            totaal += oplossing[i] - '0';
        }
        return totaal;
    };

    // gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
    // samen met hun heuristische waarde
    virtual vector<pair<char, double> > geefVoortzetting(const vector<char>& deeloplossing)
    {
        vector<pair<char, double> > voortzetting;
        for (int i = 0; i < 10; i++)
        {
            voortzetting.emplace_back('0' + i, 10 - i);
        }
        return voortzetting;
    };

    int aantal;
};

#endif
