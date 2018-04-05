#include "stroomnetwerk.h"
#include "grootstecapaciteitenpadzoeker.h"
#include "gtest/gtest.h"
#include "kortstepadzoeker.h"
#include "langpadzoeker.h"

#include <memory>

TEST(stroomnetwerk, klein_voorbeeld_grootstecapaciteitenpadzoeker)
{
    const int verwachte_max_flow = 6;
    const int producent = 0;
    const int verbruiker = 3;

    GraafMetTakdata<GERICHT, int> klein_voorbeeld{4};
    klein_voorbeeld.voegVerbindingToe(0, 1, 4);
    klein_voorbeeld.voegVerbindingToe(0, 2, 2);
    klein_voorbeeld.voegVerbindingToe(1, 2, 6);
    klein_voorbeeld.voegVerbindingToe(1, 3, 2);
    klein_voorbeeld.voegVerbindingToe(2, 3, 4);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<GrootsteCapaciteitenPadZoeker<int>>();

    Stroomnetwerk<int> sn{klein_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, klein_voorbeeld_langpadzoeker)
{
    const int verwachte_max_flow = 6;
    const int producent = 0;
    const int verbruiker = 3;

    GraafMetTakdata<GERICHT, int> klein_voorbeeld{4};
    klein_voorbeeld.voegVerbindingToe(0, 1, 4);
    klein_voorbeeld.voegVerbindingToe(0, 2, 2);
    klein_voorbeeld.voegVerbindingToe(1, 2, 6);
    klein_voorbeeld.voegVerbindingToe(1, 3, 2);
    klein_voorbeeld.voegVerbindingToe(2, 3, 4);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<LangPadZoeker<int>>();

    Stroomnetwerk<int> sn{klein_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, klein_voorbeeld_kortstepadzoeker)
{
    const int verwachte_max_flow = 6;
    const int producent = 0;
    const int verbruiker = 3;

    GraafMetTakdata<GERICHT, int> klein_voorbeeld{4};
    klein_voorbeeld.voegVerbindingToe(0, 1, 4);
    klein_voorbeeld.voegVerbindingToe(0, 2, 2);
    klein_voorbeeld.voegVerbindingToe(1, 2, 6);
    klein_voorbeeld.voegVerbindingToe(1, 3, 2);
    klein_voorbeeld.voegVerbindingToe(2, 3, 4);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<KortstePadZoeker<int>>();

    Stroomnetwerk<int> sn{klein_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, labo_voorbeeld_grootstecapaciteitenpadzoeker)
{
    const int verwachte_max_flow = 13;
    const int producent = 0;
    const int verbruiker = 7;

    GraafMetTakdata<GERICHT, int> labo_voorbeeld{8};
    labo_voorbeeld.voegVerbindingToe(0, 1, 7);
    labo_voorbeeld.voegVerbindingToe(0, 2, 10);
    labo_voorbeeld.voegVerbindingToe(1, 4, 4);
    labo_voorbeeld.voegVerbindingToe(2, 1, 3);
    labo_voorbeeld.voegVerbindingToe(2, 3, 5);
    labo_voorbeeld.voegVerbindingToe(2, 5, 6);
    labo_voorbeeld.voegVerbindingToe(3, 0, 2);
    labo_voorbeeld.voegVerbindingToe(3, 6, 6);
    labo_voorbeeld.voegVerbindingToe(4, 5, 6);
    labo_voorbeeld.voegVerbindingToe(5, 7, 8);
    labo_voorbeeld.voegVerbindingToe(6, 5, 2);
    labo_voorbeeld.voegVerbindingToe(6, 7, 7);
    labo_voorbeeld.voegVerbindingToe(7, 4, 1);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<GrootsteCapaciteitenPadZoeker<int>>();

    Stroomnetwerk<int> sn{labo_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, labo_voorbeeld_langpadzoeker)
{
    const int verwachte_max_flow = 13;
    const int producent = 0;
    const int verbruiker = 7;

    GraafMetTakdata<GERICHT, int> labo_voorbeeld{8};
    labo_voorbeeld.voegVerbindingToe(0, 1, 7);
    labo_voorbeeld.voegVerbindingToe(0, 2, 10);
    labo_voorbeeld.voegVerbindingToe(1, 4, 4);
    labo_voorbeeld.voegVerbindingToe(2, 1, 3);
    labo_voorbeeld.voegVerbindingToe(2, 3, 5);
    labo_voorbeeld.voegVerbindingToe(2, 5, 6);
    labo_voorbeeld.voegVerbindingToe(3, 0, 2);
    labo_voorbeeld.voegVerbindingToe(3, 6, 6);
    labo_voorbeeld.voegVerbindingToe(4, 5, 6);
    labo_voorbeeld.voegVerbindingToe(5, 7, 8);
    labo_voorbeeld.voegVerbindingToe(6, 5, 2);
    labo_voorbeeld.voegVerbindingToe(6, 7, 7);
    labo_voorbeeld.voegVerbindingToe(7, 4, 1);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<LangPadZoeker<int>>();

    Stroomnetwerk<int> sn{labo_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, labo_voorbeeld_kortstepadzoeker)
{
    const int verwachte_max_flow = 13;
    const int producent = 0;
    const int verbruiker = 7;

    GraafMetTakdata<GERICHT, int> labo_voorbeeld{8};
    labo_voorbeeld.voegVerbindingToe(0, 1, 7);
    labo_voorbeeld.voegVerbindingToe(0, 2, 10);
    labo_voorbeeld.voegVerbindingToe(1, 4, 4);
    labo_voorbeeld.voegVerbindingToe(2, 1, 3);
    labo_voorbeeld.voegVerbindingToe(2, 3, 5);
    labo_voorbeeld.voegVerbindingToe(2, 5, 6);
    labo_voorbeeld.voegVerbindingToe(3, 0, 2);
    labo_voorbeeld.voegVerbindingToe(3, 6, 6);
    labo_voorbeeld.voegVerbindingToe(4, 5, 6);
    labo_voorbeeld.voegVerbindingToe(5, 7, 8);
    labo_voorbeeld.voegVerbindingToe(6, 5, 2);
    labo_voorbeeld.voegVerbindingToe(6, 7, 7);
    labo_voorbeeld.voegVerbindingToe(7, 4, 1);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<KortstePadZoeker<int>>();

    Stroomnetwerk<int> sn{labo_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, terugverbindingen_voorbeeld_grootstecapaciteitenpadzoeker)
{
    const int verwachte_max_flow = 2;
    const int producent = 0;
    const int verbruiker = 5;

    GraafMetTakdata<GERICHT, int> terugverbindingen_voorbeeld{6};
    terugverbindingen_voorbeeld.voegVerbindingToe(0, 1, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(0, 2, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(1, 3, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(2, 3, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(2, 4, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(3, 5, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(4, 5, 1);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<GrootsteCapaciteitenPadZoeker<int>>();

    Stroomnetwerk<int> sn{terugverbindingen_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, terugverbindingen_voorbeeld_langpadzoeker)
{
    const int verwachte_max_flow = 2;
    const int producent = 0;
    const int verbruiker = 5;

    GraafMetTakdata<GERICHT, int> terugverbindingen_voorbeeld{6};
    terugverbindingen_voorbeeld.voegVerbindingToe(0, 1, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(0, 2, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(1, 3, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(2, 3, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(2, 4, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(3, 5, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(4, 5, 1);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<LangPadZoeker<int>>();

    Stroomnetwerk<int> sn{terugverbindingen_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, terugverbindingen_voorbeeld_kortstepadzoeker)
{
    const int verwachte_max_flow = 2;
    const int producent = 0;
    const int verbruiker = 5;

    GraafMetTakdata<GERICHT, int> terugverbindingen_voorbeeld{6};
    terugverbindingen_voorbeeld.voegVerbindingToe(0, 1, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(0, 2, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(1, 3, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(2, 3, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(2, 4, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(3, 5, 1);
    terugverbindingen_voorbeeld.voegVerbindingToe(4, 5, 1);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<KortstePadZoeker<int>>();

    Stroomnetwerk<int> sn{terugverbindingen_voorbeeld, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, cnops_voorbeeld_grootstecapaciteitenpadzoeker)
{
    const int verwachte_max_flow = 10;
    const int producent = 0;
    const int verbruiker = 5;

    GraafMetTakdata<GERICHT, int> voorbeeld_cnops{6};
    voorbeeld_cnops.voegVerbindingToe(0, 1, 3);
    voorbeeld_cnops.voegVerbindingToe(0, 2, 7);
    voorbeeld_cnops.voegVerbindingToe(1, 3, 3);
    voorbeeld_cnops.voegVerbindingToe(1, 4, 7);
    voorbeeld_cnops.voegVerbindingToe(2, 3, 7);
    voorbeeld_cnops.voegVerbindingToe(3, 1, 5);
    voorbeeld_cnops.voegVerbindingToe(3, 5, 3);
    voorbeeld_cnops.voegVerbindingToe(4, 5, 7);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<GrootsteCapaciteitenPadZoeker<int>>();

    Stroomnetwerk<int> sn{voorbeeld_cnops, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, cnops_voorbeeld_langpadzoeker)
{
    const int verwachte_max_flow = 10;
    const int producent = 0;
    const int verbruiker = 5;

    GraafMetTakdata<GERICHT, int> voorbeeld_cnops{6};
    voorbeeld_cnops.voegVerbindingToe(0, 1, 3);
    voorbeeld_cnops.voegVerbindingToe(0, 2, 7);
    voorbeeld_cnops.voegVerbindingToe(1, 3, 3);
    voorbeeld_cnops.voegVerbindingToe(1, 4, 7);
    voorbeeld_cnops.voegVerbindingToe(2, 3, 7);
    voorbeeld_cnops.voegVerbindingToe(3, 1, 5);
    voorbeeld_cnops.voegVerbindingToe(3, 5, 3);
    voorbeeld_cnops.voegVerbindingToe(4, 5, 7);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<LangPadZoeker<int>>();

    Stroomnetwerk<int> sn{voorbeeld_cnops, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}

TEST(stroomnetwerk, cnops_voorbeeld_kortstepadzoeker)
{
    const int verwachte_max_flow = 10;
    const int producent = 0;
    const int verbruiker = 5;

    GraafMetTakdata<GERICHT, int> voorbeeld_cnops{6};
    voorbeeld_cnops.voegVerbindingToe(0, 1, 3);
    voorbeeld_cnops.voegVerbindingToe(0, 2, 7);
    voorbeeld_cnops.voegVerbindingToe(1, 3, 3);
    voorbeeld_cnops.voegVerbindingToe(1, 4, 7);
    voorbeeld_cnops.voegVerbindingToe(2, 3, 7);
    voorbeeld_cnops.voegVerbindingToe(3, 1, 5);
    voorbeeld_cnops.voegVerbindingToe(3, 5, 3);
    voorbeeld_cnops.voegVerbindingToe(4, 5, 7);

    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<KortstePadZoeker<int>>();

    Stroomnetwerk<int> sn{voorbeeld_cnops, producent, verbruiker, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    ASSERT_EQ(max_flow, verwachte_max_flow);
}
