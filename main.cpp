#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <list>
#include <string>
#include <iostream>

//
// Gem
//
enum GemColor { NONE, red, blue, yellow, green, white, black };
const char GemColorTable[][7] = { "none", "red", "blue", "yellow", "green", "white", "black" };

class Gem {
public:
	Gem() : color(NONE) {};
	Gem(int c) : color(GemColor(c)) {};
	Gem(GemColor c) : color(c) {};
	GemColor getColor() { return color; }
	std::string getColorString() { return GemColorTable[color]; }

	bool operator==(Gem other)
	{
		return other.color == color;
	}

	bool operator!=(Gem other)
	{
		return !operator==(other);
	}
private:
	GemColor color;
};

//
// Result object
//
class Result {
public:
	Result(int correct, int almost) : correct(correct), almost(almost) {}

	int getCorrect() { return correct; }
	int getAlmost() { return almost; }

private:
	int correct;
	int almost;
};

//
// Solution
//
class Solution {
public:
	Solution();
	Solution(Gem p1, Gem p2, Gem p3, Gem p4);

	bool operator==(Solution other);
	Result test(Solution s);
	void dump();

private:
	std::array<Gem, 4> pieces;
};

/*
 * Construct a random solution
 */
Solution::Solution()
{
	srand(time(NULL));
	for (int i = 0; i < 4; ++i) {
		pieces[i] = rand() % 6 + 1;
	}
}

/*
 * Construct a solution using the provided Gems
 */
Solution::Solution(Gem p1, Gem p2, Gem p3, Gem p4)
{
	pieces[0] = p1;
	pieces[1] = p2;
	pieces[2] = p3;
	pieces[3] = p4;
}

/*
 * Compare gems of other Solution instances
 *
 * @returns bool The result of the comparison
 */
bool Solution::operator==(Solution other)
{
	for (int i = 0; i < 4; ++i) {
		if (pieces[i] != other.pieces[i])
			return false;
	}

	return true;
}

/*
 * Tests a given solution "other" against itself.
 * This generates the "black/colored peg" and "white peg" values.
 *
 * @returns Result The "pegs"
 */
Result Solution::test(Solution other)
{
	int correct = 0, almost = 0;
	std::list<Gem> left1, left2;

	// Check for correct guesses
	for (int i = 0; i < 4; ++i) {
		if (pieces[i] == other.pieces[i]) {
			correct++;
		} else {
			left1.push_back(pieces[i]);
			left2.push_back(other.pieces[i]);
		}
	}

	// Check if some of the remaining gems at least had correct colors
	for (std::list<Gem>::iterator it1 = left1.begin(); it1 != left1.end(); ++it1)
		for (std::list<Gem>::iterator it2 = left2.begin(); it2 != left2.end(); ++it2)
			if (*it1 == *it2)
				almost++;

	return Result(correct, almost);
}

/*
 * Dumps the value of a given solution to stdout
 */
void Solution::dump()
{
	std::cout << "Solution: ";

	// Print the individual colors
	for (int i = 0; i < 4; ++i) {
		std::cout << pieces[i].getColorString();

		// Don't print a comma on the last element
		if (i != 3)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

//
// Game board
//
class Gameboard {
private:

};

//
// Flow
//
int main()
{
	Gem g1(black);
	Solution s1;
	Solution s2(black, red, blue, yellow);
	Solution s3(black, red, black, blue);
	s1.dump();
	s2.dump();
	s3.dump();

	Result r1 = s1.test(s2);
	Result r2 = s1.test(s3);
	std::cout << "Correct: " << std::to_string(r1.getCorrect()) <<
	             ", almost: " << std::to_string(r1.getAlmost()) << std::endl;
	std::cout << "Correct: " << std::to_string(r2.getCorrect()) <<
	             ", almost: " << std::to_string(r2.getAlmost()) << std::endl;
	return 0;
}
