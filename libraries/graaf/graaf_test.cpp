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

TEST(graaf_test, voeg_verbinding_toe)
{
    Graaf<ONGERICHT> graaf{4};

    int verbind_nr1 = graaf.voegVerbindingToe(0, 1);

    EXPECT_EQ(graaf.aantalVerbindingen(), 1);
    EXPECT_EQ(verbind_nr1, 0);
    EXPECT_EQ(graaf.verbindingsnummer(0, 1), graaf.verbindingsnummer(1, 0));

    int verbind_nr2 = graaf.voegVerbindingToe(1, 2);
    int verbind_nr3 = graaf.voegVerbindingToe(2, 3);
    int verbind_nr4 = graaf.voegVerbindingToe(3, 0);

    EXPECT_EQ(graaf.aantalVerbindingen(), 4);
    EXPECT_EQ(graaf.verbindingsnummer(0, 1), verbind_nr1);
    EXPECT_EQ(graaf.verbindingsnummer(1, 2), verbind_nr2);
    EXPECT_EQ(graaf.verbindingsnummer(2, 3), verbind_nr3);
    EXPECT_EQ(graaf.verbindingsnummer(3, 0), verbind_nr4);

    EXPECT_THROW(graaf.voegVerbindingToe(0, graaf.aantalKnopen()), GraafExceptie);
    EXPECT_THROW(graaf.voegVerbindingToe(0, 1), GraafExceptie);
    EXPECT_THROW(graaf.voegVerbindingToe(0, -1), GraafExceptie);

    EXPECT_NO_THROW(graaf.voegVerbindingToe(0, 0));
    EXPECT_EQ(graaf.aantalVerbindingen(), 5);
}

TEST(graaf_test, verbindingsnummer)
{
    Graaf<ONGERICHT> graaf{4};

    int verbind_nr1 = graaf.voegVerbindingToe(0, 1);
    int verbind_nr2 = graaf.voegVerbindingToe(1, 2);
    int verbind_nr3 = graaf.voegVerbindingToe(2, 3);
    int verbind_nr4 = graaf.voegVerbindingToe(3, 0);

    EXPECT_EQ(graaf.verbindingsnummer(0, 1), verbind_nr1);
    EXPECT_EQ(graaf.verbindingsnummer(1, 2), verbind_nr2);
    EXPECT_EQ(graaf.verbindingsnummer(2, 3), verbind_nr3);
    EXPECT_EQ(graaf.verbindingsnummer(3, 0), verbind_nr4);

    EXPECT_THROW(graaf.verbindingsnummer(0, graaf.aantalKnopen()), GraafExceptie);
    EXPECT_THROW(graaf.verbindingsnummer(0, -1), GraafExceptie);
    EXPECT_EQ(graaf.verbindingsnummer(1, 3), -1);
}

TEST(graaf_test, verwijder_verbinding)
{
    Graaf<ONGERICHT> graaf{4};

    graaf.voegVerbindingToe(0, 1);
    graaf.voegVerbindingToe(1, 2);
    graaf.voegVerbindingToe(2, 3);
    graaf.voegVerbindingToe(3, 0);

    graaf.verwijderVerbinding(2, 3);

    EXPECT_EQ(graaf.aantalVerbindingen(), 3);

    EXPECT_NO_THROW(graaf.verwijderVerbinding(2, 3));
    EXPECT_EQ(graaf.aantalVerbindingen(), 3);

    EXPECT_NO_THROW(graaf.verwijderVerbinding(0, 3));
    EXPECT_EQ(graaf.aantalVerbindingen(), 2);

    EXPECT_THROW(graaf.verwijderVerbinding(0, graaf.aantalKnopen()), GraafExceptie);
    EXPECT_THROW(graaf.verwijderVerbinding(0, -1), GraafExceptie);
}

TEST(graaf_test, wis)
{
    Graaf<ONGERICHT> graaf{4};

    graaf.voegVerbindingToe(0, 1);
    graaf.voegVerbindingToe(1, 2);
    graaf.voegVerbindingToe(2, 3);
    graaf.voegVerbindingToe(3, 0);
    graaf.voegVerbindingToe(1, 3);
    graaf.voegVerbindingToe(0, 2);

    EXPECT_EQ(graaf.aantalKnopen(), 4);
    EXPECT_EQ(graaf.aantalVerbindingen(), 6);

    graaf.wis();

    EXPECT_EQ(graaf.aantalKnopen(), 0);
    EXPECT_EQ(graaf.aantalVerbindingen(), 0);

    int knoop_nr1 = graaf.voegKnoopToe();
    int knoop_nr2 = graaf.voegKnoopToe();
    int verbind_nr = graaf.voegVerbindingToe(0, 1);

    EXPECT_EQ(graaf.aantalKnopen(), 2);
    EXPECT_EQ(graaf.aantalVerbindingen(), 1);
    EXPECT_EQ(knoop_nr1, 0);
    EXPECT_EQ(knoop_nr2, 1);
    EXPECT_EQ(verbind_nr, 0);
}
