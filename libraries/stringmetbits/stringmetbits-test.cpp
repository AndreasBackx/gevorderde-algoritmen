
#include "gtest/gtest.h"

#include "stringmetbits.h"

#include <bitset>
#include <string>

class StringMetBitsTest : public ::testing::Test
{
protected:
    // void SetUp() override {}
    // void TearDown() override {}

    StringMetBits smb_carray{"char array"};
    StringMetBits smb_string{std::string{"string"}};
};

TEST_F(StringMetBitsTest, geefAantalBits)
{
    ASSERT_EQ(smb_carray.geefAantalBits(), 80);
    ASSERT_EQ(smb_string.geefAantalBits(), 48);
}

TEST_F(StringMetBitsTest, bit)
{
    // https://stackoverflow.com/questions/4975037/why-are-the-bits-of-a-stdbitset-in-reverse-order
    std::bitset<80> smb_carray_bits{"01100011011010000110000101110010001000000110000101110010011100100110000101111001"};
    std::bitset<48> smb_string_bits{"011100110111010001110010011010010110111001100111"};

    for (int i = 0; i < smb_carray.geefAantalBits(); i++)
    {
        int bitset_index = smb_carray.geefAantalBits() - i - 1;
        ASSERT_EQ(smb_carray.bit(i), smb_carray_bits.test(bitset_index)) << " on index " << i;
    }

    for (int i = 0; i < smb_string.geefAantalBits(); i++)
    {
        int bitset_index = smb_string.geefAantalBits() - i - 1;
        ASSERT_EQ(smb_string.bit(i), smb_string_bits.test(bitset_index)) << " on index " << i;
    }
}

TEST_F(StringMetBitsTest, verschilplaats)
{
    ASSERT_EQ(smb_carray.verschilplaats(smb_string), 3);
    ASSERT_EQ(smb_carray.verschilplaats(smb_string), smb_string.verschilplaats(smb_carray));

    StringMetBits kort{"abc"};
    StringMetBits lang{"abcdefgh"};

    ASSERT_EQ(kort.verschilplaats(lang), 24);
    ASSERT_EQ(kort.verschilplaats(lang), lang.verschilplaats(kort));

    ASSERT_EQ(smb_carray.verschilplaats(StringMetBits{"char array"}), -1);
}

TEST_F(StringMetBitsTest, check0)
{
    ASSERT_NO_THROW(smb_carray.check0());
    ASSERT_NO_THROW(smb_string.check0());

    std::string test_string{"test\0test", 9};
    ASSERT_THROW(StringMetBits{test_string}, const char*);
}
