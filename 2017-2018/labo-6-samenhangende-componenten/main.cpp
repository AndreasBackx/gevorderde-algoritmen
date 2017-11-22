

#include "graaf.h"

#include <iostream>
#include <fstream>
#include <string>

void schrijf_bestand(const std::string& bestandsnaam, const std::string& inhoud)
{
    std::ofstream out(bestandsnaam);
    out << inhoud;
    out.close();
}

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

    std::cout << "Done..." << std::endl;

    return 0;
}
