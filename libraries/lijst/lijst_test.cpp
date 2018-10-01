
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "lijst.h"

using ::testing::ContainerEq;
using ::testing::ElementsAreArray;
using ::testing::IsEmpty;

TEST(lijst, append)
{
    Lijst<int> a{5};
    Lijst<int> b{10};

    a.append(std::move(b));

    ASSERT_THAT(a.get_all_elements(), ElementsAreArray({5, 10}));
    ASSERT_THAT(b, IsEmpty());

    Lijst<int> x{0};
    for (int i = 1; i <= 5; i++)
    {
        Lijst<int> y(i);
        x.append(std::move(y));
    }

    ASSERT_THAT(x.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5}));
}

TEST(lijst, append_empty_lists)
{
    Lijst<int> a;
    Lijst<int> b{10};

    a.append(std::move(b));

    ASSERT_THAT(a.get_all_elements(), ElementsAreArray({10}));
    ASSERT_THAT(b, IsEmpty());

    Lijst<int> c;
    Lijst<int> d;

    c.append(std::move(d));

    ASSERT_THAT(c, IsEmpty());
    ASSERT_THAT(d, IsEmpty());

    Lijst<int> e{3};
    Lijst<int> f;

    e.append(std::move(f));

    ASSERT_THAT(e.get_all_elements(), ElementsAreArray({3}));
    ASSERT_THAT(f, IsEmpty());
}

TEST(lijst, copy)
{
    Lijst<int> x{0};

    for (int i = 1; i <= 5; i++)
    {
        x.append(Lijst<int>{i});
    }

    Lijst<int> y{x};

    ASSERT_THAT(x.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5}));
    ASSERT_THAT(y.get_all_elements(), ContainerEq(x.get_all_elements()));

    Lijst<int> a{999};
    y.append(std::move(a));

    ASSERT_THAT(x.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5}));
    ASSERT_THAT(y.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5, 999}));

    a = x;

    ASSERT_THAT(x.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5}));
    ASSERT_THAT(y.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5, 999}));
    ASSERT_THAT(a.get_all_elements(), ContainerEq(x.get_all_elements()));

    a = a;
    ASSERT_THAT(a.get_all_elements(), ElementsAreArray({0, 1, 2, 3, 4, 5}));
}

TEST(lijst, size)
{
    Lijst<int> x;

    ASSERT_EQ(x.size(), 0);

    for (int i = 0; i < 5; i++)
    {
        x.append(Lijst<int>{i});
    }

    ASSERT_EQ(x.size(), 5);
}