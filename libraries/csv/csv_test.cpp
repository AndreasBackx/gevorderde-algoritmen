#include "csv.h"
#include "gtest/gtest.h"

#include <sstream>
#include <string>

TEST(csv_test, genereer_bestand_enkel_doubles)
{
    const std::string filename{"test.csv"};

    CsvData csv(filename);

    csv.voeg_data_toe(std::vector<double>{3, 1.2, 123456.789, 100, 3.14});
    csv.voeg_data_toe(std::vector<double>{8.6, 5, 10.9});
    csv.voeg_data_toe(std::vector<double>{6.4, 8.3, 5.7, 7.1, 8.6, 9.9, 4.5, 1, 1.2, 2.0, 5.4});

    std::string expected_content{
            "3.000000e+00\t8.600000e+00\t6.400000e+00\n"
            "1.200000e+00\t5.000000e+00\t8.300000e+00\n"
            "1.234568e+05\t1.090000e+01\t5.700000e+00\n"
            "1.000000e+02\t\t7.100000e+00\n"
            "3.140000e+00\t\t8.600000e+00\n"
            "\t\t9.900000e+00\n"
            "\t\t4.500000e+00\n"
            "\t\t1.000000e+00\n"
            "\t\t1.200000e+00\n"
            "\t\t2.000000e+00\n"
            "\t\t5.400000e+00\n"};

    EXPECT_EQ(csv.to_string(), expected_content);
}

TEST(csv_test, genereer_bestand)
{
    const std::string filename{"test.csv"};

    CsvData csv(filename);

    csv.voeg_data_toe(std::vector<int>{-1, 2, -3, 4, -5});
    csv.voeg_data_toe(std::vector<float>{1.1, 2.22, 3.333, 4.4444, 5.55555});
    csv.voeg_data_toe(std::vector<unsigned int>{1, 2, 3, 4, 5});

    std::string expected_content{
            "-1.000000e+00\t1.100000e+00\t1.000000e+00\n"
            "2.000000e+00\t2.220000e+00\t2.000000e+00\n"
            "-3.000000e+00\t3.333000e+00\t3.000000e+00\n"
            "4.000000e+00\t4.444400e+00\t4.000000e+00\n"
            "-5.000000e+00\t5.555550e+00\t5.000000e+00\n"};

    EXPECT_EQ(csv.to_string(), expected_content);
}

TEST(csv_test, geef_bestandsnaam)
{
    CsvData csv("abc.csv");
    EXPECT_EQ(csv.geef_bestandsnaam(), "abc.csv");

    csv = CsvData("abc");
    EXPECT_EQ(csv.geef_bestandsnaam(), "abc.csv");

    EXPECT_THROW(CsvData(".csv"), const char*);
    EXPECT_THROW(CsvData(""), const char*);
}
