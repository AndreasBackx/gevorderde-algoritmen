
#include "graaf.h"
#include "stroom.h"
#include "pugixml.hpp"

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <limits>

void test_vergrotend_pad()
{
    std::cout << "Test voorbeeld vergrotend pad zoeken via breedte-eerst (Edmonds-Karp)" << std::endl;
    GraafMetTakdata<GERICHT, int> capaciteiten{};

    for (int i = 0; i < 8; i++)
    {
        capaciteiten.voegKnoopToe();
    }

    (void) capaciteiten.voegVerbindingToe(0, 1, 7);
    (void) capaciteiten.voegVerbindingToe(0, 2, 10);
    (void) capaciteiten.voegVerbindingToe(1, 4, 4);
    (void) capaciteiten.voegVerbindingToe(2, 1, 3);
    (void) capaciteiten.voegVerbindingToe(2, 3, 5);
    (void) capaciteiten.voegVerbindingToe(2, 5, 6);
    (void) capaciteiten.voegVerbindingToe(3, 0, 2);
    (void) capaciteiten.voegVerbindingToe(3, 6, 6);
    (void) capaciteiten.voegVerbindingToe(4, 5, 6);
    (void) capaciteiten.voegVerbindingToe(5, 7, 8);
    (void) capaciteiten.voegVerbindingToe(6, 5, 2);
    (void) capaciteiten.voegVerbindingToe(6, 7, 7);
    (void) capaciteiten.voegVerbindingToe(7, 4, 1);

    Stroomnetwerk sn{capaciteiten, 0, 7};

    int max_stroom = sn.BepaalMaxStroom();

    std::cout << "Max stroom: " << max_stroom << std::endl;
    
    std::cout << "Done ..." << std::endl;
}

// <?xml version="1.0" encoding="UTF-8"?>
//      <netwerk>                                                                                                                 
//          <knooppunt nummer='aanpassingsmoeilijkheden' xcoordinaat='17.7' ycoordinaat='31.884' hoogte='160.627'>
//              <route doel='aansprakelijkheidsbeding' afstand='24.2408'/>
//              <route doel='aansprakelijkheidsschade' afstand='23.5477'/>
//              <route doel='ambulanceverpleegkundige' afstand='15.6161'/>                                                                                                                                                      
//              <route doel='assistent-scheidsrechter' afstand='18.8787'/>

enum class Knooptype
{
    BINNENKOMEND,
    UITGAAND
};

void toepassing()
{
    std::cout << "Test toepassing: Island of Sodor" << std::endl;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("treingraaf.xml");
    assert(result);

    std::unordered_map<std::string, std::unordered_map<Knooptype, int>> naam_knoopnr;
    // naam_knoopnr[naam knooppunt][bool voor in (true) of uitgaande (false)] = knoopnr;

    GraafMetTakdata<GERICHT, int> capaciteiten;

    pugi::xml_node netwerk = doc.child("netwerk");

    // Voeg eerst alle knopen toe (dit is het gemakkelijkst)
    for (pugi::xml_node knooppunt = netwerk.child("knooppunt"); knooppunt; knooppunt = knooppunt.next_sibling("knooppunt"))
    {
        std::string van{knooppunt.attribute("nummer").value()};

        if (naam_knoopnr.find(van) == naam_knoopnr.end())
        {
            naam_knoopnr[van][Knooptype::BINNENKOMEND] = capaciteiten.voegKnoopToe();
            naam_knoopnr[van][Knooptype::UITGAAND] = capaciteiten.voegKnoopToe();

            capaciteiten.voegVerbindingToe(
                naam_knoopnr[van][Knooptype::BINNENKOMEND],
                naam_knoopnr[van][Knooptype::UITGAAND],
                1
                );
        }

        for (pugi::xml_node route = knooppunt.child("route"); route; route = route.next_sibling("route"))
        {
            std::string naar{route.attribute("doel").value()};

            if (naam_knoopnr.find(naar) == naam_knoopnr.end())
            {
                naam_knoopnr[naar][Knooptype::BINNENKOMEND] = capaciteiten.voegKnoopToe();
                naam_knoopnr[naar][Knooptype::UITGAAND] = capaciteiten.voegKnoopToe();

                capaciteiten.voegVerbindingToe(
                    naam_knoopnr[naar][Knooptype::BINNENKOMEND],
                    naam_knoopnr[naar][Knooptype::UITGAAND],
                    1
                    );
            }

            capaciteiten.voegVerbindingToe(
                naam_knoopnr[van][Knooptype::UITGAAND],
                naam_knoopnr[naar][Knooptype::BINNENKOMEND],
                std::numeric_limits<int>::max()
                );
        }
    }

    // std::cout << capaciteiten << std::endl;

    assert(capaciteiten.aantalKnopen() == 360); // 2 * 180 = 360
    assert(capaciteiten.aantalVerbindingen() == 3564); // 3384 + 180 = 3564

    Stroomnetwerk sn{
        capaciteiten,
        naam_knoopnr["Ffarquhar"][Knooptype::UITGAAND],
        naam_knoopnr["Tidmouth"][Knooptype::BINNENKOMEND]
    };
    
    int max_stroom = sn.BepaalMaxStroom();
    
    std::cout << "Aantal te sluiten stations: " << max_stroom << std::endl;
    
    std::cout << "Done..." << std::endl;
}

int main()
{
    test_vergrotend_pad();

    std::cout << std::endl;
    
    toepassing();

    return 0;
}
