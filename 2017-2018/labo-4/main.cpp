
#include "csv.h"
#include "puntgenerator.h"

#include <vector>

int main()
{
    constexpr int AANTAL_DATAPUNTEN = 6000;

    CsvData grafiek("dieptedata", ",");
    Puntgenerator generator;
    std::vector<double> data;

    int aantal = 0;
    for (int i = 0; i < AANTAL_DATAPUNTEN; i++)
    {
        double huidig_punt = generator.geef_punt();

        if (aantal >= (AANTAL_DATAPUNTEN / 50))
        {
            data.push_back(huidig_punt);
            aantal = 0;
        }
        aantal++;
    }

    grafiek.voegDataToe(data);

    return 0;
}
