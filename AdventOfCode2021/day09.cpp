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

#define DEBUG_WIDTH (10)
#define DEBUG_HEIGHT (5)

struct Point {
	Point(int r = -1, int c = -1) : row(r), col(c) {}
	int row;
	int col;
	friend bool operator ==(const Point& p1, const Point& p2);
};

bool operator ==(const Point& p1, const Point& p2) {
	return (p1.row == p2.row && p1.col == p2.col);
}

enum class EDGE {
	TOP_LEFT,
	TOP,
	TOP_RIGHT,
	RIGHT,
	BOTTOM_RIGHT,
	BOTTOM,
	BOTTOM_LEFT,
	LEFT,
	NONE
};

typedef std::vector<std::string> HeightMap;
const static HeightMap debugInput = { "2199943210","3987894921","9856789892","8767896789","9899965678" };

EDGE detectEdge(const Point p0, const int width, const int height) {

	if (p0.row == 0) {
		// top row
		if (p0.col == 0) {
			return EDGE::TOP_LEFT;
		}

		if (p0.col == (width - 1)) {
			return EDGE::TOP_RIGHT;
		}

		return EDGE::TOP;
	}
	if (p0.row == (height - 1)) {
		// bottom row
		if (p0.col == 0) {
			return EDGE::BOTTOM_LEFT;
		}

		if (p0.col == (width - 1)) {
			return EDGE::BOTTOM_RIGHT;
		}

		return EDGE::BOTTOM;
	}
	if (p0.col == 0) {
		return EDGE::LEFT;
	}
	if (p0.col == width - 1) {
		return EDGE::RIGHT;
	}

	return EDGE::NONE;
}

inline Point calcP1(const Point& p0) { return Point(p0.row, p0.col + 1); }

inline Point calcP2(const Point& p0) { return Point(p0.row + 1, p0.col); }

inline Point calcP3(const Point& p0) { return Point(p0.row, p0.col - 1); }

inline Point calcP4(const Point& p0) { return Point(p0.row - 1, p0.col); }

int getValue(const Point& point, const HeightMap& heightMap) {
	return heightMap[point.row][point.col] - '0';
}

Point findLocalMin(const Point& p0, const HeightMap& heightMap) {

	auto width = heightMap[0].length();
	auto height = heightMap.size();

	// get adjacent points
	/*
		p4
	p3  p0  p1
		p2
	p0 = r  , c
	p1 = r  , c+1
	p2 = r+1, c
	p3 = r  , c-1
	p4 = r-1, c
	*/

	std::vector<Point> points = { p0 };

	switch (detectEdge(p0, width, height))
	{
	case EDGE::TOP_LEFT:
		points.push_back(calcP1(p0));
		points.push_back(calcP2(p0));
		break;
	case EDGE::TOP:
		points.push_back(calcP1(p0));
		points.push_back(calcP2(p0));
		points.push_back(calcP3(p0));
		break;
	case EDGE::TOP_RIGHT:
		points.push_back(calcP2(p0));
		points.push_back(calcP3(p0));
		break;
	case EDGE::RIGHT:
		points.push_back(calcP2(p0));
		points.push_back(calcP3(p0));
		points.push_back(calcP4(p0));
		break;
	case EDGE::BOTTOM_RIGHT:
		points.push_back(calcP3(p0));
		points.push_back(calcP4(p0));
		break;
	case EDGE::BOTTOM:
		points.push_back(calcP1(p0));
		points.push_back(calcP3(p0));
		points.push_back(calcP4(p0));
		break;
	case EDGE::BOTTOM_LEFT:
		points.push_back(calcP1(p0));
		points.push_back(calcP4(p0));
		break;
	case EDGE::LEFT:
		points.push_back(calcP1(p0));
		points.push_back(calcP2(p0));
		points.push_back(calcP4(p0));
		break;
	case EDGE::NONE:
		points.push_back(calcP1(p0));
		points.push_back(calcP2(p0));
		points.push_back(calcP3(p0));
		points.push_back(calcP4(p0));
		break;
	default:
		break;
	}

	Point min = points[0];
	auto minValue = getValue(min, heightMap);
	for (auto& p : points) {
		int value = getValue(p, heightMap);
		if (value < minValue) {
			min = p;
			minValue = value;
		}
	}

	if (min == points[0]) {
		// found the local min
		return min;
	}

	return findLocalMin(min, heightMap);
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

	inputFile.open(inputDirectory + filename);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
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

	for (int row = 0; row < mapHeight; row++) {
		for (int col = 0; col < mapWidth; col++) {
			Point min = findLocalMin(Point(row, col), heightMap);
			if (!alreadyFound(min, localMins)) {
				localMins.push_back(min);
			}
		}
	}

	int sum = 0;
	for (auto& min : localMins) {
		sum += getValue(min, heightMap) + 1;
		int num = getValue(min, heightMap);
		if (num == 9) {
			std::cout << "P: (" << min.row << ", " << min.col << "): " << num << std::endl;
		}
	}
	std::cout << "  Answer: " << sum << std::endl;
}

/*
*/
void day09Part02()
{}