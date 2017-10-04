
#include "zoekboom14.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <algorithm>

void test_groot_aantal_sleutels()
{
    Zoekboom<int, int> groot_zb;

    constexpr int AANTAL = 100'000'000;

    std::vector<int> sleutels(AANTAL); // op deze manier geen dubbels

    std::cout << "> vector vullen" << std::endl;
    for (int i = 0; i < AANTAL; i++)
    {
        sleutels.at(i) = i;
    }

    std::cout << "> vector shufflen" << std::endl;
    std::random_shuffle(std::begin(sleutels), std::end(sleutels));

    std::cout << "> sleutels toevoegen" << std::endl;
    for (size_t i = 0; i < sleutels.size(); i++)
    {
        groot_zb.voeg_toe(sleutels[i], 2 * sleutels[i]);

        if ((i % (AANTAL/100)) == 0)
        {
            printf("Added %zu\n", i); // sneller dan cout
        }
    }

//    std::cout << "Groot uitschrijven naar bestand..." << std::endl;
//
//    std::ofstream out;
//    out.open("boom_groot.dot");
//    out << groot_zb.get_dot_code();
//    out.close();
}

int main()
{
    Zoekboom<int, int> zb;

    std::cout << "Leeg:" << std::endl;
    std::cout << zb.get_dot_code() << std::endl;
    std::ofstream out;
    out.open("lege_boom.dot");
    out << zb.get_dot_code();
    out.close();

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18}; // http://lcm.csa.iisc.ernet.in/dsa/node91.html

    for (int key : keys)
    {
        zb.voeg_toe(key, 2 * key);
    }

    std::cout << "Diepte: " << zb.diepte() << std::endl;
    assert(zb.diepte() == 8);

    std::cout << "Rep OK: " << (zb.is_rep_ok() ? "OK" : "Niet OK") << std::endl;
    assert(zb.is_rep_ok());

    Zoekknoop<int, int> zk2{*zb};

    std::cout << "Gevuld:" << std::endl;
    std::cout << zb.get_dot_code() << std::endl;
    out.open("boom_gevuld.dot");
    out << zb.get_dot_code();
    out.close();

    Zoekboom<int, int> zb_copy{zb};
    std::cout << "Origineel na copy:" << std::endl;
    std::cout << zb.get_dot_code() << std::endl;
    out.open("originele_boom_na_copy.dot");
    out << zb.get_dot_code();
    out.close();
    std::cout << "Copy:" << std::endl;
    std::cout << zb_copy.get_dot_code() << std::endl;
    out.open("boom_copy.dot");
    out << zb_copy.get_dot_code();
    out.close();
    
    Zoekboom<int, int> zb_testcopy{zb};
    std::cout << "Zijn gelijk: " << zb_testcopy.is_gelijk(zb) << std::endl;
    assert(zb_testcopy.is_gelijk(zb));
    zb_testcopy.voeg_toe(111, 1337);
    std::cout << "Zijn gelijk: " << zb_testcopy.is_gelijk(zb) << std::endl;
    assert(!zb_testcopy.is_gelijk(zb));

    Zoekboom<int, int> zb_move{std::move(zb)};
    std::cout << "Origineel na move:" << std::endl;
    std::cout << zb.get_dot_code() << std::endl;
    out.open("originele_boom_na_move.dot");
    out << zb.get_dot_code();
    out.close();
    std::cout << "Move:" << std::endl;
    std::cout << zb_move.get_dot_code() << std::endl;
    out.open("boom_move.dot");
    out << zb_move.get_dot_code();
    out.close();

    Zoekboom<int, int> zb_links{zb_copy};
    zb_links.roteer(Richting::LINKS);
    std::cout << "Roteer links:" << std::endl;
    std::cout << zb_links.get_dot_code() << std::endl;
    out.open("boom_roteer_links.dot");
    out << zb_links.get_dot_code();
    out.close();

    Zoekboom<int, int> zb_rechts{zb_copy};
    zb_rechts.roteer(Richting::RECHTS);
    std::cout << "Roteer rechts:" << std::endl;
    std::cout << zb_rechts.get_dot_code() << std::endl;
    out.open("boom_roteer_rechts.dot");
    out << zb_rechts.get_dot_code();
    out.close();

    Zoekboom<int, int> zb_onevenwichtig{zb_copy};
    zb_onevenwichtig.maak_onevenwichtig();
    std::cout << "Onevenwichtig:" << std::endl;
    std::cout << zb_onevenwichtig.get_dot_code() << std::endl;
    out.open("boom_onevenwichtig.dot");
    out << zb_onevenwichtig.get_dot_code();
    out.close();

    Zoekboom<int, int> zb_evenwichtig{zb_copy};
    zb_evenwichtig.maak_evenwichtig();
    std::cout << "Evenwichtig:" << std::endl;
    std::cout << zb_evenwichtig.get_dot_code() << std::endl;
    out.open("boom_evenwichtig.dot");
    out << zb_evenwichtig.get_dot_code();
    out.close();

//    test_groot_aantal_sleutels();

    std::cout << "Done..." << std::endl;

    return 0;
}
