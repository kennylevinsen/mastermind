#include "game.h"

/*
 * Updates the selection and prints the board
 */
void Game::update()
{
	cursor.setPiece(selectionColumn, selection);
	board.printBoard(cursor, selectionColumn);
}

/*
 * Submits the current selection
 */
void Game::submit()
{
	board.addTurn(cursor);
	cursor = Solution(red, red, red, red);
}

/*
 * Selects the next column
 */
void Game::nextColumn()
{
	if (++selectionColumn > 3)
		selectionColumn = 3;
	selection = Gem(cursor[selectionColumn].getColor());
}

/*
 * Selects the previous column
 */
void Game::prevColumn()
{
	if (--selectionColumn == -1U)
		selectionColumn = 0;
	selection = Gem(cursor[selectionColumn].getColor());
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
	selection = Gem(cursor[selectionColumn].getColor());
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
		else if (c == '\n')
			submit();
		else if (c == 'q')
			break;
		else
			continue;

		update();

	}

	return 0;
}
