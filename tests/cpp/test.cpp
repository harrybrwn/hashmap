#include "extentions/cpp/HashMap.hpp"

#include "tests/test_common.h"
#include <iostream>
#include <cassert>
#include <cstdio>

typedef void (*testfunc)();

void test_keys()
{
	// HashMap m(33);
	// int data[5];
	// std::string ks[] = {"one", "two", "three", "four", "five"};
	// for (int i = 0; i < 5; i++) {
	// 	data[i] = i;
	// 	m.put(ks[i], &data[i]);
	// }

	// std::vector<std::string> keys = m.keys();
	// for (int i = 0; i < 5; i++)
	// 	// std::cout << keys[i] << std::endl;
	// 	assert(keys[i].compare(ks[i].c_str()));
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
	// test_keys,
};

int main()
{
	int n = sizeof(tests) / sizeof(testfunc);
	Run(tests, n);
}