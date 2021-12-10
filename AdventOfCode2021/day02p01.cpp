/*
Now, you need to figure out how to pilot this thing.

It seems like the submarine can take a series of commands like forward 1, down 2, or up 3:

forward X increases the horizontal position by X units.
down X increases the depth by X units.
up X decreases the depth by X units.
Note that since you're on a submarine, down and up affect your depth, and so they have the opposite result of what you might expect.

The submarine seems to already have a planned course (your puzzle input). You should probably figure out where it's going. For example:

forward 5
down 5
forward 8
up 3
down 8
forward 2
Your horizontal position and depth both start at 0. The steps above would then modify them as follows:

forward 5 adds 5 to your horizontal position, a total of 5.
down 5 adds 5 to your depth, resulting in a value of 5.
forward 8 adds 8 to your horizontal position, a total of 13.
up 3 decreases your depth by 3, resulting in a value of 2.
down 8 adds 8 to your depth, resulting in a value of 10.
forward 2 adds 2 to your horizontal position, a total of 15.
After following these instructions, you would have a horizontal position of 15 and a depth of 10. (Multiplying these together produces 150.)

Calculate the horizontal position and depth you would have after following the planned course. What do you get if you multiply your final horizontal position by your final depth?
*/
#include "adventOfCode.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

#define MAX_LINE_LEN (100)

void day02Part01() {

	std::ifstream inputFile;
	std::string line;
	std::string dir;
	int value = 0;
	int prevSum = 0;
	int x = 0;
	int y = 0;

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	inputFile.open(inputDirectory + "/day02Input.txt");

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	while (!inputFile.eof()) {

		std::getline(inputFile, line);

		std::stringstream stream(line);
		std::getline(stream, dir, ' ');

		stream >> value;

		//std::cout << "*" << dir << "*" << " " << value << std::endl;

		if (dir == "forward") {
			x += value;
		}
		else if (dir == "up") {
			y -= value;
		}
		else if (dir == "down") {
			y += value;
		}

		//std::cin.get();
	}

	std::cout << "Horizontal: " << x << std::endl;
	std::cout << "Depth: " << y << std::endl;
	std::cout << "product: " << y * x << std::endl;
}
