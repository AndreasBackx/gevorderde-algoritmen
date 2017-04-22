
#include "csv.h"
#include "../libraries/prettyprint.h"
#include "../labo-1-week-1b/zoekboom.h"
#include "puntgenerator.h"
#include "splayboomb.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include <cmath>
#include <random>

void generate_dot_file(const Splayboom<int, int>& sboom, std::string file_name)
{
    std::ofstream dotfile(file_name);
    assert(dotfile);
    dotfile << sboom.generate_dot_code() << std::endl;
    dotfile.close();
}

void test_splayboom()
{
    Splayboom<int, int> sboom;

    sboom.voeg_toe(8, 0);
    sboom.voeg_toe(3, 0);
    sboom.voeg_toe(1, 0);
    sboom.voeg_toe(6, 0);
    sboom.voeg_toe(10, 0);
    sboom.voeg_toe(9, 0);
    sboom.voeg_toe(14, 0);
    sboom.voeg_toe(13, 0);

    // geverifieerd met http://www.cs.usfca.edu/~galles/visualization/SplayTree.html
    const std::string opgestelde_splayboom("((((((NULL <- 1 -> (NULL <- 3 -> NULL)) <- 6 -> NULL) <- 8 -> NULL) <- 9 -> NULL) <- 10 -> NULL) <- 13 -> (NULL <- 14 -> NULL))");

    assert(sboom.to_string() == opgestelde_splayboom);

    generate_dot_file(sboom, "opbouw.dot");
}

void test_zoek_met_splay()
{
    Splayboom<int, int> sboom;

    sboom.voeg_toe(8, 0);
    sboom.voeg_toe(3, 0);
    sboom.voeg_toe(1, 0);
    sboom.voeg_toe(6, 0);
    sboom.voeg_toe(10, 0);
    sboom.voeg_toe(9, 0);
    sboom.voeg_toe(14, 0);
    sboom.voeg_toe(13, 0);

    // geverifieerd met http://www.cs.usfca.edu/~galles/visualization/SplayTree.html
    const std::string splayboom_na_zoek_2("((NULL <- 1 -> NULL) <- 3 -> (((NULL <- 6 -> NULL) <- 8 -> (NULL <- 9 -> NULL)) <- 10 -> (NULL <- 13 -> (NULL <- 14 -> NULL))))");
    const std::string splayboom_na_zoek_8("(((NULL <- 1 -> NULL) <- 3 -> (NULL <- 6 -> NULL)) <- 8 -> ((NULL <- 9 -> NULL) <- 10 -> (NULL <- 13 -> (NULL <- 14 -> NULL))))");

    Zoekknoop<int, int>* ouder;
    Zoekboom<int, int>* plaats;

    sboom.zoek(2, ouder, plaats);

    //assert(sboom.to_string() == opgestelde_splayboom);

    assert(sboom.to_string() == splayboom_na_zoek_2);

    generate_dot_file(sboom, "zoek_met_splay.dot");

    sboom.zoek(8, ouder, plaats);

    assert(sboom.to_string() == splayboom_na_zoek_8);

    generate_dot_file(sboom, "zoek_met_splay_2.dot");
}

bool equal(double a, double b)
{
    const double EPSILON = 0.01;

    return fabs(a - b) < EPSILON;
}

void test_potentiaal()
{
    Splayboom<int, int> sboom;

    sboom.Zoekboom::voeg_toe(8, 0);
    sboom.Zoekboom::voeg_toe(3, 0);
    sboom.Zoekboom::voeg_toe(1, 0);
    sboom.Zoekboom::voeg_toe(6, 0);
    sboom.Zoekboom::voeg_toe(10, 0);
    sboom.Zoekboom::voeg_toe(9, 0);
    sboom.Zoekboom::voeg_toe(14, 0);
    sboom.Zoekboom::voeg_toe(13, 0);

    assert(equal(sboom.potentiaal(), 7.59));
}

void test_puntgenerator()
{
    std::cout << "Generating puntgenerator.csv" << std::endl;

    CsvData grafiek("puntgenerator", '.');
    int AANTAL_DATAPUNTEN = 6000;
    int AANTAL_RESOLUTIE_PUNTEN = 50; // Er worden in totaal AANTAL_RESOLUTIE_PUNTEN punten gegenereerd om de AANTAL_DATAPUNTEN / AANTAL_RESOLUTIE_PUNTEN datapunten

    Zoekboom<double, unsigned int> zboom_puntgenerator;
    Zoekboom<double, unsigned int> zboom_random;
    vector<double> log2;
    vector<double> dieptedata_zboom_puntgenerator;
    vector<double> dieptedata_zboom_random;

    Puntgenerator pgen;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist;

    int aantal = 0;
    for (int i = 0; i < AANTAL_DATAPUNTEN; i++)
    {
        int puntgenerator_sleutel = pgen.geefpunt();
        int random_sleutel = dist(mt);

        try
        {
            zboom_puntgenerator.voeg_toe(puntgenerator_sleutel, i);
        }
        catch (const char* e)
        {
            std::cerr << "Toevoegen: " << puntgenerator_sleutel << ": " << e << std::endl;
        }

        try
        {
            zboom_random.voeg_toe(random_sleutel, i);
        }
        catch (const char* e)
        {
            std::cerr << "Toevoegen: " << random_sleutel << ": " << e << std::endl;
        }

        aantal++;

        if (aantal * AANTAL_RESOLUTIE_PUNTEN >= AANTAL_DATAPUNTEN)
        {
            std::cout << i << "/" << AANTAL_DATAPUNTEN << " | " << ((static_cast<double> (i) / static_cast<double> (AANTAL_DATAPUNTEN)) * 100) << "%" << std::endl;

            log2.push_back(std::log2(i));
            dieptedata_zboom_puntgenerator.push_back(zboom_puntgenerator.diepte());
            dieptedata_zboom_random.push_back(zboom_random.diepte());

            aantal = 0;
        }

    }

    grafiek.voegDataToe(log2);
    grafiek.voegDataToe(dieptedata_zboom_puntgenerator);
    grafiek.voegDataToe(dieptedata_zboom_random);
}

void test_grafiek_diepte_zoekboom_splayboom()
{
    std::cout << "Generating dieptedata_zoekboom_splayboom.csv" << std::endl;

    CsvData grafiek("dieptedata_zoekboom_splayboom", '.');
    int AANTAL_DATAPUNTEN = 6000;
    int AANTAL_RESOLUTIE_PUNTEN = 50; // Er worden in totaal AANTAL_RESOLUTIE_PUNTEN punten gegenereerd om de AANTAL_DATAPUNTEN / AANTAL_RESOLUTIE_PUNTEN datapunten

    Puntgenerator dePuntgenerator;

    Zoekboom<double, unsigned int> zboom;
    Splayboom<double, unsigned int> sboom;
    vector<double> log2;
    vector<double> dieptedata_zoekboom;
    vector<double> dieptedata_splayboom;

    int aantal = 0;
    for (int i = 0; i < AANTAL_DATAPUNTEN; i++)
    {
        zboom.voeg_toe(dePuntgenerator.geefpunt(), i);
        sboom.voeg_toe(dePuntgenerator.geefpunt(), i);
        aantal++;

        if (aantal * AANTAL_RESOLUTIE_PUNTEN >= AANTAL_DATAPUNTEN)
        {
            std::cout << i << "/" << AANTAL_DATAPUNTEN << " " << ((static_cast<double> (i) / static_cast<double> (AANTAL_DATAPUNTEN)) * 100) << "%" << std::endl;

            dieptedata_zoekboom.push_back(zboom.diepte());
            dieptedata_splayboom.push_back(sboom.diepte());

            aantal = 0;
        }
    }

    grafiek.voegDataToe(log2);
    grafiek.voegDataToe(dieptedata_zoekboom);
    grafiek.voegDataToe(dieptedata_splayboom);
}

void test_grafiek_diepte_potentiaal_zoekboom_splayboom_1mil_volgorde()
{
    std::cout << "Generating dieptedata_potentiaal_zoekboom_splayboom_1mil_volgorde.csv" << std::endl;

    CsvData grafiek("dieptedata_potentiaal_zoekboom_splayboom_1mil_volgorde", '.');
    int AANTAL_DATAPUNTEN = 1000000;
    int AANTAL_RESOLUTIE_PUNTEN = 1000; // Er worden in totaal AANTAL_RESOLUTIE_PUNTEN punten gegenereerd om de AANTAL_DATAPUNTEN / AANTAL_RESOLUTIE_PUNTEN datapunten

    // Visual C++: unsigned int, 4 bytes, 0 to 4,294,967,295
    Zoekboom<double, unsigned int> zboom;
    Splayboom<double, unsigned int> sboom;
    vector<double> dieptedata_zoekboom;
    vector<double> dieptedata_splayboom;
    vector<double> potentiaal_splayboom;

    int aantal = 0;
    for (int i = 0; i < AANTAL_DATAPUNTEN; i++)
    {
        zboom.voeg_toe(i, i);
        sboom.voeg_toe(i, i);
        aantal++;

        if (aantal * AANTAL_RESOLUTIE_PUNTEN >= AANTAL_DATAPUNTEN)
        {
            std::cout << i << "/" << AANTAL_DATAPUNTEN << " | " << ((static_cast<double> (i) / static_cast<double> (AANTAL_DATAPUNTEN)) * 100) << "%" << std::endl;

            dieptedata_zoekboom.push_back(zboom.diepte());
            dieptedata_splayboom.push_back(sboom.diepte());
            potentiaal_splayboom.push_back(sboom.potentiaal());

            aantal = 0;
        }
    }

    grafiek.voegDataToe(dieptedata_zoekboom);
    grafiek.voegDataToe(dieptedata_splayboom);
    grafiek.voegDataToe(potentiaal_splayboom);
}

void test_grafiek_diepte_potentiaal_zoekboom_splayboom_1mil_random()
{
    std::cout << "Generating dieptedata_potentiaal_zoekboom_splayboom_1mil_random.csv" << std::endl;

    CsvData grafiek("dieptedata_potentiaal_zoekboom_splayboom_1mil_random", '.');
    int AANTAL_DATAPUNTEN = 1000000;
    int AANTAL_RESOLUTIE_PUNTEN = 1000;

    // Visual C++: unsigned int, 4 bytes, [0, 4,294,967,295]
    Zoekboom<double, unsigned int> zboom;
    Splayboom<double, unsigned int> sboom;
    vector<double> dieptedata_zoekboom;
    vector<double> dieptedata_splayboom;
    vector<double> potentiaal_splayboom;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist; // range is [0, 2,147,483,647]

    int aantal = 0;
    for (int i = 0; i < AANTAL_DATAPUNTEN; i++)
    {
        // rand() geeft getallen terug tussen 0 en 32767 (wat te weinig is voor deze toepassing).

        int random_sleutel = dist(mt);

        try
        {
            zboom.voeg_toe(random_sleutel, i);
            sboom.voeg_toe(random_sleutel, i);

            aantal++;

            if (aantal * AANTAL_RESOLUTIE_PUNTEN >= AANTAL_DATAPUNTEN)
            {
                std::cout << i << "/" << AANTAL_DATAPUNTEN << " | " << ((static_cast<double> (i) / static_cast<double> (AANTAL_DATAPUNTEN)) * 100) << "%" << std::endl;

                dieptedata_zoekboom.push_back(zboom.diepte());
                dieptedata_splayboom.push_back(sboom.diepte());
                potentiaal_splayboom.push_back(sboom.potentiaal());

                aantal = 0;
            }
        }
        catch (const char* e)
        {
            // std::cerr << "Toevoegen: " << random_sleutel << ": " << e << std::endl;
            i--; // smerige hack om toch aan het correct aantal datapunten te komen: TODO omvormen naar betere code
        }
    }

    grafiek.voegDataToe(dieptedata_zoekboom);
    grafiek.voegDataToe(dieptedata_splayboom);
    grafiek.voegDataToe(potentiaal_splayboom);
}

// TODO
// - splayen door een stack met richtingen bij te houden ipv telkens via de parent te checken

int main(int argc, char *argv[])
{
    srand(time(0));

    test_splayboom();

    pretty_print::printLine();

    test_puntgenerator();

    pretty_print::printLine();

    test_zoek_met_splay();

    pretty_print::printLine();

    test_potentiaal();

    pretty_print::printLine();

    test_grafiek_diepte_zoekboom_splayboom();

    pretty_print::printLine();

    // Zeer diepe en grote bomen: dit zorgt voor stack overflows bij het bepalen van de diepte via recursive functies
    // Op te lossen door de stack te vegroten (Visual Studio: Project Properties -> Linker -> System -> Stack Reserve Size, GCC: --stack)
    // Deze functies kosten veel tijd!
    //
    // test_grafiek_diepte_potentiaal_zoekboom_splayboom_1mil_volgorde();
    // pretty_print::printLine();
    // test_grafiek_diepte_potentiaal_zoekboom_splayboom_1mil_random();

    return 0;
}
