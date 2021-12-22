#include "adventOfCode.h"

#include <iostream>
#include <vector>
#include <sstream>

#ifndef __GNUC__
const std::string inputDirectory("./inputFiles");
#else
const std::string inputDirectory("../AdventOfCode2021/inputFiles");
#endif


static const std::vector<dayfunction*> functions = {
	day01Part01, day01Part02, day02Part01, day02Part02, day03Part01, day03Part02,
	day04Part01, day04Part02, day05Part01, day05Part02, day06Part01, day06Part02,
	day07Part01, day07Part02, day08Part01, day08Part02, day09Part01, day09Part02,
	day10Part01, day10Part02, day11Part01, day11Part02, day12Part01, day12Part02,
	day13Part01, day13Part02, day14Part01, day14Part02, day15Part01, day15Part02,
	day16Part01, day16Part02, day17Part01, day17Part02, day18Part01, day18Part02,
	day19Part01, day19Part02, day20Part01, day20Part02, day21Part01, day21Part02,
	day22Part01, day22Part02, day23Part01, day23Part02, day24Part01, day24Part02,
	day25Part01,
	day25Part02
};


int main(int argc, char* argv[]) {

	int day, part;
	char eater;
	const int INDEX_CORRECTION = 1;

	std::cout << "Welcome to Gary's Advent of Code 2021." << std::endl;
	std::cout << "Select a day (1-25) and part (1,2) in the following format:" << std::endl;
	std::cout << "  day, part" << std::endl;
	std::cout << "** if day = 0, you will exit" << std::endl;
	std::cout << "** if day = 26, you will run all days and all parts" << std::endl;

	std::cin >> day >> eater >> part;

	if (day > 26 || day < 0) {
		std::cout << "invalid day. Must be between 0 and 26." << std::endl;
		std::cout << "  day: " << day << std::endl;
		return -1;
	}

	if ((day != 0 && day != 26) && (part > 2 || part < 1)) {
		std::cout << "invalid part. Must be between 1 and 2." << std::endl;
		std::cout << "  part: " << part << std::endl;
		return -1;
	}

	day -= INDEX_CORRECTION;
	part -= INDEX_CORRECTION;

	switch (day)
	{
	case -1:
		break;
	case 25:
		for (auto func : functions) {
			func();
			std::cout << std::endl;
		}
		break;
	default:
		functions[day * 2 + part]();
		break;
	}

	return 0;
}
