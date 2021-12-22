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
#include <tuple>

static std::string filename = "/day09Input.txt";

enum Directions {
	CENTER,
	EAST,
	SOUTH,
	WEST,
	NORTH,
	NUM_DIRS
};

struct Point {
	Point(int r = -1, int c = -1) : row(r), col(c) {}
	int row;
	int col;

	bool isValid() { return row >= 0 && col >= 0; }

	friend bool operator ==(const Point& p1, const Point& p2);
	friend std::ostream& operator <<(std::ostream& os, const Point& p);
};

std::ostream& operator <<(std::ostream& os, const Point& p) {
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

bool  isAtTop(const Point& p) { return p.row == 0; }
bool  isAtLeft(const Point& p) { return p.col == 0; }
bool  isAtBottom(const Point& p, int height) { return p.row == (height - 1); }
bool  isAtRight(const Point& p, int width) { return p.col == (width - 1); }

bool isLocalMin(const Point& p0, const HeightMap& heightMap, int& value) {

	auto width = heightMap[0].length();
	auto height = heightMap.size();

	std::vector<int> vals(5, 10);
	vals[Directions::CENTER] = getValue(p0, heightMap);
	if (!isAtBottom(p0, height)) {
		// not on bottom, calc south
		vals[Directions::SOUTH] = getValue(calcSouth(p0), heightMap);
	}
	if (!isAtTop(p0)) {
		// not on top, calc north
		vals[Directions::NORTH] = getValue(calcNorth(p0), heightMap);
	}
	if (!isAtRight(p0, width)) {
		// not on right, calc east
		vals[Directions::EAST] = getValue(calcEast(p0), heightMap);
	}
	if (!isAtLeft(p0)) {
		// not on left, calc west
		vals[Directions::WEST] = getValue(calcWest(p0), heightMap);
	}

	if (vals[Directions::CENTER] < vals[Directions::NORTH]
		&& vals[Directions::CENTER] < vals[Directions::EAST]
		&& vals[Directions::CENTER] < vals[Directions::SOUTH]
		&& vals[Directions::CENTER] < vals[Directions::WEST]
		)
	{
		value = vals[Directions::CENTER];
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
			if (isLocalMin(Point(row, col), heightMap, value)) {
				sum += value + 1;
			}
		}
	}

	std::cout << "  Answer: " << sum << std::endl;
}

Point findLocalMin(const Point& p, const HeightMap& heightMap) {

	auto width = heightMap[0].length();
	auto height = heightMap.size();

	std::vector<Point> points(5);

	points[Directions::CENTER] = p;
	if (!isAtBottom(p, height)) {
		// not on bottom, calc south
		points[Directions::SOUTH] = calcSouth(p);
	}
	if (!isAtTop(p)) {
		// not on top, calc north
		points[Directions::NORTH] = calcNorth(p);
	}
	if (!isAtRight(p, width)) {
		// not on right, calc east
		points[Directions::EAST] = calcEast(p);
	}
	if (!isAtLeft(p)) {
		// not on left, calc west
		points[Directions::WEST] = calcWest(p);
	}

	// find min. if all are 9, then there is no min
	auto minPoint = Point();
	auto minValue = 9;
	for (int dir = Directions::CENTER; dir < Directions::NUM_DIRS; dir++) {
		Point point = points[dir];
		if (point.isValid()) {
			int value = getValue(point, heightMap);
			if (value < minValue) {
				minValue = value;
				minPoint = point;
			}
		}
	}

	if (!minPoint.isValid() || minValue == 9) {
		return Point();
	}

	if (minPoint == p) {
		// found a local min
		return p;
	}

	// recurse
	return findLocalMin(minPoint, heightMap);
}

void addToMins(const Point& p, std::vector<std::tuple<Point, int >>& mins) {

	for (auto& min : mins) {
		auto minPoint = std::get<0>(min);
		if (minPoint == p) {
			// already been added. increase count.
			std::get<1>(min)++;
			return;
		}
	}
	// hasn't beena added. create it.
	mins.push_back(std::make_tuple(p, 1));
}

int bigBasinProduct(const std::vector<std::tuple<Point, int>> &basins) {
	int m1 = 0, m2 = 0, m3 = 0;

	for (auto& basin : basins) {
		int val = std::get<1>(basin);
		if (val > m1) {
			m3 = m2;
			m2 = m1;
			m1 = val;
		}
		else if (val > m2) {
			m3 = m2;
			m2 = val;
		}
		else if (val > m3) {
			m3 = val;
		}
	}

	return m1 * m2 * m3;
}

/*
Next, you need to find the largest basins so you know what areas are most important to avoid.

A basin is all locations that eventually flow downward to a single low point. Therefore, every low
point has a basin, although some basins are very small. Locations of height 9 do not count as being
in any basin, and all other locations will always be part of exactly one basin.

The size of a basin is the number of locations within the basin, including the low point. The
example above has four basins.

The top-left basin, size 3:

2199943210
3987894921
9856789892
8767896789
9899965678
The top-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678
The middle basin, size 14:

2199943210
3987894921
9856789892
8767896789
9899965678
The bottom-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678
Find the three largest basins and multiply their sizes together. In the above example, this is
9 * 14 * 9 = 1134.

What do you get if you multiply together the sizes of the three largest basins?
*/
void day09Part02() {

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

	std::vector<std::tuple<Point, int>> basins;

	for (int row = 0; row < mapHeight; row++) {
		for (int col = 0; col < mapWidth; col++) {
			Point test = Point(row, col);
			Point localMin = findLocalMin(test, heightMap);
			if (localMin.isValid() && getValue(test, heightMap) != 9) {
				addToMins(localMin, basins);
			}
		}
	}

	std::cout << "  Answer: " << bigBasinProduct(basins) << std::endl;
}
