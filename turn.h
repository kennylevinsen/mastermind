#ifndef TURN_H
#define TURN_H

#include "solution.h"
#include "result.h"

/*
 * A tuple-like to contain a solution and result pair
 */
class Turn {
public:
	Turn(Solution s, Result r) : solution(s), result(r) {}
	const Result getResult() const { return result; }
	const Solution getSolution() const { return solution; }

private:
	const Solution solution;
	const Result result;

};

#endif /* TURN_H */
