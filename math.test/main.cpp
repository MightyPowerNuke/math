#include <chrono>
#include <iostream>

#include "../nuketest/nuketest/use_nuketest.h"

#include "matrix_test.h"
#include "primitives_test.h"
#include "transform_test.h"

int main(int, char* [])
{
	using namespace std::literals::chrono_literals;

	nukeTest::Options options;
	options
		.runModulesSequentially()
		.runTestsSequentially()
		.warnIfTestExceedsTime(500ms)
		.failTestIfExceedsTime(5s);

	int result = nukeTest::runAllTests(options);

	std::string dummy;
	std::cin >> dummy;

	return result;
}