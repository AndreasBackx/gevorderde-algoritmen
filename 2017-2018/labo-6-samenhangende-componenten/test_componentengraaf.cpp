#include "componentengraaf.h"
#include "gtest/gtest.h"

TEST(componentengraaf_test, constructor)
{
    Graaf<GERICHT> gr{10};
    gr.voegVerbindingToe(0, 1);
    gr.voegVerbindingToe(1, 0);
    gr.voegVerbindingToe(2, 1);
    gr.voegVerbindingToe(2, 3);
    gr.voegVerbindingToe(3, 4);
    gr.voegVerbindingToe(4, 2);
    gr.voegVerbindingToe(4, 5);
    gr.voegVerbindingToe(5, 8);
    gr.voegVerbindingToe(6, 5);
    gr.voegVerbindingToe(7, 6);
    gr.voegVerbindingToe(8, 7);
    gr.voegVerbindingToe(9, 8);

    ComponentenGraaf comp_gr{gr};

    EXPECT_EQ(comp_gr.aantalKnopen(), 4);
    EXPECT_EQ(comp_gr.aantalVerbindingen(), 3);

    EXPECT_EQ(comp_gr.geef_component(0), 2);
    EXPECT_EQ(comp_gr.geef_component(1), 2);
    EXPECT_EQ(comp_gr.geef_component(2), 3);
    EXPECT_EQ(comp_gr.geef_component(3), 3);
    EXPECT_EQ(comp_gr.geef_component(4), 3);
    EXPECT_EQ(comp_gr.geef_component(5), 0);
    EXPECT_EQ(comp_gr.geef_component(6), 0);
    EXPECT_EQ(comp_gr.geef_component(7), 0);
    EXPECT_EQ(comp_gr.geef_component(8), 0);
    EXPECT_EQ(comp_gr.geef_component(9), 1);

    EXPECT_NE(comp_gr.verbindingsnummer(1, 0), -1);
    EXPECT_NE(comp_gr.verbindingsnummer(3, 0), -1);
    EXPECT_NE(comp_gr.verbindingsnummer(3, 2), -1);

    EXPECT_EQ(comp_gr.verbindingsnummer(0, 0), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(0, 1), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(0, 2), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(0, 3), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(1, 1), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(1, 2), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(1, 3), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(2, 0), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(2, 1), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(2, 2), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(2, 3), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(3, 1), -1);
    EXPECT_EQ(comp_gr.verbindingsnummer(3, 3), -1);
}

TEST(componentengraaf_test, geef_component_error)
{
    Graaf<GERICHT> gr{10};
    gr.voegVerbindingToe(0, 1);
    gr.voegVerbindingToe(1, 0);
    gr.voegVerbindingToe(2, 1);
    gr.voegVerbindingToe(2, 3);
    gr.voegVerbindingToe(3, 4);
    gr.voegVerbindingToe(4, 2);
    gr.voegVerbindingToe(4, 5);
    gr.voegVerbindingToe(5, 8);
    gr.voegVerbindingToe(6, 5);
    gr.voegVerbindingToe(7, 6);
    gr.voegVerbindingToe(8, 7);
    gr.voegVerbindingToe(9, 8);

    ComponentenGraaf comp_gr{gr};

    EXPECT_THROW(comp_gr.geef_component(-1), GraafExceptie);
    EXPECT_THROW(comp_gr.geef_component(gr.aantalKnopen()), GraafExceptie);
}
