

#include "graaf.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

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

void test_componentengraaf()
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
}

void ga_door_woordengraaf(const GraafMetKnoopEnTakdata<GERICHT, std::string, std::string>& woorden,
                          const std::unordered_map<std::string, int>& woord_nr)
{
    Graaf<GERICHT> comp_woorden;
    comp_woorden.wordt_componentengraaf_van(woorden);

    std::string woord{"aal"};
    // std::cout << "Geef het startwoord: ";
    // std::cin >> woord;
    //
    // while (woord_nr.find(woord) == woord_nr.end())
    // {
    //     std::cout << "\"" << woord << "\" is niet aanwezig in de graaf, geef een ander woord in: ";
    //     std::cin >> woord;
    // }

    int start_knoop_nr = woord_nr.at(woord);

    std::stack<int> te_bezoeken_knopen;
    te_bezoeken_knopen.push(start_knoop_nr);
    std::unordered_set<int> al_bezocht;
    std::unordered_map<int, int> voorlopers;
    bool gevonden = false;
    while (!te_bezoeken_knopen.empty() && !gevonden)
    {
        int huidige_knoop_nr = te_bezoeken_knopen.top();
        te_bezoeken_knopen.pop();

        if (al_bezocht.find(huidige_knoop_nr) == al_bezocht.end()) // O(lg(n))
        {
            al_bezocht.insert(huidige_knoop_nr); // O(lg(n))

            for (const auto& buur : woorden[huidige_knoop_nr])
            {
                int buur_nr = buur.first;

                if ((buur_nr != huidige_knoop_nr)
                    && (comp_woorden.geef_component(buur_nr) == comp_woorden.geef_component(huidige_knoop_nr)))
                {
                    if (voorlopers.find(buur_nr) == voorlopers.end()) // O(1)
                    {
                        voorlopers[buur_nr] = huidige_knoop_nr; // O(1)
                    }

                    te_bezoeken_knopen.push(buur_nr); // O(1)
                }
            }
        }
        else if (huidige_knoop_nr == start_knoop_nr) // huidige knoop is al bezocht in het begin
        {
            gevonden = true;
        }
    }

    std::cout << "Gevonden (in omgekeerde volgorde):" << std::endl;

    int huidige_knoop_nr = start_knoop_nr;
    gevonden = false;
    while (!gevonden)
    {
        int voorloper_nr = voorlopers[huidige_knoop_nr]; // O(1)
        std::cout << *(woorden.geefKnoopdata(voorloper_nr)) << " -> " << *(woorden.geefKnoopdata(huidige_knoop_nr))
                  << ": " << *(woorden.geefTakdata(voorloper_nr, huidige_knoop_nr)) << std::endl;
        huidige_knoop_nr = voorloper_nr;

        if (huidige_knoop_nr == start_knoop_nr)
        // Aparte bool omdat de eerste keer huidige_knoop_nr == start_knoop_nr, maar we verder moeten tot de volgende
        // keer (start en eind zijn hetzelfde)
        {
            gevonden = true;
        }
    }
}

int main()
{
    GraafMetKnoopEnTakdata<GERICHT, std::string, std::string> woorden;

    std::unordered_map<std::string, int> woord_nr;

    std::ifstream in("woordenlijst");
    std::string woord;
    while (in >> woord)
    {
        woord_nr[woord] = woorden.voegKnoopToe(woord);
        std::cout << woord << std::endl;
    }

    std::cout << "Alle woorden ingelezen" << std::endl;

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

    std::cout << "Alle verbindingen ingelezen" << std::endl;

    ga_door_woordengraaf(woorden, woord_nr);

    std::cout << "Done..." << std::endl;

    return 0;
}
