
#include "../..//libraries/rood-zwarte-boom/rzboom-bu.h"

#include <fstream>
#include <iostream>
#include <sstream>

void test_bottom_up()
{
    RZBoom<int, int> rzb;

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18}; // http://lcm.csa.iisc.ernet.in/dsa/node91.html

    int i = 0;
    for (int key : keys)
    {
        rzb.voeg_toe(key, 2 * key);

        std::ofstream out;
        std::stringstream naam;
        naam << "boom_voegtoe_bottom_up_" << i << "_sleutel" << key << ".dot";
        out.open(naam.str());
        out << rzb.get_dot_code();
        out.close();

        i++;
    }

    assert(rzb.is_rep_ok());
}

void test_top_down()
{
    int keys[] = {4, 7, 10, 2, 5, 6, 3, 8, 0, 9, 1}; // https://www.coursehero.com/file/22593733/red-black-examples/

    RZBoom<int, int> rzb;

    i = 0;
    for (int key : keys2)
    {
        rzb_top_down.voeg_toe(key, 2 * key);

        std::ofstream out;
        std::stringstream naam;
        naam << "boom_voegtoe_top_down_" << i << "_sleutel" << key << ".dot";
        out.open(naam.str());
        out << rzb_top_down.get_dot_code();
        out.close();

        i++;
    }

    assert(rzb_top_down.is_rep_ok());
}

int main()
{
    test_bottom_up();
    // test_top_down();

    std::cout << "Done..." << std::endl;

    return 0;
}
