#include "2021.h"

#include <fstream>
#include <iostream>
#include <filesystem>


#define MAX_LINE_LEN (100)

void day01Part02() {
 
	std::ifstream inputFile;
	char str[MAX_LINE_LEN];
	int nextSum = 0;
	int prevSum = 0;
	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	int n4 = 0;

	int count = 0; 

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	inputFile.open(inputDirectory + "/day01Input.txt");

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	inputFile.getline(str, MAX_LINE_LEN);
	n1 = std::strtol(str, nullptr, 10);

	inputFile.getline(str, MAX_LINE_LEN);
	n2 = std::strtol(str, nullptr, 10);

	inputFile.getline(str, MAX_LINE_LEN);
	n3 = std::strtol(str, nullptr, 10);

	prevSum = n3 + n2 + n1;

	while (inputFile.getline(str, MAX_LINE_LEN)) {
		
		n4 = std::strtol(str, nullptr, 10);
		nextSum = n4 + n3 + n2;
		if (nextSum > prevSum) {
			count++;
		}

		n2 = n3;
		n3 = n4;
		prevSum = nextSum;
	}

	std::cout << "Num increases: " << count << std::endl;
}