#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <list>
#include <string>
#include <iostream>

//
// Gem
//
enum GemColor { NONE, red, blue, yellow, green, orange, purple };
const char GemColorTable[][7] = { "none", "red", "blue", "yellow", "green", "orange", "purple" };

class Gem {
public:
	Gem() : _color(NONE) {}
	Gem(int color) : _color(GemColor(color)) {}
	Gem(GemColor color) : _color(color) {}
	GemColor getColor() const { return _color; }
	const std::string getColorString() const { return GemColorTable[_color]; }
	const std::string getShortColorString() const { return std::string(GemColorTable[_color]).substr(0, 1); }

	bool operator==(Gem other) const
	{
		return other._color == _color;
	}

	bool operator!=(Gem other) const
	{
		return !operator==(other);
	}
private:
	GemColor _color;
};

//
// Result object
//
class Result {
public:
	Result(int correct, int almost) : _correct(correct), _almost(almost) {}

	int getCorrect() { return _correct; }
	int getAlmost() { return _almost; }

private:
	int _correct;
	int _almost;
};

//
// Solution
//
class Solution {
public:
	Solution();
	Solution(Gem p1, Gem p2, Gem p3, Gem p4);

	bool operator==(Solution other) const;
	const Result test(Solution s) const;
	void dump(bool shortString) const;

private:
	std::array<Gem, 4> pieces;
};

/*
 * Construct a random solution
 */
Solution::Solution()
{
	srand(static_cast<unsigned int>(time(NULL)));
	for (unsigned int i = 0; i < 4; ++i) {
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
bool Solution::operator== (Solution other) const
{
	for (unsigned int i = 0; i < 4; ++i) {
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
const Result Solution::test(Solution other) const
{
	int correct = 0, almost = 0;
	std::list<Gem> left1, left2;

	// Check for correct guesses
	for (unsigned int i = 0; i < 4; ++i) {
		if (pieces[i] == other.pieces[i]) {
			correct++;
		} else {
			left1.push_back(pieces[i]);
			left2.push_back(other.pieces[i]);
		}
	}

	// Check if some of the remaining gems at least had correct colors
	for (std::list<Gem>::const_iterator it1 = left1.begin(); it1 != left1.end(); ++it1)
		for (std::list<Gem>::const_iterator it2 = left2.begin(); it2 != left2.end(); ++it2)
			if (*it1 == *it2)
				almost++;

	return Result(correct, almost);
}

/*
 * Dumps the value of a given solution to stdout
 */
void Solution::dump(bool shortString) const
{
	std::cout << "Solution: ";

	// Print the individual colors
	for (unsigned int i = 0; i < 4; ++i) {
		if (shortString)
			std::cout << pieces[i].getShortColorString();
		else
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
class Turn {
public:
	Turn(Solution s, Result r) : solution(s), result(r) {}
	const Result getResult() const { return result; }
	const Solution getSolution() const { return solution; }
private:
	const Solution solution;
	const Result result;
};

class Gameboard {
public:
	Gameboard(Solution code) : _code(code) {}
	void addTurn(Turn t);
	void addTurn(Solution s) { Result r = _code.test(s); addTurn(s, r); }
	void addTurn(Solution s, Result r) { addTurn(Turn(s, r)); }
	void printBoard() const;
private:
	Solution _code;
	std::list<Turn> turns;
};

void Gameboard::addTurn(Turn t) {
	turns.push_back(t);
}

void Gameboard::printBoard() const {
	for (std::list<Turn>::const_iterator it = turns.begin(); it != turns.end(); ++it) {
		std::cout << "------" << std::endl;
		(*it).getSolution().dump(true);
		Result r = (*it).getResult();
		std::cout << "Correct: " << std::to_string(r.getCorrect()) <<
	             ", almost: " << std::to_string(r.getAlmost()) << std::endl;
	}
}

//
// Flow
//
int main()
{
	Solution s1;
	Gameboard g(s1);

	Solution s2(orange, red, blue, yellow);
	Solution s3(orange, red, purple, blue);
	g.addTurn(s2);
	g.addTurn(s3);
	g.printBoard();
	return 0;
}
