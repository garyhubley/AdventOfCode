/*
The sea floor is getting steeper. Maybe the sleigh keys got carried this way?

A massive school of glowing lanternfish swims past. They must spawn quickly to reach such large
numbers - maybe exponentially quickly? You should model their growth rate to be sure.

Although you know nothing about this specific species of lanternfish, you make some guesses about
their attributes. Surely, each lanternfish creates a new lanternfish once every 7 days.

However, this process isn't necessarily synchronized between every lanternfish - one lanternfish
might have 2 days left until it creates another lanternfish, while another might have 4. So, you
can model each fish as a single number that represents the number of days until it creates a new
lanternfish.

Furthermore, you reason, a new lanternfish would surely need slightly longer before it's capable
of producing more lanternfish: two more days for its first cycle.

So, suppose you have a lanternfish with an internal timer value of 3:

After one day, its internal timer would become 2.
After another day, its internal timer would become 1.
After another day, its internal timer would become 0.
After another day, its internal timer would reset to 6, and it would create a new lanternfish
with an internal timer of 8.
After another day, the first lanternfish would have an internal timer of 5, and the second
lanternfish would have an internal timer of 7.
A lanternfish that creates a new fish resets its timer to 6, not 7 (because 0 is included as a
valid timer value). The new lanternfish starts with an internal timer of 8 and does not start
counting down until the next day.

Realizing what you're trying to do, the submarine automatically produces a list of the ages of
several hundred nearby lanternfish (your puzzle input). For example, suppose you were given the
following list:

3,4,3,1,2
This list means that the first fish has an internal timer of 3, the second fish has an internal
timer of 4, and so on until the fifth fish, which has an internal timer of 2. Simulating these
fish over several days would proceed as follows:

Initial state: 3,4,3,1,2
After  1 day:  2,3,2,0,1
After  2 days: 1,2,1,6,0,8
After  3 days: 0,1,0,5,6,7,8
After  4 days: 6,0,6,4,5,6,7,8,8
After  5 days: 5,6,5,3,4,5,6,7,7,8
After  6 days: 4,5,4,2,3,4,5,6,6,7
After  7 days: 3,4,3,1,2,3,4,5,5,6
After  8 days: 2,3,2,0,1,2,3,4,4,5
After  9 days: 1,2,1,6,0,1,2,3,3,4,8
After 10 days: 0,1,0,5,6,0,1,2,2,3,7,8
After 11 days: 6,0,6,4,5,6,0,1,1,2,6,7,8,8,8
After 12 days: 5,6,5,3,4,5,6,0,0,1,5,6,7,7,7,8,8
After 13 days: 4,5,4,2,3,4,5,6,6,0,4,5,6,6,6,7,7,8,8
After 14 days: 3,4,3,1,2,3,4,5,5,6,3,4,5,5,5,6,6,7,7,8
After 15 days: 2,3,2,0,1,2,3,4,4,5,2,3,4,4,4,5,5,6,6,7
After 16 days: 1,2,1,6,0,1,2,3,3,4,1,2,3,3,3,4,4,5,5,6,8
After 17 days: 0,1,0,5,6,0,1,2,2,3,0,1,2,2,2,3,3,4,4,5,7,8
After 18 days: 6,0,6,4,5,6,0,1,1,2,6,0,1,1,1,2,2,3,3,4,6,7,8,8,8,8
Each day, a 0 becomes a 6 and adds a new 8 to the end of the list, while each other number
decreases by 1 if it was present at the start of the day.

In this example, after 18 days, there are a total of 26 fish. After 80 days, there would be a
total of 5934.

Find a way to simulate lanternfish. How many lanternfish would there be after 80 days?
*/

#include "adventOfCode.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

static std::string filename = "/day06Input.txt";

std::vector<int> debugFish = { 3,4,3,1,2 };

void day06Part01() {

	std::ifstream inputFile;
	std::string str;
	std::vector<int> fish;
	int days = 80;

	inputFile.open(inputDirectory + filename);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	// get first line, 
	while (!inputFile.eof()) {
		int tmp;
		char eater;
		inputFile >> tmp >> eater;
		fish.push_back(tmp);
	}

	inputFile.close();

	fish = debugFish;

	for (int i = 0; i < days; i++) {
		std::vector<int> newFish;
		for (auto& timer : fish) {
			if (--timer == -1) {
				timer = 6;
				newFish.push_back(8);
			}
		}

		fish.insert(
			fish.end(),
			std::make_move_iterator(newFish.begin()),
			std::make_move_iterator(newFish.end())
		);

	}

	std::cout << "  Answer: " << fish.size() << std::endl;
}

/*
Suppose the lanternfish live forever and have unlimited food and space. Would they take over the 
entire ocean?

After 256 days in the example above, there would be a total of 26984457539 lanternfish!

How many lanternfish would there be after 256 days?
*/
void day06Part02() {

	std::ifstream inputFile;
	long long sum = 0;
	int days = 256;
	std::vector<long long> bucketsOfFish(10);

	inputFile.open(inputDirectory + filename);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	// get first line, 
	while (!inputFile.eof()) {
		int tmp;
		char eater;
		inputFile >> tmp >> eater;
		bucketsOfFish[tmp]++;
	}

	inputFile.close();

	for (int i = 0; i < days; i++) {
		bucketsOfFish[9] = bucketsOfFish[0]; // save 0
		bucketsOfFish[0] = bucketsOfFish[1];
		bucketsOfFish[1] = bucketsOfFish[2];
		bucketsOfFish[2] = bucketsOfFish[3];
		bucketsOfFish[3] = bucketsOfFish[4];
		bucketsOfFish[4] = bucketsOfFish[5];
		bucketsOfFish[5] = bucketsOfFish[6];
		bucketsOfFish[6] = bucketsOfFish[7];
		bucketsOfFish[7] = bucketsOfFish[8];
		bucketsOfFish[8] = bucketsOfFish[9]; // new fish

		bucketsOfFish[6] += bucketsOfFish[9]; // timer reset fish
	}

	for (auto b : bucketsOfFish) {
		sum += b;
	}
	
	sum -= bucketsOfFish[9];
	std::cout << "  Answer: " << sum << std::endl;
}