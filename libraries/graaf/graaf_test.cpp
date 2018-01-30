#include "graaf.h"
#include "gtest/gtest.h"

TEST(graaf_test, constructor)
{
    const int SIZE = 20;

    Graaf<ONGERICHT> graaf{SIZE};

    EXPECT_EQ(graaf.aantalKnopen(), SIZE);
    EXPECT_EQ(graaf.aantalVerbindingen(), 0);
}

TEST(graaf_test, constructor_with_default_parameter)
{
    Graaf<ONGERICHT> graaf;

    EXPECT_EQ(graaf.aantalKnopen(), 0);
    EXPECT_EQ(graaf.aantalVerbindingen(), 0);
}

TEST(graaf_test, is_gericht)
{
    Graaf<ONGERICHT> ongerichte_graaf;
    Graaf<GERICHT> gerichte_graaf;

    EXPECT_FALSE(ongerichte_graaf.isGericht());
    EXPECT_TRUE(gerichte_graaf.isGericht());
}

TEST(graaf_test, voeg_knoop_toe)
{
    Graaf<ONGERICHT> graaf;

    EXPECT_EQ(graaf.aantalKnopen(), 0);

    int knoop_nr = graaf.voegKnoopToe();

    EXPECT_EQ(graaf.aantalKnopen(), 1);
    EXPECT_EQ(knoop_nr, 0);

    knoop_nr = graaf.voegKnoopToe();

    EXPECT_EQ(graaf.aantalKnopen(), 2);
}

TEST(graaf_test, voeg_verbinding_toe_ongericht)
{
    Graaf<ONGERICHT> graaf{4};

    int verbind_nr = graaf.voegVerbindingToe(0, 1);

    EXPECT_EQ(graaf.aantalVerbindingen(), 1);
    EXPECT_EQ(verbind_nr, 0);

    verbind_nr = graaf.voegVerbindingToe(1, 2);
    verbind_nr = graaf.voegVerbindingToe(2, 3);
    verbind_nr = graaf.voegVerbindingToe(3, 0);

    EXPECT_EQ(graaf.aantalVerbindingen(), 4);

    EXPECT_THROW(graaf.voegVerbindingToe(0, graaf.aantalKnopen()), GraafExceptie);
    EXPECT_THROW(graaf.voegVerbindingToe(0, 1), GraafExceptie);
    EXPECT_THROW(graaf.voegVerbindingToe(0, -1), GraafExceptie);

    EXPECT_NO_THROW(graaf.voegVerbindingToe(0, 0));

    EXPECT_EQ(graaf.aantalVerbindingen(), 5);
}
