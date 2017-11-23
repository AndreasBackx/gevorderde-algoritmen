

#include "graaf.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

void schrijf_bestand(const std::string& bestandsnaam, const std::string& inhoud)
{
    std::ofstream out(bestandsnaam);
    out << inhoud;
    out.close();
}

// digraph gr {
//         0 -> 1
//         1 -> 0
//         2 -> 1
//         2 -> 3
//         3 -> 4
//         4 -> 2
//         4 -> 5
//         5 -> 8
//         6 -> 5
//         7 -> 6
//         8 -> 7
//         9 -> 8
// }
//
// componentnummers = { 2, 2, 3, 3, 3, 0, 0, 0, 0, 1 }
//
// digraph comp_gr {
//         1 -> 0
//         3 -> 0
//         3 -> 2
// }

int main()
{
    Graaf<GERICHT> gr{10};
    gr.voegVerbindingToe(0, 1);
    gr.voegVerbindingToe(1, 0);
    gr.voegVerbindingToe(2, 1);
    gr.voegVerbindingToe(2, 3);
    gr.voegVerbindingToe(3, 4);
    gr.voegVerbindingToe(4, 2);
    gr.voegVerbindingToe(4, 5);
    gr.voegVerbindingToe(5, 8);
    gr.voegVerbindingToe(6, 5);
    gr.voegVerbindingToe(7, 6);
    gr.voegVerbindingToe(8, 7);
    gr.voegVerbindingToe(9, 8);
    std::cout << gr.genereer_dot_code() << std::endl;
    schrijf_bestand("voorbeeld_graaf.dot", gr.genereer_dot_code());

    Graaf<GERICHT> comp_gr;
    comp_gr.wordt_componentengraaf_van(gr);
    std::cout << comp_gr.genereer_dot_code() << std::endl;
    schrijf_bestand("voorbeeld_graaf_componentengraaf.dot", comp_gr.genereer_dot_code());

    GraafMetKnoopEnTakdata<GERICHT, std::string, std::string> woorden;

    std::unordered_map<std::string, int> woord_nr;

    std::ifstream in("woordenlijst");
    std::string woord;
    while (in >> woord)
    {
        woord_nr[woord] = woorden.voegKnoopToe(woord);
        std::cout << woord << std::endl;
    }

    in.close();
    in.clear();
    in.open("takkenlijst");

    std::string van;
    std::string naar;
    while (in >> van >> naar >> woord)
    {
        int van_nr = woord_nr.at(van);
        int naar_nr = woord_nr.at(naar);
        std::cout << van << " [" << van_nr << "] -> " << naar << " [" << naar_nr << "] (" << woord << ")";

        if (woorden.verbindingsnummer(van_nr, naar_nr) == -1)
        {
            woorden.voegVerbindingToe(van_nr, naar_nr, woord);
            std::cout << " toegevoegd";
        }
        else
        {
            std::cout << " genegeerd";
        }
        std::cout << std::endl;
    }

    Graaf<GERICHT> comp_woorden;
    comp_woorden.wordt_componentengraaf_van(woorden);
    // std::cout << comp_woorden.genereer_dot_code() << std::endl;
    schrijf_bestand("componentengraaf_woorden.dot", comp_woorden.genereer_dot_code());
    std::cout << "Aantal componenten: " << comp_woorden.aantalKnopen() << std::endl;

    std::cout << "Done..." << std::endl;

    return 0;
}
