#ifndef GEM_H
#define GEM_H
#include <string>

enum GemColor { none, red, blue, yellow, green, orange, purple };
const char GemColorTable[][7] = { " ", "red", "blue", "yellow", "green", "orange", "purple" };

/*
 * The piece/"gem"
 */
class Gem {
public:
	// Constructors
	Gem() : _color(none) {}
	Gem(int color) : _color(GemColor(color)) {}
	Gem(GemColor color) : _color(color) {}

	// Utilities
	GemColor getColor() const;
	void nextColor();
	void prevColor();
	const std::string getColorString() const;
	const std::string getShortColorString() const;

	// Comparisons
	bool operator==(Gem other) const;
	bool operator!=(Gem other) const;

private:
	GemColor _color;

};

#endif /* GEM_H */
