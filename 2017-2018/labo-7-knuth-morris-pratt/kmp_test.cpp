#include "kmp.h"
#include "gtest/gtest.h"

TEST(kmp, )
{
    std::string pattern{"aba"};
    std::string::text{"ababa"};

    std::vector<int> table;
    std::queue<int> results = KnuthMorrisPratt{pattern, table};

    ASSERT_EQ(results.size(), 2);
    ASSERT_EQ(results.top(), 0);
    results.pop();
    ASSERT_EQ(results.top(), 2);
    results.pop();
}

int main()
{
    KnuthMorrisPratt kmp{"babac"};
    std::cout << kmp.to_string() << std::endl;

    std::string woord;
    std::cout << "> ";
    while (std::cin >> woord)
    {
        kmp = KnuthMorrisPratt{woord};
        std::cout << kmp.to_string() << std::endl << "> ";
    }

    std::cout << "Done..." << std::endl;

    return 0;
}
