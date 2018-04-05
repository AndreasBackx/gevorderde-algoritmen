#include "zoekna.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <utility>
#include <vector>

TEST(zoekna, klein_voorbeeld)
{
    Regexp regexp{"doel|cout"};
    std::string bestandsnaam{"thompsonna.h"};
    ThompsonNA nda{regexp};
    ZoekNA zoeker{nda};

    std::vector<std::pair<int, std::string>> verwachte_waarden = {
            std::make_pair(19, "    Verbinding(int bron, int doel, uchar karakter);"),
            std::make_pair(22, "    int doel;"),
            std::make_pair(40, "    void verbind(int bron, int doel, uchar c);"),
            std::make_pair(
                    46,
                    "Verbinding::Verbinding(int bron, int doel, uchar karakter) : bron{bron}, doel{doel}, karakter{karakter}"),
            std::make_pair(
                    70,
                    "        overgang.push_back(Verbinding(oud.bron + verschuiving, oud.doel + verschuiving, oud.karakter));"),
            std::make_pair(73, "void ThompsonNA::verbind(int bron, int doel, uchar c = epsilon)"),
            std::make_pair(75, "    overgang.push_back(Verbinding(bron, doel, c));"),
            std::make_pair(136, "        out << \"\\t \" << v.bron << \" -> \" << v.doel << \" [label=\\\"\";")};

    std::vector<std::pair<int, std::string>> resultaten = zoeker.grep_bestand(bestandsnaam);

    EXPECT_THAT(resultaten, ::testing::ContainerEq(verwachte_waarden));
}
