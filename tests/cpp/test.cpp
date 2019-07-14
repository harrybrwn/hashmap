#include "extentions/cpp/HashMap.hpp"

#include "tests/test_common.h"
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdlib>

typedef void (*testfunc)();

void test_keys()
{
	HashMap m(33);
	int data[5];
	const char *kys[] = {"one", "two", "three", "four", "five"};
	
	for (int i = 0; i < 5; i++) {
		data[i] = i;
		m.put(kys[i], &data[i]);
	}
	
	std::vector<std::string> vkeys = m.keys();
	for (int i = 0; i < 5; i++)
		assert(vkeys[i].compare(kys[i]));

	char* skeys[m.item_count()];
	m.keys(skeys);
	int i, k;
	ASSERT_STR_ARR_EQ(skeys, kys, 5);
}

void test()
{
	HashMap m(32);
	int one = 1;
	m.put("one", &one);
	int res = *(int*)m.get("one");
	assert(res == one);
}

static testfunc tests[] = {
	test,
	test_keys,
};

int main()
{
	test_keys();
	int n = sizeof(tests) / sizeof(testfunc);
	Run(tests, n);
}