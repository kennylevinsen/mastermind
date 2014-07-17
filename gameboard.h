#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <list>
#include <termios.h>
#include <unistd.h>
#include "turn.h"
#include "solution.h"

/*
 * The game board - handles the terminal, accounts for moves
 */
class Gameboard {
public:
	// Constructor / Destructors
	Gameboard(Solution code, size_t rows) :
		_code(code),
		_rows(rows),
		solved(false),
		end(false)
	{
		init();
	}

	Gameboard(size_t rows) :
		_code(Solution()),
		_rows(rows),
		solved(false),
		end(false)
	{
		init();
	}

	Gameboard() :
		_code(Solution()),
		_rows(10),
		solved(false),
		end(false)
	{
		init();
	}

	~Gameboard()
	{
		reset();
	}

	// Terminal stuff
	void init();
	void reset();

	// Turn interfaces
	void addTurn(Turn t);
	void addTurn(Solution s);
	void addTurn(Solution s, Result r);

	// Board updating
	void printBoard(Solution cursor, size_t selectionCol) const;

private:
	Solution _code;
	std::list<Turn> turns;
	size_t _rows;
	bool solved;
	bool end;
	struct termios saved_attributes;

};

#endif /* GAMEBOARD_H */
