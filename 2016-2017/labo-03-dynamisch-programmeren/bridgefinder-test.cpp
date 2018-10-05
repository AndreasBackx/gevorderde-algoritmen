
#include "bridgefinder.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <fstream>

class BridgeFinderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        try_out = read_bridges_from_file("try-out.dat");
        try_out_2 = read_bridges_from_file("try-out-2.dat");
        versnick = read_bridges_from_file("versnick.dat");
    }

    std::vector<Bridge> read_bridges_from_file(const std::string& filename)
    {
        std::vector<Bridge> bridges;

        if (filename.empty())
        {
            throw "No filename given";;
        }

        std::ifstream in(filename);
        if (!in)
        {
            throw "Cannot open input file.";
        }

        int amount_of_bridges = 0;
        in >> amount_of_bridges;

        for (int i = 0; i < amount_of_bridges; i++)
        {
            if (!in)
            {
                throw "Cannot read the cost of a bridge.";
            }

            int cost = 0;
            in >> cost;

            if (!in)
            {
                throw "Cannot read the south city of a bridge.";
            }

            int other_city = 0;
            in >> other_city;

            if (other_city >= amount_of_bridges)
            {
                throw "Invalid input: city number is too large.";
            }

            bridges.push_back(Bridge(i, other_city, cost));
        }

        return bridges;
    }

    void TearDown() override
    {
    }

    std::vector<Bridge> try_out;
    std::vector<Bridge> try_out_2;
    std::vector<Bridge> versnick;
};

TEST_F(BridgeFinderTest, get_greatest_cost)
{
    BridgeFinder solver{try_out};

    ASSERT_EQ(solver.get_greatest_cost(), 4);

    solver = BridgeFinder{try_out_2};

    ASSERT_EQ(solver.get_greatest_cost(), 5);

    solver = BridgeFinder{versnick};

    ASSERT_EQ(solver.get_greatest_cost(), 43004);
}

/* try-out.dat
 * 
 *   | 3
 * 0 | 1 1
 * 1 | 2 2
 * 2 | 4 0
 * 
 * (2, 0)
 */

/*   | 8
 * 0 | 1 1
 * 1 | 1 5
 * 2 | 1 3
 * 3 | 1 4
 * 4 | 1 0
 * 5 | 1 6
 * 6 | 1 7
 * 7 | 1 2
 * 
 * (0, 1), (2, 3), (3, 4), (5, 6), (6, 7)
 */

/* versnick.dat
 *  
 * 60 bruggen met een kost van 43004:
 * 
 * 9
 * 13
 * 39
 * 47
 * 49
 * 58
 * 75
 * 109
 * 123
 * 148
 * 160
 * 171
 * 179
 * 202
 * 210
 * 217
 * 221
 * 244
 * 259
 * 266
 * 281
 * 310
 * 313
 * 321
 * 324
 * 332
 * 334
 * 381
 * 385
 * 403
 * 404
 * 429
 * 473
 * 476
 * 484
 * 487
 * 494
 * 503
 * 529
 * 540
 * 549
 * 568
 * 576
 * 598
 * 599
 * 600
 * 605
 * 613
 * 645
 * 647
 * 682
 * 708
 * 735
 * 771
 * 797
 * 814
 * 831
 * 881
 * 910
 * 978
 */
