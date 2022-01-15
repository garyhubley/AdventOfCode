/*
With your submarine's subterranean subsystems subsisting suboptimally, the only way you're getting out of this cave anytime soon is by finding a path yourself. Not just a path - the only way to know if you've found the best path is to find all of them.

Fortunately, the sensors are still mostly working, and so you build a rough map of the remaining caves (your puzzle input). For example:

start-A
start-b
A-c
A-b
b-d
A-end
b-end
This is a list of how all of the caves are connected. You start in the cave named start, and your destination is the cave named end. An entry like b-d means that cave b is connected to cave d - that is, you can move between them.

So, the above cave system looks roughly like this:

	start
	/   \
c--A-----b--d
	\   /
	 end
Your goal is to find the number of distinct paths that start at start, end at end, and don't visit small caves more than once. There are two types of caves: big caves (written in uppercase, like A) and small caves (written in lowercase, like b). It would be a waste of time to visit any small cave more than once, but big caves are large enough that it might be worth visiting them multiple times. So, all paths you find should visit small caves at most once, and can visit big caves any number of times.

Given these rules, there are 10 paths through this example cave system:

start,A,b,A,c,A,end
start,A,b,A,end
start,A,b,end
start,A,c,A,b,A,end
start,A,c,A,b,end
start,A,c,A,end
start,A,end
start,b,A,c,A,end
start,b,A,end
start,b,end
(Each line in the above list corresponds to a single path; the caves visited by that path are listed in the order they are visited and separated by commas.)

Note that in this cave system, cave d is never visited by any path: to do so, cave b would need to be visited twice (once on the way to cave d and a second time when returning from cave d), and since cave b is small, this is not allowed.

Here is a slightly larger example:

dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc
The 19 paths through it are as follows:

start,HN,dc,HN,end
start,HN,dc,HN,kj,HN,end
start,HN,dc,end
start,HN,dc,kj,HN,end
start,HN,end
start,HN,kj,HN,dc,HN,end
start,HN,kj,HN,dc,end
start,HN,kj,HN,end
start,HN,kj,dc,HN,end
start,HN,kj,dc,end
start,dc,HN,end
start,dc,HN,kj,HN,end
start,dc,end
start,dc,kj,HN,end
start,kj,HN,dc,HN,end
start,kj,HN,dc,end
start,kj,HN,end
start,kj,dc,HN,end
start,kj,dc,end
Finally, this even larger example has 226 paths through it:

fs-end
he-DX
fs-he
start-DX
pj-DX
end-zg
zg-sl
zg-pj
pj-he
RW-he
fs-DX
pj-RW
zg-RW
start-pj
he-WI
zg-he
pj-fs
start-RW
How many paths through this cave system are there that visit small caves at most once?
*/

#include "adventOfCode.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

static std::vector<std::string> debugInput{
	"start-A",
	"start-b",
	"A-c",
	"A-b",
	"b-d",
	"A-end",
	"b-end"
};

enum CAVE_TYPE {
	SMALL,
	LARGE
};

struct Cave {
	Cave(const std::string& name) :_name(name) {
		_type = (isupper(name[0])) ? CAVE_TYPE::LARGE : CAVE_TYPE::SMALL;
		_connectedNodes = {};
	}
	std::string _name;
	CAVE_TYPE _type;
	std::vector<int> _connectedNodes;
};

void PrintMap(std::ostream& out, const std::vector<Cave>& map) {
	out << "Map: " << std::endl;
	for (int i = 0; i < map.size(); i++) {
		out << "  " << map[i]._name << ": ";
		for (auto j : map[i]._connectedNodes) {
			out << map[j]._name << ",";
		}
		out << std::endl;
	}
}
int ContainsCave(std::vector<Cave>& map, const Cave& cave) {
	const std::string& name = cave._name;
	for (int i = 0; i < map.size(); ++i) {
		if (map[i]._name == name) {
			return i;
		}
	}
	return -1;
}


int AddCave(std::vector<Cave>& map, const Cave& cave) {
	int idx = ContainsCave(map, cave);

	if (idx != -1) {
		return idx;
	}

	map.push_back(cave);
	return map.size() - 1;
}
void AddLink(std::vector<Cave>& map, int firstIdx, int secondIdx) {
	if (map[firstIdx]._name != "start" && map[secondIdx]._name != "end") {
		map[secondIdx]._connectedNodes.push_back(firstIdx);
	}

	map[firstIdx]._connectedNodes.push_back(secondIdx);
	return;
}

static std::string filename = "/day12Input.txt";

void day12Part01() {

	std::ifstream inputFile;

	std::string filepath = (inputDirectory + filename);
	inputFile.open(filepath);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file: " << filepath << std::endl;
		return;
	}

	std::vector<Cave> map;

	for (auto line : debugInput) {
		auto dashIdx = line.find("-");
		Cave first = Cave(line.substr(0, dashIdx));
		Cave second = Cave(line.substr(dashIdx + 1));
		int firstIdx = AddCave(map, first);
		int secondIdx = AddCave(map, second);
		AddLink(map, firstIdx, secondIdx);
	}

	std::cout << "size: " << map.size() << std::endl;
	PrintMap(std::cout, map);

	/*
	while (!inputFile.eof()) {
		char ch;
		inputFile >> ch;
	}*/

	inputFile.close();

	std::cout << "  Answer: " << std::endl;
}

/*
*/
void day12Part02() {

	std::ifstream inputFile;

	std::string filepath = (inputDirectory + filename);
	inputFile.open(filepath);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file: " << filepath << std::endl;
		return;
	}

	while (!inputFile.eof()) {
		char ch;
		inputFile >> ch;
	}

	inputFile.close();

	std::cout << "  Answer: " << std::endl;
}

