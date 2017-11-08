
#include "rzboom14.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main()
{
    RZBoom<int, int> rzb;

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18}; // http://lcm.csa.iisc.ernet.in/dsa/node91.html

    int i = 0;
    for (int key : keys)
    {
        rzb.voeg_toe_bottom_up(key, 2 * key);

        std::ofstream out;
        std::stringstream naam;
        naam << "boom_voegtoe_bottom_up_" << i << "_sleutel" << key << ".dot";
        out.open(naam.str());
        out << rzb.get_dot_code();
        out.close();

        // std::cout << i << std::endl;
        i++;
    }

    assert(rzb.is_rep_ok());

    int keys2[] = {4, 7, 10, 2, 5, 6, 3, 8, 0, 9, 1}; // https://www.coursehero.com/file/22593733/red-black-examples/

    RZBoom<int, int> rzb_top_down;

    i = 0;
    for (int key : keys2)
    {
        rzb_top_down.voeg_toe_top_down(key, 2 * key);

        std::ofstream out;
        std::stringstream naam;
        naam << "boom_voegtoe_top_down_" << i << "_sleutel" << key << ".dot";
        out.open(naam.str());
        out << rzb_top_down.get_dot_code();
        out.close();

        // std::cout << i << std::endl;
        i++;
    }

    assert(rzb_top_down.is_rep_ok());

    std::cout << "Done..." << std::endl;

    return 0;
}
