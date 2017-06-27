

#ifndef BKNOOP_UNITTEST_H
#define BKNOOP_UNITTEST_H

#include "gtest/gtest.h"
#include "../bknoop.h"

#include <iostream>

TEST(test_bknoop, test_is_aanwezig)
{
    BKnoop<int, int, 8> b{true};

    b.voegtoe(123, 321, 0);

    std::cout << b.to_string() << std::endl;

    ASSERT_TRUE(b.is_aanwezig(123));
}

TEST(test_bknoop, test_is_vol)
{
    BKnoop<int, int, 8> b{true};

    b.voegtoe(0, 100, 0);
    // ASSERT_FALSE(b.is_vol());

    // b.voegtoe(1, 111, 0);
    // ASSERT_FALSE(b.is_vol());

    // b.voegtoe(2, 222, 0);
    // ASSERT_FALSE(b.is_vol());

    // b.voegtoe(3, 333, 0);
    // ASSERT_FALSE(b.is_vol());

    // b.voegtoe(4, 444, 0);
    // ASSERT_FALSE(b.is_vol());

    // b.voegtoe(5, 555, 0);
    // ASSERT_FALSE(b.is_vol());

    // b.voegtoe(6, 666, 0);
    // ASSERT_FALSE(b.is_vol());

    std::cout << b.to_string() << std::endl;

    // b.voegtoe(7, 777, 0);
    // ASSERT_TRUE(b.is_vol());
}

#endif /* BKNOOP_UNITTEST_H */
