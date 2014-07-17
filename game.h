#ifndef GAME_H
#define GAME_H

#include "gameboard.h"
#include "solution.h"
#include "gem.h"

/*
 * The game handler
 */
class Game {
public:

	// Constructors
	Game() : board(20), cursor(Solution(red, red, red, red)), selectionColumn(0), selection(none) {}

	// The loop
	int run();

private:
	// Handlers
	void update();
	void submit();
	void nextColumn();
	void prevColumn();
	void nextColor();
	void prevColor();

	// Members
	Gameboard board;
	Solution cursor;
	size_t selectionColumn;
	Gem selection;

};

#endif /* GAME_H */
