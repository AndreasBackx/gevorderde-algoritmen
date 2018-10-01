
#include <iostream>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <string>

#include "../../libraries/binaire-boom/binaireboom.h"
#include "../tools/prettyprint.h"

// TODO
// - terug evenwijdig maken op alle niveaus via recursie
// - genereer dot uitbreiden om ook de ouderpointers weer te geven

void print_dot_code_to_file(const Zoekboom<int, int>& boom, std::string file_name)
{
    std::ofstream dotfile(file_name);
    assert(dotfile);
    dotfile << boom.generate_dot_code() << std::endl;
    dotfile.close();
}

void test_boom()
{
    Zoekboom<int, int> boom;

    assert(boom.is_leeg());

    boom.voeg_toe(8, 0);
    boom.voeg_toe(3, 0);
    boom.voeg_toe(1, 0);
    boom.voeg_toe(6, 0);
    boom.voeg_toe(10, 0);
    boom.voeg_toe(9, 0);
    boom.voeg_toe(14, 0);
    boom.voeg_toe(13, 0);

    assert(!boom.is_leeg());

    std::cout << "Boom: " << std::endl
        << std::endl
        << boom << std::endl;

    assert(boom.diepte() == 3);
    assert(boom.gemiddelde_diepte() == (static_cast<double> (13) / static_cast<double> (8))); // (0 + 1 + 2 + 2 + 1 + 2 + 2 + 3) / 8 = 13 / 8 = 1.625

    const std::string correcte_boom_string{"(((NULL <- 1 -> NULL) <- 3 -> (NULL <- 6 -> NULL)) <- 8 -> ((NULL <- 9 -> NULL) <- 10 -> ((NULL <- 13 -> NULL) <- 14 -> NULL)))"};
    assert(boom.to_string() == correcte_boom_string);

    std::cout << boom.generate_dot_code() << std::endl;

    print_dot_code_to_file(boom, "boom.dot");
}

void test_rotatie_links()
{
    Zoekboom<int, int> boom;

    boom.voeg_toe(8, 0);
    boom.voeg_toe(3, 0);
    boom.voeg_toe(1, 0);
    boom.voeg_toe(6, 0);
    boom.voeg_toe(10, 0);
    boom.voeg_toe(9, 0);
    boom.voeg_toe(14, 0);
    boom.voeg_toe(13, 0);

    const std::string correcte_boom_string_rotatie_links{"((((NULL <- 1 -> NULL) <- 3 -> (NULL <- 6 -> NULL)) <- 8 -> (NULL <- 9 -> NULL)) <- 10 -> ((NULL <- 13 -> NULL) <- 14 -> NULL))"};

    boom.roteer(Richting::LINKS);

    std::cout << "Boom geroteerd naar rechts (efficient): " << std::endl
        << std::endl
        << boom << std::endl;
    assert(boom.to_string() == correcte_boom_string_rotatie_links);

    print_dot_code_to_file(boom, "boom_rotatie_links.dot");
}

void test_rotatie_rechts()
{
    Zoekboom<int, int> boom;

    boom.voeg_toe(8, 0);
    boom.voeg_toe(3, 0);
    boom.voeg_toe(1, 0);
    boom.voeg_toe(6, 0);
    boom.voeg_toe(10, 0);
    boom.voeg_toe(9, 0);
    boom.voeg_toe(14, 0);
    boom.voeg_toe(13, 0);

    const std::string correcte_boom_string_rotatie_rechts{"((NULL <- 1 -> NULL) <- 3 -> ((NULL <- 6 -> NULL) <- 8 -> ((NULL <- 9 -> NULL) <- 10 -> ((NULL <- 13 -> NULL) <- 14 -> NULL))))"};

    boom.roteer(Richting::RECHTS);
    std::cout << "Boom geroteerd naar links (efficient): " << std::endl
        << std::endl
        << boom << std::endl;
    assert(boom.to_string() == correcte_boom_string_rotatie_rechts);

    print_dot_code_to_file(boom, "boom_rotatie_rechts.dot");
}

void test_evenwichtigheid()
{
    const std::string correcte_onevenwichtige_boom{"(NULL <- 1 -> (NULL <- 3 -> (NULL <- 6 -> (NULL <- 8 -> (NULL <- 9 -> (NULL <- 10 -> (NULL <- 13 -> (NULL <- 14 -> NULL))))))))"};
    const std::string correcte_onevenwichtige_boom_weer_evenwichtig{"((((NULL <- 1 -> NULL) <- 3 -> NULL) <- 6 -> NULL) <- 8 -> (NULL <- 9 -> (NULL <- 10 -> (NULL <- 13 -> (NULL <- 14 -> NULL)))))"};

    Zoekboom<int, int> boom;

    assert(boom.is_leeg());

    boom.voeg_toe(8, 0);
    boom.voeg_toe(3, 0);
    boom.voeg_toe(1, 0);
    boom.voeg_toe(6, 0);
    boom.voeg_toe(10, 0);
    boom.voeg_toe(9, 0);
    boom.voeg_toe(14, 0);
    boom.voeg_toe(13, 0);

    boom.maak_onevenwichtig();

    std::cout << "Onevenwichtige boom: " << std::endl
        << std::endl
        << boom << std::endl;
    assert(boom.to_string() == correcte_onevenwichtige_boom);
    assert(boom.diepte() == 7);

    print_dot_code_to_file(boom, "onevenwichtige_boom.dot");

    boom.maak_onevenwichtig_weer_evenwichtig();

    std::cout << "Onevenwichtige boom terug evenwichtig gemaakt: " << std::endl
        << std::endl <<
        boom << std::endl;
    std::cout << boom.to_string() << std::endl;
    assert(boom.to_string() == correcte_onevenwichtige_boom_weer_evenwichtig);
    assert(boom.diepte() == 4);

    print_dot_code_to_file(boom, "terug_evenwichtige_boom.dot");
}

int main(int argc, char** argv)
{
    pretty_print::printLine();

    test_boom();

    pretty_print::printLine();

    test_rotatie_links();

    pretty_print::printLine();

    test_rotatie_rechts();

    pretty_print::printLine();

    test_evenwichtigheid();

    std::cout << std::endl << "Done..." << std::endl;

    return 0;
}

