#include "woordstroom.h"
#include "gtest/gtest.h"

#include <iostream>
#include <istream>
#include <string>
#include <vector>

TEST(woordstroom_test, lees)
{
    std::string woord;
    Woordstroom stroom("test.txt");

    std::vector<std::string> woorden;

    while (stroom >> woord)
    {
        woorden.push_back(woord);
    }

    // Hello World! 123abcABC.987!poi{a1a}
    // 1A2Blolzor.

    EXPECT_EQ(woorden.size(), 10);

    EXPECT_EQ(woorden[0], "Hello");
    EXPECT_EQ(woorden[1], " ");
    EXPECT_EQ(woorden[2], "World");
    EXPECT_EQ(woorden[3], "! 123");
    EXPECT_EQ(woorden[4], "abcABC");
    EXPECT_EQ(woorden[5], ".987!");
    EXPECT_EQ(woorden[6], "poi");
    EXPECT_EQ(woorden[7], "{");
    EXPECT_EQ(woorden[8], "a");
    EXPECT_EQ(woorden[9], "1");
    EXPECT_EQ(woorden[10], "a");
    EXPECT_EQ(woorden[11], "}\n1");
    EXPECT_EQ(woorden[12], "A");
    EXPECT_EQ(woorden[13], "2");
    EXPECT_EQ(woorden[14], "Blolzor");
    EXPECT_EQ(woorden[15], ".\n");
}
