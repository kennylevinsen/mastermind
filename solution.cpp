#include "solution.h"

#include <stdlib.h>
#include <list>
#include <iostream>

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
 *
 * @param Gem p1 First gem
 * @param Gem p2 Second gem
 * @param Gem p3 Third gem
 * @param Gem p4 Fourth gem
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
 * @param Solution other The solution to compare with
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
 * @param Solution other The solution to test
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

			// Avoid duplication
			bool pushIt = true;
			for (std::list<Gem>::const_iterator it = left2.begin(); it != left2.end(); ++it)
				if (*it == other.pieces[i])
					pushIt = false;

			if (pushIt)
				left2.push_back(other.pieces[i]);
		}
	}

	// Check if some of the remaining gems at least had correct colors
	for (std::list<Gem>::const_iterator it1 = left2.begin(); it1 != left2.end(); ++it1) {
		for (std::list<Gem>::const_iterator it2 = left1.begin(); it2 != left1.end(); ++it2) {
			if (*it1 == *it2) {
				almost++;
				break;
			}
		}
	}

	return Result(correct, almost);
}

/*
 * Dumps the value of a given solution to stdout
 *
 * @param bool shortString Use short color notation
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
