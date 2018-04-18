#include "zoekda.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <utility>
#include <vector>

TEST(zoekda, grep_bestand)
{
    Regexp regexp{"(a|b)*a(a|b)"};
    std::string bestandsnaam{"test_input.txt"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};
    ZoekDA da_zoeker{na_zoeker};

    std::vector<std::pair<int, std::string>> verwachte_waarden = {
            std::make_pair(6, "There were white out conditions in the town; subsequently, the roads were impassable."),
            std::make_pair(23, "Abstraction is often one floor above you."),
            std::make_pair(61, "The book is in front of the table."),
            std::make_pair(77, "Wednesday is hump day, but has anyone asked the camel if he's happy about it?"),
            std::make_pair(
                    82,
                    "If the Easter Bunny and the Tooth Fairy had babies would they take your teeth and leave chocolate for you?")};

    std::vector<std::pair<int, std::string>> resultaten = da_zoeker.grep_bestand(bestandsnaam);

    ASSERT_THAT(resultaten, ::testing::ContainerEq(verwachte_waarden));
}

TEST(zoekda, genereer_grafiek_voorbeeld1)
{
    Regexp regexp{"a(b|c)*"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};
    ZoekDA da_zoeker{na_zoeker};

    std::vector<std::map<uchar, int>> verwachte_overgangstabel = da_zoeker.geef_overgangstabel();
    ASSERT_EQ(verwachte_overgangstabel.size(), 4);
    ASSERT_EQ(verwachte_overgangstabel[0]['a'], 1);
    ASSERT_EQ(verwachte_overgangstabel[1]['b'], 2);
    ASSERT_EQ(verwachte_overgangstabel[1]['c'], 3);
    ASSERT_EQ(verwachte_overgangstabel[2]['b'], 2);
    ASSERT_EQ(verwachte_overgangstabel[2]['c'], 3);
    ASSERT_EQ(verwachte_overgangstabel[3]['b'], 2);
    ASSERT_EQ(verwachte_overgangstabel[3]['c'], 3);
}

TEST(zoekda, genereer_grafiek_voorbeeld2)
{
    Regexp regexp{"(a|b)a*"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};
    ZoekDA da_zoeker{na_zoeker};

    std::vector<std::map<uchar, int>> verwachte_overgangstabel = da_zoeker.geef_overgangstabel();
    ASSERT_EQ(verwachte_overgangstabel.size(), 4);
    ASSERT_EQ(verwachte_overgangstabel[0]['a'], 1);
    ASSERT_EQ(verwachte_overgangstabel[0]['b'], 2);
    ASSERT_EQ(verwachte_overgangstabel[1]['a'], 3);
    ASSERT_EQ(verwachte_overgangstabel[2]['a'], 3);
    ASSERT_EQ(verwachte_overgangstabel[3]['a'], 3);
}

TEST(zoekda, genereer_grafiek_voorbeeld3)
{
    Regexp regexp{"((ab)|c)*"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};
    ZoekDA da_zoeker{na_zoeker};

    std::vector<std::map<uchar, int>> verwachte_overgangstabel = da_zoeker.geef_overgangstabel();
    ASSERT_EQ(verwachte_overgangstabel.size(), 4);
    ASSERT_EQ(verwachte_overgangstabel[0]['a'], 1);
    ASSERT_EQ(verwachte_overgangstabel[0]['c'], 2);
    ASSERT_EQ(verwachte_overgangstabel[1]['b'], 3);
    ASSERT_EQ(verwachte_overgangstabel[2]['a'], 1);
    ASSERT_EQ(verwachte_overgangstabel[2]['c'], 2);
    ASSERT_EQ(verwachte_overgangstabel[3]['a'], 1);
    ASSERT_EQ(verwachte_overgangstabel[3]['c'], 2);
}
