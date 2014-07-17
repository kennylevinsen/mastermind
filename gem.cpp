#include "gem.h"

/*
 * Retrieves the color
 *
 * @returns GemColor The color
 */
GemColor Gem::getColor() const
{
	return _color;
}

/*
 * Iterates the color forward
 *
 */
void Gem::nextColor()
{
	_color = static_cast<GemColor>(_color+1);
	if (_color > 6)
		_color = static_cast<GemColor>(1);
}

/*
 * Iterates the color backwards
 */
void Gem::prevColor()
{
	_color = static_cast<GemColor>(_color-1);
	if (_color < 1)
		_color = static_cast<GemColor>(6);
}

/*
 * Retrieves the color as a string
 *
 * @returns std::string The color as a string
 */
const std::string Gem::getColorString() const
{
	return GemColorTable[_color];
}

/*
 * Retrieves the color in the short string representation
 *
 * @returns std::string The color as a stirng
 */
const std::string Gem::getShortColorString() const
{
	return std::string(GemColorTable[_color]).substr(0, 1);
}

/*
 * Compare to other
 *
 * @params Gem other The other piece
 * @returns bool Result
 */
bool Gem::operator==(Gem other) const
{
		return other._color == _color;
}

/*
 * Compare to other
 *
 * @params Gem other The other piece
 * @returns bool Result
 */
bool Gem::operator!=(Gem other) const
{
	return !operator==(other);
}
