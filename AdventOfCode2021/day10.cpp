/*
*/

#include "adventOfCode.h"

#include <iostream>
#include <fstream>
#include <vector>

static std::string filename = "/day10Input.txt";

void day10Part01() {

	std::ifstream inputFile;

	std::string filepath = (inputDirectory + filename);
	inputFile.open(filepath);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file: " << filepath << std::endl;
		return;
	}

	while (!inputFile.eof()) {
		std::string line;
		inputFile >> line;
		if (!line.empty()) {
		}
	}

	inputFile.close();

	std::cout << "  Answer: " << std::endl;
}

/*
*/
void day10Part02() {

	std::ifstream inputFile;

	std::string filepath = (inputDirectory + filename);
	inputFile.open(filepath);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file: " << filepath << std::endl;
		return;
	}

	while (!inputFile.eof()) {
		std::string line;
		inputFile >> line;
		if (!line.empty()) {
		}
	}

	inputFile.close();

	std::cout << "  Answer: " << std::endl;
}
