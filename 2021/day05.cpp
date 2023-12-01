/*
You come across a field of hydrothermal vents on the ocean floor!These vents constantly produce
large, opaque clouds, so it would be best to avoid them if possible.

They tend to form in lines; the submarine helpfully produces a list of nearby lines of vents (your
puzzle input) for you to review. For example:

0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2
Each line of vents is given as a line segment in the format x1,y1 -> x2,y2 where x1,y1 are the
coordinates of one end the line segment and x2,y2 are the coordinates of the other end. These line
segments include the points at both ends. In other words:

An entry like 1,1 -> 1,3 covers points 1,1, 1,2, and 1,3.
An entry like 9,7 -> 7,7 covers points 9,7, 8,7, and 7,7.
For now, only consider horizontal and vertical lines: lines where either x1 = x2 or y1 = y2.

So, the horizontal and vertical lines from the above list would produce the following diagram:

.......1..
..1....1..
..1....1..
.......1..
.112111211
..........
..........
..........
..........
222111....
In this diagram, the top left corner is 0,0 and the bottom right corner is 9,9. Each position is
shown as the number of lines which cover that point or . if no line covers that point. The top-
left pair of 1s, for example, comes from 2,2 -> 2,1; the very bottom row is formed by the
overlapping lines 0,9 -> 5,9 and 0,9 -> 2,9.

To avoid the most dangerous areas, you need to determine the number of points where at least two
lines overlap. In the above example, this is anywhere in the diagram with a 2 or larger - a total
of 5 points.

Consider only horizontal and vertical lines. At how many points do at least two lines overlap?
*/

#include "2021.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

static std::string filename = "/day05Input.txt";

struct Point {
	//Point(int x = 0, int y = 0) : x(0), y(0) {}

	int x = 0, y = 0;
};

struct Line {
	Line(Point s, Point e) : start(s), end(e) {}

	Point start = { 0, 0 };
	Point end = { 0, 0 };

	bool isHorizontal() {
		return start.y == end.y;
	}

	bool isVertical() {
		return start.x == end.x;
	}
};

struct Layout {
	Layout(int w, int h) : width(w), height(h), area(w* h, 0) {}

	std::vector<int> area;
	int width, height;
};

std::vector<Line> debugLines = {
	{{0, 9},{5, 9}},
	{{8, 0},{0, 8}},
	{{9, 4},{3, 4}},
	{{2, 2},{2, 1}},
	{{7, 0},{7, 4}},
	{{6, 4},{2, 0}},
	{{0, 9},{2, 9}},
	{{3, 4},{1, 4}},
	{{0, 0},{8, 8}},
	{{5, 5},{8, 2}}
};

void PrintLayout(std::ostream& output, Layout layout) {
	for (int i = 0; i < layout.area.size(); ++i) {
		if (!(i % layout.width)) {
			output << std::endl;
		}

		if ((layout.area[i] == 0)) {
			output << ".";
		}
		else {
			output << layout.area[i];
		}
		//std::cout << " ";
	}
}

void DrawHorizontal(const Line& l, Layout& layout) {
	// y is the row, find start point
	int startIdx;
	int length;

	if (l.start.x < l.end.x) {
		startIdx = l.start.y * layout.width + l.start.x;
		length = l.end.x - l.start.x;
	}
	else {
		startIdx = l.end.y * layout.width + l.end.x;
		length = l.start.x - l.end.x;
	}

	for (int i = startIdx; i <= length + startIdx; i++) {
		layout.area[i]++;
	}

}

void DrawVertical(const Line& l, Layout& layout) {

	// x is the col, find start point
	int startIdx;
	int length;
	int endIdx;

	if (l.start.y < l.end.y) {
		startIdx = l.start.y * layout.width + l.start.x;
		endIdx = l.end.y * layout.width + l.end.x;
		length = l.end.y - l.start.y;
	}
	else {
		startIdx = l.end.y * layout.width + l.end.x;
		endIdx = l.start.y * layout.width + l.start.x;
		length = l.start.y - l.end.y;
	}

	for (int i = startIdx; i <= endIdx; i += layout.width) {
		layout.area[i]++;
	}

	//std::cout << std::endl;
	//PrintLayout(layout);
}

void DrawDiagonal(const Line& l, Layout& layout) {

	int startIdx;
	int mod;
	int endIdx;

	if (l.start.x < l.end.x) {
		// going right
		if (l.start.y < l.end.y) {
			// going down
			startIdx = l.start.y * layout.width + l.start.x;
			endIdx = l.end.y * layout.width + l.end.x;
			mod = 1;
		}
		else {
			// going up
			startIdx = l.end.y * layout.width + l.end.x;
			endIdx = l.start.y * layout.width + l.start.x;
			mod = -1;
		}
	}
	else {
		// going left
		if (l.start.y < l.end.y) {
			// going down
			startIdx = l.start.y * layout.width + l.start.x;
			endIdx = l.end.y * layout.width + l.end.x;
			mod = -1;
		}
		else {
			// going up
			startIdx = l.end.y * layout.width + l.end.x;
			endIdx = l.start.y * layout.width + l.start.x;
			mod = 1;
		}
	}

	for (int i = startIdx; i <= endIdx; i += layout.width + mod) {
		layout.area[i]++;
	}

}

void day05Part01() {

	std::ifstream inputFile;
	std::ofstream outputFile;
	std::string str;
	int width = 0;
	int height = 0;
	std::vector<Line> lines;

	inputFile.open(inputDirectory + filename);
	outputFile.open("layout.txt");

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	// get first line, 
	while (!inputFile.eof()) {
		int x1, x2;
		int y1, y2;
		char eater;
		std::getline(inputFile, str);
		std::stringstream ss(str);
		ss >> x1 >> eater >> y1 >> eater >> eater >> x2 >> eater >> y2;
		//std::cout << "{" << x1 << "," << y1 << "}, ";
		//std::cout << "{" << x2 << "," << y2 << "}" << std::endl;
		Line line({ x1,y1 }, { x2,y2 });

		lines.push_back(line);

	}

	inputFile.close();

	lines = debugLines;

	// find layout width and height
	for (auto& l : lines) {
		int maxX = (l.start.x > l.end.x) ? l.start.x : l.end.x;
		int maxY = (l.start.y > l.end.y) ? l.start.y : l.end.y;

		width = (width > maxX) ? width : maxX + 1;
		height = (height > maxY) ? height : maxY + 1;
	}

	Layout layout(width, height);

	for (auto& l : lines) {
		if (l.isHorizontal()) {
			DrawHorizontal(l, layout);
		}
		else if (l.isVertical()) {
			DrawVertical(l, layout);
		}
	}

	int sum = 0;
	for (auto v : layout.area) {
		if (v > 1) {
			sum++;
		}
	}

	//PrintLayout(outputFile, layout);
	PrintLayout(std::cout, layout);
	outputFile.close();
	std::cout << std::endl;
	std::cout << "  Answer: " << sum << std::endl;
}

/*
Unfortunately, considering only horizontal and vertical lines doesn't give you the full picture;
you need to also consider diagonal lines.

Because of the limits of the hydrothermal vent mapping system, the lines in your list will only
ever be horizontal, vertical, or a diagonal line at exactly 45 degrees. In other words:

An entry like 1,1 -> 3,3 covers points 1,1, 2,2, and 3,3.
An entry like 9,7 -> 7,9 covers points 9,7, 8,8, and 7,9.
Considering all lines from the above example would now produce the following diagram:

1.1....11.
.111...2..
..2.1.111.
...1.2.2..
.112313211
...1.2....
..1...1...
.1.....1..
1.......1.
222111....
You still need to determine the number of points where at least two lines overlap. In the above
example, this is still anywhere in the diagram with a 2 or larger - now a total of 12 points.

Consider all of the lines. At how many points do at least two lines overlap?
*/
void day05Part02() {

	std::ifstream inputFile;
	std::ofstream outputFile;
	std::string str;
	int width = 0;
	int height = 0;
	std::vector<Line> lines;

	inputFile.open(inputDirectory + filename);
	outputFile.open("layout.txt");

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	// get first line, 
	while (!inputFile.eof()) {
		int x1, x2;
		int y1, y2;
		char eater;
		std::getline(inputFile, str);
		std::stringstream ss(str);
		ss >> x1 >> eater >> y1 >> eater >> eater >> x2 >> eater >> y2;
		//std::cout << "{" << x1 << "," << y1 << "}, ";
		//std::cout << "{" << x2 << "," << y2 << "}" << std::endl;
		Line line({ x1,y1 }, { x2,y2 });

		lines.push_back(line);

	}

	inputFile.close();

	//lines = debugLines;

	// find layout width and height
	for (auto& l : lines) {
		int maxX = (l.start.x > l.end.x) ? l.start.x : l.end.x;
		int maxY = (l.start.y > l.end.y) ? l.start.y : l.end.y;

		width = (width > maxX) ? width : maxX + 1;
		height = (height > maxY) ? height : maxY + 1;
	}

	Layout layout(width, height);

	for (auto& l : lines) {
		if (l.isHorizontal()) {
			DrawHorizontal(l, layout);
		}
		else if (l.isVertical()) {
			DrawVertical(l, layout);
		}
		else {
			DrawDiagonal(l, layout);
		}
	}

	int sum = 0;
	for (auto v : layout.area) {
		if (v > 1) {
			sum++;
		}
	}

	//PrintLayout(outputFile, layout);
	//PrintLayout(std::cout, layout);
	outputFile.close();
	std::cout << std::endl;
	std::cout << "  Answer: " << sum << std::endl;
}