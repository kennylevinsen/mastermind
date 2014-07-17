#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <list>
#include <string>
#include <iostream>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>

//
// Gem
//
enum GemColor { none, red, blue, yellow, green, orange, purple };
const char GemColorTable[][7] = { " ", "red", "blue", "yellow", "green", "orange", "purple" };

class Gem {
public:
	Gem() : _color(none) {}
	// Gem(Gem &g) : _color(none) { _color = g.getColor(); }
	Gem(int color) : _color(GemColor(color)) {}
	Gem(GemColor color) : _color(color) {}
	GemColor getColor() const { return _color; }
	void nextColor() { if ((_color = static_cast<GemColor>(_color+1)) > 6) _color = static_cast<GemColor>(1); }
	void prevColor() { if ((_color = static_cast<GemColor>(_color-1)) < 1) _color = static_cast<GemColor>(6); }
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

	int getCorrect() const { return _correct; }
	int getAlmost() const { return _almost; }

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

	// std::array<Gem, 4> getPieces() const { return pieces; }
	bool operator==(Solution other) const;
	Gem operator[](size_t index) const { return pieces.operator[](index); }
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

//
// Game board stuff
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

// class Cursor {
// public:
// 	Cursor(size_t col, Gem selection) : _col(col), _selection(selection) {}
// 	const size_t getCol() const { return _col; }
// 	Gem getPiece() const { return _selection; }
// private:
// 	size_t _col;
// 	Gem _selection;
// };

class Gameboard {
public:
	Gameboard(Solution code, size_t rows) : _code(code), _rows(rows), solved(false), end(false) { init(); }
	Gameboard(size_t rows) : _code(Solution()), _rows(rows), solved(false), end(false) { init(); }
	Gameboard() : _code(Solution()), _rows(10), solved(false), end(false) { init(); }
	~Gameboard() { reset(); }
	void init();
	void reset();
	void addTurn(Turn t);
	void addTurn(Solution s) { Result r = _code.test(s); addTurn(s, r); }
	void addTurn(Solution s, Result r) { addTurn(Turn(s, r)); }
	void printBoard(Solution cursor) const;
private:
	Solution _code;
	std::list<Turn> turns;
	size_t _rows;
	bool solved;
	bool end;
	struct termios saved_attributes;
};

void Gameboard::init()
{
	struct termios tattr;
	char *name;
	if (!isatty(STDIN_FILENO)) {
		fprintf(stderr, "Not connected to a terminal!\n");
		exit(-1);
	}

	tcgetattr(STDIN_FILENO, &saved_attributes);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO);
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

void Gameboard::reset()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
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
 * Prints the board
 *
 * @param Cursor cursor The current cursor
 */
void Gameboard::printBoard(Solution cursor) const
{
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
				std::cout << "│ " << cursor[y].getShortColorString() << " ";
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

//
// Game flow
//
class Game {
public:
	Game() : cursor(Solution(none, none, none, none)), selectionColumn(0), selection(red) {}
	int run();
private:
	void update();
	void submit();
	void nextColumn();
	void prevColumn();
	void nextColor();
	void prevColor();
	Gameboard board;
	Solution cursor;
	int selectionColumn;
	Gem selection;
};

/*
 * Updates the selection and prints the board
 */
void Game::update()
{
	cursor[static_cast<size_t>(selectionColumn)] = selection;
	selection = Gem(selection.getColor());
	board.printBoard(cursor);

}

/*
 * Submits the current selection
 */
void Game::submit()
{
	board.addTurn(cursor);
	cursor = Solution(none, none, none, none);
}

/*
 * Selects the next column
 */
void Game::nextColumn()
{
	if (++selectionColumn > 3)
		selectionColumn = 3;
}

/*
 * Selects the previous column
 */
void Game::prevColumn()
{
	if (--selectionColumn < 0)
		selectionColumn = 0;
}

/*
 * Selects the next color
 */
void Game::nextColor()
{
	selection.nextColor();
}

/*
 * Selects the previous color
 */
void Game::prevColor()
{
	selection.prevColor();
}

/*
 * The loop
 */
int Game::run()
{
	char c;
	update();
	while (true) {
		read(STDIN_FILENO, &c, 1);
		if (c == 'a')
			prevColumn();
		else if (c == 'd')
			nextColumn();
		else if (c == 's')
			prevColor();
		else if (c == 'w')
			nextColor();
		else if (c == 'q')
			break;
		else
			continue;

		update();

	}

	return 0;
}

//
// main
//
int main() { return Game().run(); }
