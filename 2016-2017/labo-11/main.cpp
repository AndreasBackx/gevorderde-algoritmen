
#include <iostream>
#include <map>
#include <string>
#include <ctime>

#include "dom.h"
#include "pugixml.hpp"
#include "simulanneal11.h"
#include "affiches_sa.h"

int main(int argc, char *argv[])
{
    srand(time(0));

    //    Dom dom(19);
    //    std::vector<char> optimum = dom.geefOptimum(2000);
    //    cout << "Eindoplossing:\n";
    //    for (auto&& c : optimum)
    //    {
    //        cout << " " << c;
    //    }
    //    cout << "\n";

    pugi::xml_document doc;

    if (!doc.load_file("treingraaf.xml"))
    {
        throw "Could not load XML file!";
    }

    std::map<std::string, std::map<std::string, double>> kaart;
    for (pugi::xml_node knooppunt : doc.child("netwerk").children("knooppunt"))
    {
        std::string stad(knooppunt.attribute("nummer").value());

        for (pugi::xml_node route : knooppunt.children("route"))
        {
            std::string buur(route.attribute("doel").value());
            double afstand = route.attribute("afstand").as_double();

            kaart[stad][buur] = afstand;

            //std::cout << "Route van " << stad << " naar " << buur << " met afstand " << afstand << " ingelezen" << std::endl;
        }
    }

    Affiches_SA asa{2000, 0.5, kaart};
    std::vector<std::string> solution = asa.oplossing();

    for (const std::string& city : solution)
    {
        std::cout << city << " -> ";
    }
    std::cout << std::endl
        << "Totale cost: " << asa.f(solution) << std::endl;

    return 0;
}