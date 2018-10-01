
#include "binaireboom.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>

using ::testing::ElementsAreArray;
using ::testing::Pair;
using ::testing::Test;

class BinaireBoomTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18};

        for (int key : keys)
        {
            bb.voeg_toe(key, (2 * key));
        }

        int keys2[] = {42, 16, 58, 8, 25, 49, 62};

        for (int key : keys2)
        {
            bb2.voeg_toe(key, (2 * key));
        }

        groot.voeg_toe(25, "25");
        groot.voeg_toe(15, "15");
        groot.voeg_toe(50, "50");
        groot.voeg_toe(10, "10");
        groot.voeg_toe(22, "22");
        groot.voeg_toe(35, "35");
        groot.voeg_toe(70, "70");
        groot.voeg_toe(4, "4");
        groot.voeg_toe(12, "12");
        groot.voeg_toe(18, "18");
        groot.voeg_toe(24, "24");
        groot.voeg_toe(31, "31");
        groot.voeg_toe(44, "44");
        groot.voeg_toe(66, "66");
        groot.voeg_toe(90, "90");
    }

    void TearDown() override
    {
    }

    BinaireBoom<int, int> bb;
    BinaireBoom<int, int> bb2;
    BinaireBoom<int, std::string> groot;
};

TEST_F(BinaireBoomTest, voeg_toe)
{
    ASSERT_THAT(groot.geef_inhoud_inorder(),
                ElementsAreArray({
                        Pair(4, "4"),
                        Pair(10, "10"),
                        Pair(12, "12"),
                        Pair(15, "15"),
                        Pair(18, "18"),
                        Pair(22, "22"),
                        Pair(24, "24"),
                        Pair(25, "25"),
                        Pair(31, "31"),
                        Pair(35, "35"),
                        Pair(44, "44"),
                        Pair(50, "50"),
                        Pair(66, "66"),
                        Pair(70, "70"),
                        Pair(90, "90"),
                }));
}

TEST_F(BinaireBoomTest, deepcopy)
{
    BinaireBoom<int, int> bb_testcopy{bb};
    ASSERT_EQ(bb_testcopy, bb);

    bb_testcopy.voeg_toe(111, 1337);

    ASSERT_NE(bb_testcopy, bb);
}

TEST_F(BinaireBoomTest, move)
{
    BinaireBoom<int, int> bb_testcopy{bb};
    BinaireBoom<int, int> bb_move{std::move(bb_testcopy)};

    ASSERT_EQ(bb_move, bb);
    ASSERT_EQ(bb_testcopy, (BinaireBoom<int, int>{}));
}

TEST_F(BinaireBoomTest, is_leeg)
{
    BinaireBoom<int, int> bb;

    ASSERT_TRUE(bb.is_leeg());

    bb.voeg_toe(1, 11);

    ASSERT_FALSE(bb.is_leeg());

    // TODO terug legen en testen
}

TEST_F(BinaireBoomTest, diepte)
{
    ASSERT_EQ(bb.diepte(), 8);
}

TEST_F(BinaireBoomTest, geef_inhoud_inorder)
{
    ASSERT_THAT(bb2.geef_inhoud_inorder(),
                ElementsAreArray({
                        Pair(8, 16),
                        Pair(16, 32),
                        Pair(25, 50),
                        Pair(42, 84),
                        Pair(49, 98),
                        Pair(58, 116),
                        Pair(62, 124),
                }));
}

TEST_F(BinaireBoomTest, geef_inhoud_preorder)
{
}

TEST_F(BinaireBoomTest, geef_inhoud_postorder)
{
}

TEST_F(BinaireBoomTest, roteer_links)
{
    BinaireBoom<int, int> bb_links{bb2};
    bb_links.roteer(Richting::LINKS);

    ASSERT_THAT(bb_links.geef_inhoud_inorder(),
                ElementsAreArray({
                        Pair(8, 16),
                        Pair(16, 32),
                        Pair(25, 50),
                        Pair(42, 84),
                        Pair(49, 98),
                        Pair(58, 116),
                        Pair(62, 124),
                }));

    ASSERT_EQ(bb_links->geef_sleutel(), 58);

    bb_links.roteer(Richting::RECHTS);
    ASSERT_EQ(bb_links, bb2);
}

TEST_F(BinaireBoomTest, roteer_rechts)
{
    BinaireBoom<int, int> bb_rechts{bb2};
    bb_rechts.roteer(Richting::RECHTS);

    ASSERT_THAT(bb_rechts.geef_inhoud_inorder(),
                ElementsAreArray({
                        Pair(8, 16),
                        Pair(16, 32),
                        Pair(25, 50),
                        Pair(42, 84),
                        Pair(49, 98),
                        Pair(58, 116),
                        Pair(62, 124),
                }));

    ASSERT_EQ(bb_rechts->geef_sleutel(), 16);

    bb_rechts.roteer(Richting::LINKS);
    ASSERT_EQ(bb_rechts, bb2);
}
