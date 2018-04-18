#include "zoekna.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <utility>
#include <vector>

TEST(zoekna, grep_bestand)
{
    Regexp regexp{"(a|b)*a(a|b)"};
    std::string bestandsnaam{"test_input.txt"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};

    std::vector<std::pair<int, std::string>> verwachte_waarden = {
            std::make_pair(6, "There were white out conditions in the town; subsequently, the roads were impassable."),
            std::make_pair(23, "Abstraction is often one floor above you."),
            std::make_pair(61, "The book is in front of the table."),
            std::make_pair(77, "Wednesday is hump day, but has anyone asked the camel if he's happy about it?"),
            std::make_pair(
                    82,
                    "If the Easter Bunny and the Tooth Fairy had babies would they take your teeth and leave chocolate for you?")};

    std::vector<std::pair<int, std::string>> resultaten = na_zoeker.grep_bestand(bestandsnaam);

    ASSERT_THAT(resultaten, ::testing::ContainerEq(verwachte_waarden));
}

TEST(zoekna, genereer_grafiek_voorbeeld1)
{
    Regexp regexp{"a(b|c)*"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};

    std::vector<std::multimap<uchar, int>> verwachte_overgangstabel(9);
    verwachte_overgangstabel[0].insert(std::make_pair('a', 1));
    verwachte_overgangstabel[1].insert(std::make_pair(epsilon, 2));
    verwachte_overgangstabel[1].insert(std::make_pair(epsilon, 8));
    verwachte_overgangstabel[2].insert(std::make_pair(epsilon, 3));
    verwachte_overgangstabel[2].insert(std::make_pair(epsilon, 5));
    verwachte_overgangstabel[3].insert(std::make_pair('b', 4));
    verwachte_overgangstabel[4].insert(std::make_pair(epsilon, 7));
    verwachte_overgangstabel[5].insert(std::make_pair('c', 6));
    verwachte_overgangstabel[6].insert(std::make_pair(epsilon, 7));
    verwachte_overgangstabel[7].insert(std::make_pair(epsilon, 8));
    verwachte_overgangstabel[7].insert(std::make_pair(epsilon, 2));

    ASSERT_THAT(na_zoeker.geef_overgangstabel(), ::testing::ContainerEq(verwachte_overgangstabel));
}

TEST(zoekna, genereer_grafiek_voorbeeld2)
{
    Regexp regexp{"(a|b)a*"};
    ThompsonNA na_thompson{regexp};
    ZoekNA na_zoeker{na_thompson};

    std::vector<std::multimap<uchar, int>> verwachte_overgangstabel(9);
    verwachte_overgangstabel[0].insert(std::make_pair(epsilon, 1));
    verwachte_overgangstabel[0].insert(std::make_pair(epsilon, 3));
    verwachte_overgangstabel[1].insert(std::make_pair('a', 2));
    verwachte_overgangstabel[2].insert(std::make_pair(epsilon, 5));
    verwachte_overgangstabel[3].insert(std::make_pair('b', 4));
    verwachte_overgangstabel[4].insert(std::make_pair(epsilon, 5));
    verwachte_overgangstabel[5].insert(std::make_pair(epsilon, 6));
    verwachte_overgangstabel[5].insert(std::make_pair(epsilon, 8));
    verwachte_overgangstabel[6].insert(std::make_pair('a', 7));
    verwachte_overgangstabel[7].insert(std::make_pair(epsilon, 8));
    verwachte_overgangstabel[7].insert(std::make_pair(epsilon, 6));

    ASSERT_THAT(na_zoeker.geef_overgangstabel(), ::testing::ContainerEq(verwachte_overgangstabel));
}
