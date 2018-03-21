#include <random>

#include "../../libraries/graaf/graaf.h"

class Stroomnetwerkgenerator
{
public:
    virtual GraafMetTakdata<GERICHT, int>* nieuw_stroomnetwerk(int aantal_knopen, double takkans, int max_gewicht)
    {
        GraafMetTakdata<GERICHT, int>* nieuwe_graaf = new GraafMetTakdata<GERICHT, int>(aantal_knopen);

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> welke_takdata(0, max_gewicht);
        std::uniform_real_distribution<double> is_er_verbinding(0, 1);

        for (int i = 0; i < aantal_knopen; i++)
        {
            for (int j = 0; j < aantal_knopen; j++)
            {
                if ((j != i) && (is_er_verbinding(mt) < takkans))
                {
                    nieuwe_graaf->voegVerbindingToe(i, j, welke_takdata(mt));
                }
            }
        }

        return nieuwe_graaf;
    }
};
