/*
Next, you should verify the life support rating, which can be determined by multiplying the oxygen
generator rating by the CO2 scrubber rating.

Both the oxygen generator rating and the CO2 scrubber rating are values that can be found in your
diagnostic report - finding them is the tricky part. Both values are located using a similar
process that involves filtering out values until only one remains. Before searching for either
rating value, start with the full list of binary numbers from your diagnostic report and consider
just the first bit of those numbers. Then:

Keep only numbers selected by the bit criteria for the type of rating value for which you are
searching. Discard numbers which do not match the bit criteria.
If you only have one number left, stop; this is the rating value for which you are searching.
Otherwise, repeat the process, considering the next bit to the right.
The bit criteria depends on which type of rating value you want to find:

To find oxygen generator rating, determine the most common value (0 or 1) in the current bit
position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep
values with a 1 in the position being considered.
To find CO2 scrubber rating, determine the least common value (0 or 1) in the current bit position,
and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values
with a 0 in the position being considered.
For example, to determine the oxygen generator rating value using the same example diagnostic
report from above:

Start with all 12 numbers and consider only the first bit of each number. There are more 1 bits (7)
than 0 bits (5), so keep only the 7 numbers with a 1 in the first position: 11110, 10110, 10111,
10101, 11100, 10000, and 11001.
Then, consider the second bit of the 7 remaining numbers: there are more 0 bits (4) than 1 bits
(3), so keep only the 4 numbers with a 0 in the second position: 10110, 10111, 10101, and 10000.
In the third position, three of the four numbers have a 1, so keep those three: 10110, 10111, and
10101.
In the fourth position, two of the three numbers have a 1, so keep those two: 10110 and 10111.
In the fifth position, there are an equal number of 0 bits and 1 bits (one each). So, to find the
oxygen generator rating, keep the number with a 1 in that position: 10111.
As there is only one number left, stop; the oxygen generator rating is 10111, or 23 in decimal.
Then, to determine the CO2 scrubber rating value from the same example above:

Start again with all 12 numbers and consider only the first bit of each number. There are fewer 0
bits (5) than 1 bits (7), so keep only the 5 numbers with a 0 in the first position: 00100, 01111,
00111, 00010, and 01010.
Then, consider the second bit of the 5 remaining numbers: there are fewer 1 bits (2) than 0 bits
(3), so keep only the 2 numbers with a 1 in the second position: 01111 and 01010.
In the third position, there are an equal number of 0 bits and 1 bits (one each). So, to find the
CO2 scrubber rating, keep the number with a 0 in that position: 01010.
As there is only one number left, stop; the CO2 scrubber rating is 01010, or 10 in decimal.
Finally, to find the life support rating, multiply the oxygen generator rating (23) by the CO2
scrubber rating (10) to get 230.

Use the binary numbers in your diagnostic report to calculate the oxygen generator rating and CO2
scrubber rating, then multiply them together. What is the life support rating of the submarine?
(Be sure to represent your answer in decimal, not binary.)
*/

#include "adventOfCode.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

std::vector<std::string> debugReport = {
	"00100", "11110", "10110", "10111", "10101", "01111",
	"00111", "11100", "10000", "11001", "00010", "01010"
};

std::vector<std::string> OxygenGenerator(const std::vector<std::string>& diagnosticReport, int bitNum, bool oxy) {
	auto sz = diagnosticReport[0].length();
	auto reportSize = diagnosticReport.size();
	std::vector<int> numOnes(sz);
	std::vector<std::string> reducedDiagReport;
	int ones;
	int zeros;
	char bit;

	for (auto code : diagnosticReport) {
		for (int i = 0; i < sz; i++) {
			if (code[i] == '1') {
				numOnes[i]++;
			}
		}
	}

	ones = numOnes[bitNum];
	zeros = reportSize - numOnes[bitNum];
	if (oxy) {
		bit = ones >= zeros ? '1' : '0';
	}
	else {
		bit = zeros > ones ? '1' : '0';
	}

	for (int i = 0; i < reportSize; i++) {
		if (diagnosticReport[i][bitNum] == bit) {
			reducedDiagReport.push_back(diagnosticReport[i]);
		}
	}

	return reducedDiagReport;
}

int BinaryStringToInt(const std::string& binaryString) {

	int ret = 0;
	auto sz = binaryString.size();
	for (int i = 0; i < sz; ++i)
	{
		if (binaryString[i] == '1') {
			ret += std::pow(2, sz - (i + 1));
		}
	}

	return ret;
}

void day03Part02() {

	std::ifstream inputFile;
	std::string str;
	std::vector<std::string> diagnosticReport;
	std::vector<std::string> oxygenGen;
	std::vector<std::string> co2Scrubber;
	std::ios_base::fmtflags f(std::cout.flags());
	int oxyRating;
	int co2Rating;
	size_t binaryLength;


	inputFile.open(inputDirectory + "/day03Input.txt");

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	while (!inputFile.eof()) {
		std::getline(inputFile, str);
		diagnosticReport.push_back(str);
	}

	oxygenGen = diagnosticReport;
	co2Scrubber = diagnosticReport;

	binaryLength = diagnosticReport[0].length();

	for (int i = 0; oxygenGen.size() > 1 && i < binaryLength; i++) {
		oxygenGen = OxygenGenerator(oxygenGen, i, true);
	}

	for (int i = 0; co2Scrubber.size() > 1 && i < binaryLength; i++) {
		co2Scrubber = OxygenGenerator(co2Scrubber, i, false);
	}

	oxyRating = BinaryStringToInt(oxygenGen[0]);
	co2Rating = BinaryStringToInt(co2Scrubber[0]);

	std::cout << "dr: " << oxygenGen[0] << ", " << oxyRating << ", " << oxygenGen.size() << std::endl;
	std::cout << "dr: " << co2Scrubber[0] << ", " << co2Rating << ", " << co2Scrubber.size() << std::endl;

	std::cout << "product: " << oxyRating * co2Rating << std::endl;
}
