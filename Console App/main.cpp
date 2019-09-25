#ifndef _MAIN_CCX
#define _MAIN_CCX

/**** STANDARD ****/
#include <iostream>
#include <ctime>

#include <mc_tests.h>

using namespace mc::tests;

#endif // !_MAIN_CCX

int main(int ac, char **av)
{
	//launchTests(ALL_TEST, ALL_LIBS);

	std::cerr << (3 & 4) << std::endl;

	std::getchar();

	return 0;
}