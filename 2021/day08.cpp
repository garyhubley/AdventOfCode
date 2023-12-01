/*
You barely reach the safety of the cave when the whale smashes into the cave mouth, collapsing it.
Sensors indicate another exit to this cave at a much greater depth, so you have no choice but to
press on.

As your submarine slowly makes its way through the cave system, you notice that the four-digit
seven-segment displays in your submarine are malfunctioning; they must have been damaged during
the escape. You'll be in a lot of trouble without them, so you'd better figure out what's wrong.

Each digit of a seven-segment display is rendered by turning on or off any of seven segments named
a through g:

  0:      1:      2:      3:      4:
 aaaa    ....    aaaa    aaaa    ....
b    c  .    c  .    c  .    c  b    c
b    c  .    c  .    c  .    c  b    c
 ....    ....    dddd    dddd    dddd
e    f  .    f  e    .  .    f  .    f
e    f  .    f  e    .  .    f  .    f
 gggg    ....    gggg    gggg    ....

  5:      6:      7:      8:      9:
 aaaa    aaaa    aaaa    aaaa    aaaa
b    .  b    .  .    c  b    c  b    c
b    .  b    .  .    c  b    c  b    c
 dddd    dddd    ....    dddd    dddd
.    f  e    f  .    f  e    f  .    f
.    f  e    f  .    f  e    f  .    f
 gggg    gggg    ....    gggg    gggg
So, to render a 1, only segments c and f would be turned on; the rest would be off. To render a 7,
only segments a, c, and f would be turned on.

The problem is that the signals which control the segments have been mixed up on each display. The
submarine is still trying to display numbers by producing output on signal wires a through g, but
those wires are connected to segments randomly. Worse, the wire/segment connections are mixed up
separately for each four-digit display! (All of the digits within a display use the same
connections, though.)

So, you might know that only signal wires b and g are turned on, but that doesn't mean segments b
and g are turned on: the only digit that uses two segments is 1, so it must mean segments c and f
are meant to be on. With just that information, you still can't tell which wire (b/g) goes to
which segment (c/f). For that, you'll need to collect more information.

For each display, you watch the changing signals for a while, make a note of all ten unique
signal patterns you see, and then write down a single four digit output value (your puzzle input).
Using the signal patterns, you should be able to work out which pattern corresponds to which
digit.

For example, here is what you might see in a single entry in your notes:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf
(The entry is wrapped here to two lines so it fits; in your notes, it will all be on a single
line.)

Each entry consists of ten unique signal patterns, a | delimiter, and finally the four digit output
value. Within an entry, the same wire/segment connections are used (but you don't know what the
connections actually are). The unique signal patterns correspond to the ten different ways the
submarine tries to render a digit using the current wire/segment connections. Because 7 is the
only digit that uses three segments, dab in the above example means that to render a 7, signal
lines d, a, and b are on. Because 4 is the only digit that uses four segments, eafb means that to
render a 4, signal lines e, a, f, and b are on.

Using this information, you should be able to work out which combination of signal wires
corresponds to each of the ten digits. Then, you can decode the four digit output value.
Unfortunately, in the above example, all of the digits in the output value (cdfeb fcadb cdfeb
cdbaf) use five segments and are more difficult to deduce.

For now, focus on the easy digits. Consider this larger example:

be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb |
fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec |
fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef |
cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega |
efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga |
gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf |
gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf |
cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd |
ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg |
gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc |
fgae cfgab fg bagce
Because the digits 1, 4, 7, and 8 each use a unique number of segments, you should be able to
tell which combinations of signals correspond to those digits. Counting only digits in the output
values (the part after | on each line), in the above example, there are 26 instances of digits
that use a unique number of segments (highlighted above).

In the output values, how many times do digits 1, 4, 7, or 8 appear?
*/

#include "2021.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#define NUM_DIGITS (10)
#define NUM_SEGS (7)
#define NUM_OUTPUTS (4)

#define SEG_TOP          (0)
#define SEG_TOP_RIGHT    (1)
#define SEG_BOTTOM_RIGHT (2)
#define SEG_BOTTOM       (3)
#define SEG_BOTTOM_LEFT  (4)
#define SEG_TOP_LEFT     (5)
#define SEG_MIDDLE       (6)

static std::string filename = "/day08Input.txt";

static std::stringstream debugInput(
	"be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe\n\
	edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc\n\
	fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg\n\
	fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb\n\
	aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea\n\
	fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb\n\
	dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe\n\
	bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef\n\
	egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb\n\
	gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"
);

// number of segments illuminated per digit
const int numSegments[NUM_DIGITS] = {
	//  0  1  2  3  4  5  6  7  8  9
		6, 2, 5, 5, 4, 5, 6, 3, 7, 6
};

// digits 1, 4, 7, and 8 all have unique numbers of segments illuminated
bool isEasyDigit(const std::string& str) {
	auto len = str.length();
	return ((len == numSegments[1])
		|| (len == numSegments[4])
		|| (len == numSegments[7])
		|| (len == numSegments[8])
		);
}

// Checks to see if s1 has all the same letters as s2
bool isAnagram(const std::string& s1, const std::string& s2) {
	if (s1.length() != s2.length()) {
		return false;
	}
	// they are the same length
	for (auto c : s1) {
		if (s2.find(c) == std::string::npos) {
			// character not found. Not an anagram
			return false;
		}
	}

	// all characters the same. 
	return true;
}

// decyphers the digit using the codebook
int decypher(const std::string& digit, const std::vector<std::string>& codebook) {
	for (int i = 0; i < NUM_DIGITS; i++) {
		// check all digits to see if any are anagrams. 
		if (isAnagram(digit, codebook[i])) {
			// if the digit is an anagram of the codebook digit, return the codebook digit
			return i;
		}
	}

	// make the compiler shut up. nothing should reach here.
	return -1;
}

char segToIndex(int seg) {
	return (char)seg - 'a';
}

void countSegment(std::vector<int>& segmentsfound, char c) {
	segmentsfound[segToIndex(c)]++;
}

char indexToSeg(int idx) {
	return (char)idx + 'a';
}

void decode(const std::vector<std::string>& clues, std::vector<std::string>& decodedNumbers) {

	std::vector<std::string> lenSix;
	std::vector<std::string> lenFive;
	std::vector<int> segmentsfound(NUM_SEGS, 0);
	std::vector<char> segs(NUM_SEGS, '\0');
	char tmp;

	// find the easy digits first
	for (auto clue : clues) {
		for (auto c : clue) {
			countSegment(segmentsfound, c);
		}
		switch (clue.length()) {
		case 2: // 1
			decodedNumbers[1] = clue;
			break;
		case 3: // 7
			decodedNumbers[7] = clue;
			break;
		case 4: // 4
			decodedNumbers[4] = clue;
			break;
		case 7: // 8
			decodedNumbers[8] = clue;
			break;
		case 6:
			lenSix.push_back(clue);
			break;
		case 5:
			lenFive.push_back(clue);
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < NUM_SEGS; i++) {
		switch (segmentsfound[i]) {
		case 9:
			segs[SEG_BOTTOM_RIGHT] = indexToSeg(i);
			break;
		case 4:
			segs[SEG_BOTTOM_LEFT] = indexToSeg(i);
			break;
		case 6:
			segs[SEG_TOP_LEFT] = indexToSeg(i);
			break;
		case 7:
			tmp = indexToSeg(i);
			// if there are 7 instances of the segment, and the segment is lit up in '4' then it is
			// the middle segment
			if (decodedNumbers[4].find(tmp) != std::string::npos) {
				segs[SEG_MIDDLE] = tmp;
			}
			break;
		default:
			break;
		}
	}

	for (auto clue : lenFive) {
		if (clue.find(segs[SEG_TOP_LEFT]) != std::string::npos) {
			decodedNumbers[5] = clue;
		}
		else if (clue.find(segs[SEG_BOTTOM_LEFT]) != std::string::npos) {
			decodedNumbers[2] = clue;
		}
		else {
			decodedNumbers[3] = clue;
		}
	}

	// at this point, we know 1, 2, 3, 4, 5, 7, and 8
	// also know segments BL, TL, BR, and M
	for (auto clue : lenSix) {
		if (clue.find(segs[SEG_BOTTOM_LEFT]) == std::string::npos) {
			decodedNumbers[9] = clue;
		}
		else if (clue.find(segs[SEG_MIDDLE]) == std::string::npos) {
			decodedNumbers[0] = clue;
		}
		else {
			decodedNumbers[6] = clue;
		}

	}

	for (auto s : decodedNumbers) {
		if (s.empty()) {
			std::cout << "oops" << std::endl;
		}
	}

}

void day08Part01() {

	std::ifstream inputFile;
	std::vector<int> crabLocations;

	inputFile.open(inputDirectory + filename);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}

	int numEasyDigits = 0;
	while (!inputFile.eof()) {
		std::string eater;
		std::string t1, t2, t3, t4;

		inputFile >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater;
		inputFile >> t1 >> t2 >> t3 >> t4;

		/*
		debugInput >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater >> eater;
		debugInput >> t1 >> t2 >> t3 >> t4;
		std::cout << "Line " << i << ":" << std::endl;
		std::cout << "  1: " << t1 << std::endl;
		std::cout << "  2: " << t2 << std::endl;
		std::cout << "  3: " << t3 << std::endl;
		std::cout << "  4: " << t4 << std::endl;
*/
		numEasyDigits += isEasyDigit(t1) + isEasyDigit(t2) + isEasyDigit(t3) + isEasyDigit(t4);

		//std::cout << "  count: " << numEasyDigits << std::endl;
	}


	inputFile.close();

	std::cout << "  Answer: " << numEasyDigits << std::endl;
}

/*
Through a little deduction, you should now be able to determine the remaining digits. Consider
again the first example above:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf
After some careful analysis, the mapping between signal wires and segments only make sense in the
following configuration:

 dddd
e    a
e    a
 ffff
g    b
g    b
 cccc
So, the unique signal patterns would correspond to the following digits:

acedgfb: 8
cdfbe: 5
gcdfa: 2
fbcad: 3
dab: 7
cefabd: 9
cdfgeb: 6
eafb: 4
cagedb: 0
ab: 1
Then, the four digits of the output value can be decoded:

cdfeb: 5
fcadb: 3
cdfeb: 5
cdbaf: 3
Therefore, the output value for this entry is 5353.

Following this same process for each entry in the second, larger example above, the output value
of each entry can be determined:

fdgacbe cefdb cefbgd gcbe: 8394
fcgedb cgb dgebacf gc: 9781
cg cg fdcagb cbg: 1197
efabcd cedba gadfec cb: 9361
gecf egdcabf bgf bfgea: 4873
gebdcfa ecba ca fadegcb: 8418
cefg dcbef fcge gbcadfe: 4548
ed bcgafe cdgba cbgef: 1625
gbdfcae bgc cg cgb: 8717
fgae cfgab fg bagce: 4315
Adding all of the output values in this larger example produces 61229.

For each entry, determine all of the wire/segment connections and decode the four-digit output
values. What do you get if you add up all of the output values?
*/
void day08Part02() {

	std::ifstream inputFile;
	std::vector<int> crabLocations;

	inputFile.open(inputDirectory + filename);

	std::cout << "Starting function: " << __FUNCTION__ << std::endl;

	if (!inputFile.is_open()) {
		std::cout << "couldn't open file" << std::endl;
		return;
	}


	int sum = 0;
	while (!inputFile.eof()) {
	//for (int i = 0; i < 10; i++) {
		std::string eater;
		std::vector<std::string> codebook(NUM_DIGITS);
		std::vector<std::string> outputDigits(NUM_OUTPUTS);
		std::vector<std::string> clues(NUM_DIGITS);

		// get the 10 input clues
		for (int j = 0; j < NUM_DIGITS; j++) {
			//debugInput >> clues[j];
			inputFile >> clues[j];
		}

		//debugInput >> eater; // discard the | separator
		inputFile >> eater; // discard the | separator

		for (int j = 0; j < NUM_OUTPUTS; j++) {
			//debugInput >> outputDigits[j];
			inputFile >> outputDigits[j];
		}

		decode(clues, codebook);

		int multiplier = 1000;
		for (auto& digit : outputDigits) {
			sum += decypher(digit, codebook) * multiplier;
			multiplier /= 10;
		}
	}

	inputFile.close();

	std::cout << "  Answer: " << sum << std::endl;
}