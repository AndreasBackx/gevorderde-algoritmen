
#include "rzboom14.h"

#include <iostream>
#include <fstream>
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

        std::cout << i << std::endl;
        i++;
    }


    std::cout << "Done..." << std::endl;

    return 0;
}
