#ifndef SOLUTION_H
#define SOLUTION_H

#include <array>

#include "gem.h"
#include "result.h"

/*
 * Container for 4 gems - a solution.
 * Also includes the capability to test a solution
 */
class Solution {
public:
	Solution();
	Solution(Gem p1, Gem p2, Gem p3, Gem p4);

	void setPiece(size_t index, Gem g) { pieces[index] = g; }
	const Result test(Solution s) const;

	bool operator==(Solution other) const;
	Gem operator[](size_t index) const { return pieces.operator[](index); }

	void dump(bool shortString) const;

private:
	std::array<Gem, 4> pieces;

};

#endif /* SOLUTION_H */
