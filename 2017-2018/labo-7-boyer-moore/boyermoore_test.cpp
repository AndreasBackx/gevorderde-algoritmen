#include "boyermoore.h"
#include "gtest/gtest.h"

TEST(boyermoore, weak_common_suffix_table)
{
    BoyerMoore bm{"ababdab"};
    std::vector<int> weak_common_suffix_table = bm.get_weak_common_suffix_table();

    ASSERT_EQ(weak_common_suffix_table.size(), 7);
    ASSERT_EQ(weak_common_suffix_table[0], 2);
    ASSERT_EQ(weak_common_suffix_table[1], 1);
    ASSERT_EQ(weak_common_suffix_table[2], 2);
    ASSERT_EQ(weak_common_suffix_table[3], 1);
    ASSERT_EQ(weak_common_suffix_table[4], 0);
    ASSERT_EQ(weak_common_suffix_table[5], 0);
    ASSERT_EQ(weak_common_suffix_table[6], 0);

    bm = BoyerMoore{"abbabab"};
    weak_common_suffix_table = bm.get_weak_common_suffix_table();

    ASSERT_EQ(weak_common_suffix_table.size(), 7);
    ASSERT_EQ(weak_common_suffix_table[0], 2);
    ASSERT_EQ(weak_common_suffix_table[1], 1);
    ASSERT_EQ(weak_common_suffix_table[2], 3);
    ASSERT_EQ(weak_common_suffix_table[3], 2);
    ASSERT_EQ(weak_common_suffix_table[4], 1);
    ASSERT_EQ(weak_common_suffix_table[5], 0);
    ASSERT_EQ(weak_common_suffix_table[6], 0);
}

TEST(boyermoore, bad_character_table)
{
    BoyerMoore bm{"ababdab"};
    std::vector<int> bad_character_table = bm.get_bad_character_table();

    ASSERT_EQ(bad_character_table[static_cast<uchar>('a')], 5);
    ASSERT_EQ(bad_character_table[static_cast<uchar>('b')], 6);
    ASSERT_EQ(bad_character_table[static_cast<uchar>('d')], 4);
}

TEST(boyermoore, weak_good_suffix_shift_table)
{
    BoyerMoore bm{"abbabab"};
    std::vector<int> weak_good_suffix_shift_table = bm.get_weak_good_suffix_shift_table();

    ASSERT_EQ(weak_good_suffix_shift_table.size(), 7);
    ASSERT_EQ(weak_good_suffix_shift_table[0], 5);
    ASSERT_EQ(weak_good_suffix_shift_table[1], 5);
    ASSERT_EQ(weak_good_suffix_shift_table[2], 5);
    ASSERT_EQ(weak_good_suffix_shift_table[3], 2);
    ASSERT_EQ(weak_good_suffix_shift_table[4], 2);
    ASSERT_EQ(weak_good_suffix_shift_table[5], 2);
    ASSERT_EQ(weak_good_suffix_shift_table[6], 1);
}
