#include "extentions/cpp/HashMap.hpp"

// #include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "tests/test_common.h"

// clang-format off
TEST(keys, ({
    HashMap m(33);
    int data[5];
    const char* kys[] = { "one", "two", "three", "four", "five" };

    for (int i = 0; i < 5; i++)
    {
        data[i] = i;
        m.put(kys[i], &data[i]);
    }

    std::vector<std::string> vkeys = m.keys();
    for (int i = 0; i < 5; i++)
        assert(vkeys[i].compare(kys[i]));

	assert_eq(5, m.item_count());
    char* skeys[5];
    m.keys(skeys);
    int i, k;
    ASSERT_STR_ARR_EQ(skeys, kys, 5);
}))

TEST(test, ({
    HashMap m(32);
    int one = 1;
    m.put("one", &one);
    int res = *(int*)m.get("one");
    assert(res == one);
}))

// clang-format off
RUN_TEST_SUITE (
	ADD_TEST(keys),
	ADD_TEST(test)
)