
#include "rzboom-bu.h"
#include "rzboom-td.h"
#include "gtest/gtest.h"

#include <string>

// http://lcm.csa.iisc.ernet.in/dsa/node91.html
TEST(rzboom, voeg_toe_bottom_up)
{
    RZBoomBottomUp<int, std::string> rzb;
    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18};

    for (int key : keys)
    {
        rzb.voeg_toe(key, std::to_string(key));
    }

    ASSERT_TRUE(rzb.is_rep_ok());
}

// https://www.coursehero.com/file/22593733/red-black-examples/
TEST(rzboom, voeg_toe_top_down)
{
    RZBoomTopDown<int, std::string> rzb;
    int keys[] = {4, 7, 10, 2, 5, 6, 3, 8, 0, 9, 1};

    for (int key : keys)
    {
        rzb.voeg_toe(key, std::to_string(key));
    }

    ASSERT_TRUE(rzb.is_rep_ok());
}
