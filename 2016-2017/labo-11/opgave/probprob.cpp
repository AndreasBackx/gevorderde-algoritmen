#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
#include "probprob.h"
using namespace std;

/**********************************************************************

   Class: Dom
   
   beschrijving: klasse die een domme implementatie geeft van Probprob
   ze probeert een getal te vinden met een gegeven aantal cijfers dat zo klein mogelijk is.
   
***************************************************************************/
class Dom:public ProbProb<char>{
public:
    Dom(int aantal):aantal{aantal}{};
protected:
    virtual bool isOplossing(const vector<char>& deeloplossing){
        return deeloplossing.size()==aantal;
    };
    virtual double f(const vector<char>& oplossing){
        int totaal=oplossing[0]-'0';
        for (int i=1; i<oplossing.size(); i++ ){
            totaal*=10;
            totaal+=oplossing[i]-'0';
        }
        return totaal;
    };
//gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
//samen met hun heuristische waarde
    virtual vector<pair<char,double> > geefVoortzetting(const vector<char>& deeloplossing){
        vector<pair<char,double> > voortzetting;
        for (int i=0; i<9; i++ ){
            voortzetting.emplace_back('0'+i,10-i);
        }
        return voortzetting;
    };
    int aantal;
};

int main(int argc, char *argv[]){
    srand(time(0));
    Dom dom(19);
    vector<char> optimum=dom.geefOptimum(2000);
    cout<<"Eindoplossing:\n";
    for (auto&& c : optimum ){
        cout<<" "<<c;
    }
    cout<<"\n";
    return 0;
}
