
#include "adventOfCode.h"

#include <fstream>
#include <iostream>

#define MAX_LINE_LEN (100)

void day01Part01() {

	std::ifstream inputFile;
	char str[MAX_LINE_LEN];
	int next = 0;
	int prev = 0;
	int count = -1; // account for first value being greater than 0

	inputFile.open(inputDirectory + "/day01Input.txt");

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	while (inputFile.getline(str, MAX_LINE_LEN)) {
		next = std::strtol(str, nullptr, 10);
		if (next > prev) {
			count++;
		}
		prev = next;
	}

	std::cout << "Num increases: " << count << std::endl;
}