#include "extentions/cpp/HashMap.hpp"

#include "tests/test_common.h"
#include <iostream>
#include <cassert>

typedef void (*testfunc)();

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
};

int main()
{
	int n = sizeof(tests) / sizeof(testfunc);
	Run(tests, n);
}