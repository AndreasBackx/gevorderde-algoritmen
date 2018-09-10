
#include "binaireboom.h"
#include "gtest/gtest.h"

#include <string>

TEST(binaire_boom, voeg_toe)
{
    BinaireBoom<int, std::string> bb;

    bb.voeg_toe(25, "25");
    bb.voeg_toe(15, "15");
    bb.voeg_toe(50, "50");
    bb.voeg_toe(10, "10");
    bb.voeg_toe(22, "22");
    bb.voeg_toe(35, "35");
    bb.voeg_toe(70, "70");
    bb.voeg_toe(4, "4");
    bb.voeg_toe(12, "12");
    bb.voeg_toe(18, "18");
    bb.voeg_toe(24, "24");
    bb.voeg_toe(31, "31");
    bb.voeg_toe(44, "44");
    bb.voeg_toe(66, "66");
    bb.voeg_toe(90, "90");

    std::vector<std::pair<int, std::string>> inhoud_bb = bb.geef_inhoud_inorder();

    ASSERT_EQ(inhoud_bb.size(), 15);
    ASSERT_EQ(inhoud_bb[0].first, 4);
    ASSERT_EQ(inhoud_bb[0].second, "4");
    ASSERT_EQ(inhoud_bb[1].first, 10);
    ASSERT_EQ(inhoud_bb[1].second, "10");
    ASSERT_EQ(inhoud_bb[2].first, 12);
    ASSERT_EQ(inhoud_bb[2].second, "12");
    ASSERT_EQ(inhoud_bb[3].first, 15);
    ASSERT_EQ(inhoud_bb[3].second, "15");
    ASSERT_EQ(inhoud_bb[4].first, 18);
    ASSERT_EQ(inhoud_bb[4].second, "18");
    ASSERT_EQ(inhoud_bb[5].first, 22);
    ASSERT_EQ(inhoud_bb[5].second, "22");
    ASSERT_EQ(inhoud_bb[6].first, 24);
    ASSERT_EQ(inhoud_bb[6].second, "24");
    ASSERT_EQ(inhoud_bb[7].first, 25);
    ASSERT_EQ(inhoud_bb[7].second, "25");
    ASSERT_EQ(inhoud_bb[8].first, 31);
    ASSERT_EQ(inhoud_bb[8].second, "31");
    ASSERT_EQ(inhoud_bb[9].first, 35);
    ASSERT_EQ(inhoud_bb[9].second, "35");
    ASSERT_EQ(inhoud_bb[10].first, 44);
    ASSERT_EQ(inhoud_bb[10].second, "44");
    ASSERT_EQ(inhoud_bb[11].first, 50);
    ASSERT_EQ(inhoud_bb[11].second, "50");
    ASSERT_EQ(inhoud_bb[12].first, 66);
    ASSERT_EQ(inhoud_bb[12].second, "66");
    ASSERT_EQ(inhoud_bb[13].first, 70);
    ASSERT_EQ(inhoud_bb[13].second, "70");
    ASSERT_EQ(inhoud_bb[14].first, 90);
    ASSERT_EQ(inhoud_bb[14].second, "90");
}

TEST(binaire_boom, deepcopy)
{
    BinaireBoom<int, int> bb;

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18};

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    BinaireBoom<int, int> bb_testcopy{bb};
    ASSERT_EQ(bb_testcopy, bb);

    bb_testcopy.voeg_toe(111, 1337);

    ASSERT_NE(bb_testcopy, bb);
}

TEST(binaire_boom, move)
{
    BinaireBoom<int, int> bb;

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18};

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    BinaireBoom<int, int> bb_testcopy{bb};
    BinaireBoom<int, int> bb_move{std::move(bb_testcopy)};

    ASSERT_EQ(bb_move, bb);
    ASSERT_EQ(bb_testcopy, (BinaireBoom<int, int>{}));
}

TEST(binaire_boom, diepte)
{
    BinaireBoom<int, int> bb;

    int keys[] = {13, 3, 4, 12, 14, 10, 5, 1, 8, 2, 7, 9, 11, 6, 18};

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    ASSERT_EQ(bb.diepte(), 8);
}

TEST(binaire_boom, roteer_links)
{
    BinaireBoom<int, int> bb;

    int keys[] = {42, 16, 58, 8, 25, 49, 62};

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    std::vector<std::pair<int, int>> inhoud_bb = bb.geef_inhoud_inorder();

    ASSERT_EQ(inhoud_bb.size(), 7);
    ASSERT_EQ(inhoud_bb[0].first, 8);
    ASSERT_EQ(inhoud_bb[1].first, 16);
    ASSERT_EQ(inhoud_bb[2].first, 25);
    ASSERT_EQ(inhoud_bb[3].first, 42);
    ASSERT_EQ(inhoud_bb[4].first, 49);
    ASSERT_EQ(inhoud_bb[5].first, 58);
    ASSERT_EQ(inhoud_bb[6].first, 62);
    ASSERT_EQ(bb->geef_sleutel(), 42);

    BinaireBoom<int, int> bb_links{bb};
    bb_links.roteer(Richting::LINKS);
    inhoud_bb = bb_links.geef_inhoud_inorder();

    ASSERT_EQ(inhoud_bb.size(), 7);
    ASSERT_EQ(inhoud_bb[0].first, 8);
    ASSERT_EQ(inhoud_bb[1].first, 16);
    ASSERT_EQ(inhoud_bb[2].first, 25);
    ASSERT_EQ(inhoud_bb[3].first, 42);
    ASSERT_EQ(inhoud_bb[4].first, 49);
    ASSERT_EQ(inhoud_bb[5].first, 58);
    ASSERT_EQ(inhoud_bb[6].first, 62);
    ASSERT_EQ(bb_links->geef_sleutel(), 58);

    bb_links.roteer(Richting::RECHTS);
    ASSERT_EQ(bb_links, bb);
}

TEST(binaire_boom, roteer_rechts)
{
    BinaireBoom<int, int> bb;

    int keys[] = {42, 16, 58, 8, 25, 49, 62};

    for (int key : keys)
    {
        bb.voeg_toe(key, 2 * key);
    }

    std::vector<std::pair<int, int>> inhoud_bb = bb.geef_inhoud_inorder();

    ASSERT_EQ(inhoud_bb.size(), 7);
    ASSERT_EQ(inhoud_bb[0].first, 8);
    ASSERT_EQ(inhoud_bb[1].first, 16);
    ASSERT_EQ(inhoud_bb[2].first, 25);
    ASSERT_EQ(inhoud_bb[3].first, 42);
    ASSERT_EQ(inhoud_bb[4].first, 49);
    ASSERT_EQ(inhoud_bb[5].first, 58);
    ASSERT_EQ(inhoud_bb[6].first, 62);
    ASSERT_EQ(bb->geef_sleutel(), 42);

    BinaireBoom<int, int> bb_rechts{bb};
    bb_rechts.roteer(Richting::RECHTS);
    inhoud_bb = bb_rechts.geef_inhoud_inorder();

    ASSERT_EQ(inhoud_bb.size(), 7);
    ASSERT_EQ(inhoud_bb[0].first, 8);
    ASSERT_EQ(inhoud_bb[1].first, 16);
    ASSERT_EQ(inhoud_bb[2].first, 25);
    ASSERT_EQ(inhoud_bb[3].first, 42);
    ASSERT_EQ(inhoud_bb[4].first, 49);
    ASSERT_EQ(inhoud_bb[5].first, 58);
    ASSERT_EQ(inhoud_bb[6].first, 62);
    ASSERT_EQ(bb_rechts->geef_sleutel(), 16);

    bb_rechts.roteer(Richting::LINKS);
    ASSERT_EQ(bb_rechts, bb);
}

TEST(knoop, geef_sleutel)
{
    Knoop<int, std::string> kn{101, "Dit is een test"};

    ASSERT_EQ(kn.geef_sleutel(), 101);
}

TEST(knoop, geef_data)
{
    Knoop<int, std::string> kn{101, "Dit is een test"};

    ASSERT_EQ(kn.geef_data(), "Dit is een test");
}
