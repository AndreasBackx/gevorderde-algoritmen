

#ifndef BKNOOP_UNITTEST_H
#define BKNOOP_UNITTEST_H

#include "gtest/gtest.h"
#include "../bknoop.h"

TEST(test_bknoop, test_is_aanwezig)
{
    BKnoop<int, int, 8> b{true};

    b.voegtoe(123, 321, 0);

    ASSERT_TRUE(b.is_aanwezig(123));
    ASSERT_FALSE(b.is_aanwezig(789));
}

TEST(test_bknoop, test_is_vol)
{
    BKnoop<int, int, 8> b{true};

    b.voegtoe(0, 100, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(1, 111, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(2, 222, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(3, 333, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(4, 444, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(5, 555, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(6, 666, 0);
    ASSERT_FALSE(b.is_vol());

    b.voegtoe(7, 777, 0);
    ASSERT_TRUE(b.is_vol());
}

TEST(test_bknoop, test_voegtoe)
{
    BKnoop<int, int, 8> b{true};

    b.voegtoe(1, 111, 0);
    b.voegtoe(5, 555, 0);
    b.voegtoe(2, 222, 0);
    b.voegtoe(8, 888, 0);
    b.voegtoe(7, 777, 0);
    b.voegtoe(4, 444, 0);
    b.voegtoe(3, 333, 0);
    b.voegtoe(6, 666, 0);

    ASSERT_EQ(1, b[0]);
    ASSERT_EQ(2, b[1]);
    ASSERT_EQ(3, b[2]);
    ASSERT_EQ(4, b[3]);
    ASSERT_EQ(5, b[4]);
    ASSERT_EQ(6, b[5]);
    ASSERT_EQ(7, b[6]);
    ASSERT_EQ(8, b[7]);
}

TEST(test_bknoop, test_index_operator) /* operator[] */
{
    BKnoop<int, int, 8> b{true};

    b.voegtoe(1, 111, 0);
    b.voegtoe(2, 222, 0);
    b.voegtoe(3, 333, 0);

    ASSERT_THROW(b[-1], const char*);
    ASSERT_EQ(1, b[0]);
    ASSERT_EQ(2, b[1]);
    ASSERT_EQ(3, b[2]);
    ASSERT_THROW(b[3], const char*);
}

TEST(test_bknoop, test_geef_data)
{
    BKnoop<int, int, 3> b{true};

    b.voegtoe(1, 111, 0);
    b.voegtoe(2, 222, 0);
    b.voegtoe(3, 333, 0);

    ASSERT_EQ(222, b.geef_data(2));
    ASSERT_THROW(b.geef_data(123), const char*);
}

TEST(test_bknoop, test_aantal_kinderen)
{
    BKnoop<int, int, 8> b{true};
    ASSERT_EQ(0, b.aantal_kinderen());

    b.voegtoe(1, 111, 0);
    ASSERT_EQ(1, b.aantal_kinderen());

    b.voegtoe(2, 222, 0);
    ASSERT_EQ(2, b.aantal_kinderen());

    b.voegtoe(3, 333, 0);
    ASSERT_EQ(3, b.aantal_kinderen());
}

TEST(test_bknoop, test_geef_index)
{
    BKnoop<int, int, 8> b{0, 10, 1010, 1};

    b.voegtoe(20, 2020, 2);
    b.voegtoe(30, 3030, 3);
    b.voegtoe(40, 4040, 4);
    b.voegtoe(50, 5050, 5);
    b.voegtoe(60, 6060, 6);

    ASSERT_THROW(b.geef_kindindex(20), const char*);

    ASSERT_EQ(0, b.geef_kindindex(5));
    ASSERT_EQ(4, b.geef_kindindex(45));
    ASSERT_EQ(6, b.geef_kindindex(100));
}

#endif /* BKNOOP_UNITTEST_H */
