
#include "../../libraries/binaire-boom/boom.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void test_evenwichtig()
{
    BinaireBoom<int, int> bb;

    constexpr int AANTAL = 16384;

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
        bb.voeg_toe(sleutels[i], 2 * sleutels[i]);
    }

    auto begin = std::chrono::steady_clock::now();

    bb.maak_evenwichtig();

    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
}

void test_iterator()
{
    BinaireBoom<int, int> bb;

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18}; // http://lcm.csa.iisc.ernet.in/dsa/node91.html

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    for (const auto& knoop : bb)
    {
        std::cout << knoop.geef_sleutel() << ", ";
    }

    std::cout << std::endl;

    BinaireBoom<int, int> bb_copy{bb};
    bb.roteer(Richting::LINKS);
    bb.roteer(Richting::LINKS);
    bb.roteer(Richting::LINKS);
    bb.roteer(Richting::LINKS);
    bb.roteer(Richting::LINKS);
    bb.roteer(Richting::LINKS);

    for (const auto& knoop : bb_copy)
    {
        std::cout << knoop.geef_sleutel() << ", ";
    }

    std::cout << std::endl;
    std::cout << bb_copy.is_enkel_content_gelijk(bb) << std::endl;
}

void test_groot_aantal_sleutels()
{
    BinaireBoom<int, int> groot_bb;

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
        groot_bb.voeg_toe(sleutels[i], 2 * sleutels[i]);

        if ((i % (AANTAL / 100)) == 0)
        {
            printf("Added %zu\n", i); // sneller dan cout
        }
    }

    //    std::cout << "Groot uitschrijven naar bestand..." << std::endl;
    //
    //    std::ofstream out;
    //    out.open("boom_groot.dot");
    //    out << groot_bb.get_dot_code();
    //    out.close();
}

int main()
{
    BinaireBoom<int, int> bb;

    std::cout << "Leeg:" << std::endl;
    std::cout << bb.get_dot_code() << std::endl;
    std::ofstream out;
    out.open("lege_boom.dot");
    out << bb.get_dot_code();
    out.close();

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18}; // http://lcm.csa.iisc.ernet.in/dsa/node91.html

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    std::cout << "Diepte: " << bb.diepte() << std::endl;
    assert(bb.diepte() == 8);

    std::cout << "Rep OK: " << (bb.is_rep_ok() ? "OK" : "Niet OK") << std::endl;
    assert(bb.is_rep_ok());

    Knoop<int, int> zk2{*bb};

    std::cout << "Gevuld:" << std::endl;
    std::cout << bb.get_dot_code() << std::endl;
    out.open("boom_gevuld.dot");
    out << bb.get_dot_code();
    out.close();

    BinaireBoom<int, int> bb_copy{bb};
    std::cout << "Origineel na copy:" << std::endl;
    std::cout << bb.get_dot_code() << std::endl;
    out.open("originele_boom_na_copy.dot");
    out << bb.get_dot_code();
    out.close();
    std::cout << "Copy:" << std::endl;
    std::cout << bb_copy.get_dot_code() << std::endl;
    out.open("boom_copy.dot");
    out << bb_copy.get_dot_code();
    out.close();

    BinaireBoom<int, int> bb_testcopy{bb};
    std::cout << "Zijn gelijk: " << (bb_testcopy == bb) << std::endl;
    assert(bb_testcopy == bb);
    bb_testcopy.voeg_toe(111, 1337);
    std::cout << "Zijn gelijk: " << (bb_testcopy == bb) << std::endl;
    assert(bb_testcopy != bb);

    BinaireBoom<int, int> bb_move{std::move(bb)};
    std::cout << "Origineel na move:" << std::endl;
    std::cout << bb.get_dot_code() << std::endl;
    out.open("originele_boom_na_move.dot");
    out << bb.get_dot_code();
    out.close();
    std::cout << "Move:" << std::endl;
    std::cout << bb_move.get_dot_code() << std::endl;
    out.open("boom_move.dot");
    out << bb_move.get_dot_code();
    out.close();

    BinaireBoom<int, int> bb_links{bb_copy};
    bb_links.roteer(Richting::LINKS);
    std::cout << "Roteer links:" << std::endl;
    std::cout << bb_links.get_dot_code() << std::endl;
    out.open("boom_roteer_links.dot");
    out << bb_links.get_dot_code();
    out.close();

    BinaireBoom<int, int> bb_rechts{bb_copy};
    bb_rechts.roteer(Richting::RECHTS);
    std::cout << "Roteer rechts:" << std::endl;
    std::cout << bb_rechts.get_dot_code() << std::endl;
    out.open("boom_roteer_rechts.dot");
    out << bb_rechts.get_dot_code();
    out.close();

    BinaireBoom<int, int> bb_onevenwichtig{bb_copy};
    bb_onevenwichtig.maak_onevenwichtig();
    std::cout << "Onevenwichtig:" << std::endl;
    std::cout << bb_onevenwichtig.get_dot_code() << std::endl;
    out.open("boom_onevenwichtig.dot");
    out << bb_onevenwichtig.get_dot_code();
    out.close();

    BinaireBoom<int, int> bb_evenwichtig{bb_copy};
    bb_evenwichtig.maak_evenwichtig();
    std::cout << "Evenwichtig:" << std::endl;
    std::cout << bb_evenwichtig.get_dot_code() << std::endl;
    out.open("boom_evenwichtig.dot");
    out << bb_evenwichtig.get_dot_code();
    out.close();

    //    test_groot_aantal_sleutels();
    test_iterator();

    test_evenwichtig();

    std::cout << "Done..." << std::endl;

    return 0;
}
