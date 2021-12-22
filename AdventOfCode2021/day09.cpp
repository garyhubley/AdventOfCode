/*
These caves seem to be lava tubes. Parts are even still volcanically active; small hydrothermal
vents release smoke into the caves that slowly settles like rain.

If you can model how the smoke flows through the caves, you might be able to avoid it and be that
much safer. The submarine generates a heightmap of the floor of the nearby caves for you (your
puzzle input).

Smoke flows to the lowest point of the area it's in. For example, consider the following heightmap:

2199943210
3987894921
9856789892
8767896789
9899965678
Each number corresponds to the height of a particular location, where 9 is the highest and 0 is
the lowest a location can be.

Your first goal is to find the low points - the locations that are lower than any of its adjacent
locations. Most locations have four adjacent locations (up, down, left, and right); locations on
the edge or corner of the map have three or two adjacent locations, respectively. (Diagonal
locations do not count as adjacent.)

In the above example, there are four low points, all highlighted: two are in the first row (a 1
and a 0), one is in the third row (a 5), and one is in the bottom row (also a 5). All other
locations on the heightmap have some lower adjacent location, and so are not low points.

The risk level of a low point is 1 plus its height. In the above example, the risk levels of the
low points are 2, 1, 6, and 6. The sum of the risk levels of all low points in the heightmap is
therefore 15.

Find all of the low points on your heightmap. What is the sum of the risk levels of all low
points on your heightmap?
*/

#include "adventOfCode.h"

#include <iostream>
#include <fstream>
#include <vector>

static std::string filename = "/day09Input.txt";

#define CENTER (0)
#define EAST   (1)
#define SOUTH  (2)
#define WEST   (3)
#define NORTH  (4)

struct Point {
	Point(int r = -1, int c = -1) : row(r), col(c) {}
	int row;
	int col;
	friend bool operator ==(const Point& p1, const Point& p2);
	friend std::ostream& operator <<(std::ostream &os, const Point& p);
};

std::ostream& operator <<(std::ostream &os, const Point& p) {
    os << "(" << p.row << ", " << p.col << ")";
    return os;
}

bool operator ==(const Point& p1, const Point& p2) {
	return (p1.row == p2.row && p1.col == p2.col);
}

typedef std::vector<std::string> HeightMap;
const static HeightMap debugInput = { "2199943210","3987894921","9856789892","8767896789","9899965678" };

inline Point calcEast(const Point& p0) { return Point(p0.row, p0.col + 1); }

inline Point calcSouth(const Point& p0) { return Point(p0.row + 1, p0.col); }

inline Point calcWest(const Point& p0) { return Point(p0.row, p0.col - 1); }

inline Point calcNorth(const Point& p0) { return Point(p0.row - 1, p0.col); }

int getValue(const Point& point, const HeightMap& heightMap) {
	return heightMap[point.row][point.col] - '0';
}

bool  isAtTop(const Point &p) { return p.row == 0; }
bool  isAtLeft(const Point &p) { return p.col == 0; }
bool  isAtBottom(const Point &p, int height) { return p.row == (height - 1); }
bool  isAtRight(const Point &p, int width) { return p.col == (width - 1); }

bool isLocalMin(const Point& p0, const HeightMap& heightMap, int &value) {

	auto width = heightMap[0].length();
	auto height = heightMap.size();

    std::vector<int> vals(5,10);
    vals[CENTER] = getValue(p0, heightMap);
    if(!isAtBottom(p0, height)) {
        // not on bottom, calc south
        vals[SOUTH] = getValue(calcSouth(p0), heightMap);
    }
    if(!isAtTop(p0)) {
        // not on top, calc north
        vals[NORTH] = getValue(calcNorth(p0), heightMap);
    }
    if(!isAtRight(p0, width)) {
        // not on right, calc east
        vals[EAST] = getValue(calcEast(p0), heightMap);
    }
    if(!isAtLeft(p0)) {
        // not on left, calc west
        vals[WEST] = getValue(calcWest(p0), heightMap);
    }

    if(vals[CENTER] < vals[NORTH] 
        && vals[CENTER] < vals[EAST] 
        && vals[CENTER] < vals[SOUTH] 
        && vals[CENTER] < vals[WEST] 
      )
    {
        value = vals[CENTER];
        return true;
    }
    return false;
}

bool alreadyFound(const Point& p, std::vector<Point>& points) {
	for (auto& found : points) {
		if (found == p) {
			return true;
		}
	}
	return false;
}

void day09Part01() {

	std::ifstream inputFile;
	HeightMap heightMap;
	int mapWidth;
	int mapHeight;

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
			heightMap.push_back(line);
		}
	}

	inputFile.close();

	//heightMap = debugInput;
	mapWidth = heightMap[0].length();
	mapHeight = heightMap.size();

	std::vector<Point> localMins;

	int sum = 0;
	for (int row = 0; row < mapHeight; row++) {
		for (int col = 0; col < mapWidth; col++) {
            int value = 10;
			if(isLocalMin(Point(row, col), heightMap, value)){
                sum += value + 1;
            }
		}
	}

	std::cout << "  Answer: " << sum << std::endl;
}

/*
*/
void day09Part02()
{}
