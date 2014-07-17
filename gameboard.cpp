#include "gameboard.h"
#include <iostream>
#include <stdlib.h>

/*
 * Init the terminal for the board
 */
void Gameboard::init()
{
	struct termios tattr;
	if (!isatty(STDIN_FILENO)) {
		fprintf(stderr, "Not connected to a terminal!\n");
		exit(-1);
	}

	tcgetattr(STDIN_FILENO, &saved_attributes);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= static_cast<size_t>(~(ICANON|ECHO));
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);

   std::cout << "\x1b[?25l";
   std::cout << "\x1b[?1049h";
}

/*
 * Reset the terminal
 */
void Gameboard::reset()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
	std::cout << "\x1b[2J";
	std::cout << "\x1b[?1049l";
	std::cout << "\x1b[?25h";
}

/*
 * Adds a turn to the current game
 *
 * @param Turn t The turn to add
 */
void Gameboard::addTurn(Turn t)
{
	// NOPE!
	if (end || solved)
		return;

	// Update the game status
	turns.push_back(t);
	if (t.getResult().getCorrect() == 4) {
		solved = true;
		end = true;
	}
	if (turns.size() >= _rows) {
		end = true;
	}
}

/*
 * Adds a turn to the current game
 * Calculates the result from the solution itself.
 *
 * @param Solution s Solution to add
 */
void Gameboard::addTurn(Solution s)
{
	Result r = _code.test(s);
	Turn t(s, r);
	addTurn(t);
}

/*
 * Adds a turn to the current game
 * Makes a turn object using the given solution and result objects
 *
 * @param Solution s Solution to add
 * @param Result r Result to add
 */
void Gameboard::addTurn(Solution s, Result r)
{
	Turn t(s, r);
	addTurn(t);
}


/*
 * Prints the board
 *
 * @param Cursor cursor The current cursor
 */
void Gameboard::printBoard(Solution cursor, size_t selectionCol) const
{
	std::cout << "\x1b[1;1H";
	// Prepare the game header
	std::cout << "╭───────────────╮          " << std::endl;

	if (end) {
		if (solved) {
			std::cout << "│  ! YOU WON !  │          " << std::endl;
		} else {
			std::cout << "│  ! FAILURE !  │          " << std::endl;
		}
	} else {
		std::cout << "│   MASTERMIND  │          " << std::endl;
	}

	std::cout << "├───┬───┬───┬───┤ ╭───┬───╮" << std::endl;

	// Generate the empty rows
	for (size_t i = turns.size(); i < _rows; ++i) {
		for (size_t y = 0; y < 4; ++y) {

			// Time for a cursor?
			if (!solved && i == _rows-1) {
				std::cout << "│ ";
				if (selectionCol == y)
					std::cout << "\x1b[30m\x1b[47m";
				std::cout << cursor[y].getShortColorString() << "\x1b[0m ";
			} else {
				std::cout << "│   ";
			}

		}
		// The empty score section
		std::cout << "│ │   │   │" << std::endl;
	}

	// Time for the existing guesses
	for (std::list<Turn>::const_reverse_iterator it1 = turns.rbegin(); it1 != turns.rend(); ++it1) {

		const Solution sol = (*it1).getSolution();

		// Fill the guesses
		std::cout << "│";
		for (size_t i = 0; i < 4; ++i) {
			if (i != 0)
				std::cout << "│";
			std::cout << " " << sol[i].getShortColorString() << " ";
		}

		// And the scores
		Result r = (*it1).getResult();
		std::cout << "│ │ " << std::to_string(r.getCorrect()) << " │ " << std::to_string(r.getAlmost()) << " │" << std::endl;
	}
	std::cout << "╰───┴───┴───┴───╯ ╰───┴───╯" << std::endl;
}
