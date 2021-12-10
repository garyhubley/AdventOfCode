/*
You're already almost 1.5km (almost a mile) below the surface of the ocean, already so deep that
you can't see any sunlight. What you can see, however, is a giant squid that has attached itself to
the outside of your submarine.

Maybe it wants to play bingo?

Bingo is played on a set of boards each consisting of a 5x5 grid of numbers. Numbers are chosen at
random, and the chosen number is marked on all boards on which it appears. (Numbers may not appear
on all boards.) If all numbers in any row or any column of a board are marked, that board wins.
(Diagonals don't count.)

The submarine has a bingo subsystem to help passengers (currently, you and the giant squid) pass
the time. It automatically generates a random order in which to draw numbers and a random set of
boards (your puzzle input). For example:

7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7
After the first five numbers are drawn (7, 4, 9, 5, and 11), there are no winners, but the boards
are marked as follows (shown here adjacent to each other to save space):

22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
After the next six numbers are drawn (17, 23, 2, 0, 14, and 21), there are still no winners:

22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
Finally, 24 is drawn:

22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
At this point, the third board wins because it has at least one complete row or column of marked
numbers (in this case, the entire top row is marked: 14 21 17 24 4).

The score of the winning board can now be calculated. Start by finding the sum of all unmarked
numbers on that board; in this case, the sum is 188. Then, multiply that sum by the number that was
just called when the board won, 24, to get the final score, 188 * 24 = 4512.

To guarantee victory against the giant squid, figure out which board will win first. What will your
final score be if you choose that board?
*/

#include "adventOfCode.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

typedef std::tuple<int, bool> bingoCell;
typedef std::vector<bingoCell> bingoBoard;

std::vector<int> debugNumbers = { 7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1 };

std::vector<bingoBoard> debugBoards = {
	{ {22,false}, {13,false}, {17,false}, {11,false}, {0, false},
	  {8, false}, {2, false}, {23, false}, {4, false}, {24,false},
	  {21,false}, {9, false}, {14,false}, {16,false}, {7, false},
	  {6, false}, {10,false}, {3, false}, {18,false}, {5, false},
	  {1, false}, {12,false}, {20,false}, {15,false}, {19, false}},
	{ {3,false}, {15,false}, {0,false}, {2,false}, {22,false},
	  {9,false}, {18,false}, {13,false}, {17,false}, {5,false},
	  {19,false}, {8,false}, {7,false}, {25,false}, {23,false},
	  {20,false}, {11,false}, {10,false}, {24,false}, {4,false},
	  {14,false}, {21,false}, {16,false}, {12,false}, {6, false}},
	{ {14,false}, {21,false}, {17,false}, {24,false}, { 4,false},
	  {10,false}, {16,false}, {15,false}, { 9,false}, {19,false},
	  {18,false}, { 8,false}, {23,false}, {26,false}, {20,false},
	  {22,false}, {11,false}, {13,false}, { 6,false}, { 5,false},
	  {2, false}, { 0,false}, {12,false}, {3, false}, { 7, false}}
};

bingoBoard GetBoard(std::ifstream& inputFile) {
	bingoBoard board(25);
	std::string str;
	std::getline(inputFile, str); // discard blank line
	for (int i = 0; i < 5; i++) {
		std::getline(inputFile, str);
		std::stringstream boardStream(str);
		int rowOffset = i * 5;
		for (int j = 0; j < 5; j++) {
			int tmp;
			boardStream >> tmp;
			board[rowOffset + j] = std::make_tuple(tmp, false);
		}
	}

	return board;
}

void MarkBoard(bingoBoard& board, int num) {
	for (auto& cell : board) {
		if (std::get<0>(cell) == num) {
			std::get<1>(cell) = true;
		}
	}
}

bool CheckRow(const bingoBoard& board, int row) {
	for (int col = 0; col < 5; col++) {
		if (std::get<1>(board[row * 5 + col]) == false) {
			return false;
		}
	}
	return true;
}

bool CheckCol(const bingoBoard& board, int col) {
	for (int row = 0; row < 5; row++) {
		if (std::get<1>(board[row * 5 + col]) == false) {
			return false;
		}
	}
	return true;
}

bool CheckBoard(const bingoBoard& board) {
	for (int i = 0; i < 5; i++) {
		if (CheckRow(board, i)) {
			return true;
		}
		if (CheckCol(board, i)) {
			return true;
		}
	}
	return false;
}

int CalcBoardSum(const bingoBoard& board) {
	// return (rowCol == ROW) ? CalcRowSum(board, rowColVal) : CalcColSum(board, rowColVal);
	int sum = 0;
	for (auto& cell : board) {
		if (std::get<1>(cell) == false) {
			sum += std::get<0>(cell);
		}
	}
	return sum;
}

void day04Part01() {

	std::ifstream inputFile;
	std::string str;
	int callNum;
	char eater;
	std::vector<int> calledNumbers;
	std::vector<bingoBoard> boards;

	inputFile.open(inputDirectory + "/day04Input.txt");

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	// get firs line, 
	std::getline(inputFile, str);

	// parse into vector<int> 
	std::stringstream ss(str);
	while (ss >> callNum) {
		ss >> eater;
		calledNumbers.push_back(callNum);
	}

	// get each board
	while (!inputFile.eof()) {
		boards.push_back(GetBoard(inputFile));
	}
	/*
	boards = debugBoards;
	calledNumbers = debugNumbers;

*/
	int winner = -1;
	int winningNum = -1;

	for (int i = 0; winner == -1 && i < calledNumbers.size(); ++i) {
		for (int j = 0; winner == -1 && j < boards.size(); ++j) {
			MarkBoard(boards[j], calledNumbers[i]);
			if (CheckBoard(boards[j])) {
				std::cout << "winner" << std::endl;
				winner = j;
				winningNum = i;
			}
		}
	}
	// for each number in input,i
	//   mark off each board
	//   check each board for winner

	int sum = CalcBoardSum(boards[winner]);

	// **** can't win before 5 numbers drawn, 
	std::cout << "Winner: " << winner << std::endl;
	std::cout << "  sum: " << sum * calledNumbers[winningNum] << std::endl;

}


/*
--- Part Two ---
On the other hand, it might be wise to try a different strategy: let the giant squid win.

You aren't sure how many bingo boards a giant squid could play at once, so rather than waste time
counting its arms, the safe thing to do is to figure out which board will win last and choose that
one. That way, no matter which boards it picks, it will win for sure.

In the above example, the second board is the last to win, which happens after 13 is eventually
called and its middle column is completely marked. If you were to keep playing until this point,
the second board would have a sum of unmarked numbers equal to 148 for a final score of 148 * 13 =
1924.

Figure out which board will win last. Once it wins, what would its final score be?
*/
void day04Part02() {

	std::ifstream inputFile;
	std::string str;
	int callNum;
	char eater;
	std::vector<int> calledNumbers;
	std::vector<bingoBoard> boards;

	inputFile.open(inputDirectory + "/day04Input.txt");

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	// get firs line, 
	std::getline(inputFile, str);

	// parse into vector<int> 
	std::stringstream ss(str);
	while (ss >> callNum) {
		ss >> eater;
		calledNumbers.push_back(callNum);
	}

	// get each board
	while (!inputFile.eof()) {
		boards.push_back(GetBoard(inputFile));
	}
	/*
	boards = debugBoards;
	calledNumbers = debugNumbers;

*/
	std::vector<int> winners(boards.size(), 0);
	int winCount = 0;
	int winner = -1;
	int winningNum = -1;

	for (int i = 0; i < calledNumbers.size(); ++i) {
		for (int j = 0; j < boards.size(); ++j) {
			MarkBoard(boards[j], calledNumbers[i]);
			if (CheckBoard(boards[j]) && winners[j] == 0) {
				winCount++;
				winners[j] = winCount;
				std::cout << "winner b" << j << " num[" << i << "] = " << calledNumbers[i] << std::endl;
				if (winCount == boards.size()) {
					winner = j;
					winningNum = i;
				}
			}
		}
	}

	int sum = CalcBoardSum(boards[winner]);

	std::cout << "Winner: " << winner << std::endl;
	std::cout << "  sum: " << sum * calledNumbers[winningNum] << std::endl;

}