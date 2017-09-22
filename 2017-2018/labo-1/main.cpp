
#include "zoekboom11.h"

#include <string>
#include <iostream>
#include <fstream>

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
        zb.voegtoe(key, 2 * key);
    }

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

    return 0;
}
