
#include "gtest/gtest.h"

#include "schijf.h"

class SchijfTest : public ::testing::Test
{
protected:
    // void SetUp() override {}
    // void TearDown() override {}

    Schijf<int> schijf;
};

TEST_F(SchijfTest, constructor)
{
    ASSERT_NE(schijf.schrijf(1337), 0);
}

TEST_F(SchijfTest, schrijf)
{
    blokindex b = schijf.schrijf(1337);
    int res = 0;

    schijf.lees(res, b);
    ASSERT_EQ(res, 1337);
}

TEST_F(SchijfTest, herschrijf)
{
    blokindex b = schijf.schrijf(1337);
    int res = 0;

    schijf.lees(res, b);
    ASSERT_EQ(res, 1337);

    schijf.herschrijf(101, b);
    schijf.lees(res, b);
    ASSERT_EQ(res, 101);
}