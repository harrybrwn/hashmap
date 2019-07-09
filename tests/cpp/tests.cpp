#include "extentions/cpp/HashMap.hpp"

#include <iostream>
#include <assert.h>

void test()
{
	HashMap m(32);
	int one = 1;
	m.put("one", &one);
	int res = *(int*)m.get("one");
	assert(res == one);
}

int main()
{
	test();
	std::cout << "OK " << __FILE__ << std::endl;
}