#include <string>

#include "gtest/gtest.h"
#include "knoop.h"

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
