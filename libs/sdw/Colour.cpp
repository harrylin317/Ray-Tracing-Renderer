#include "Colour.h"
#include <utility>

Colour::Colour() = default;
Colour::Colour(float r, float g, float b) : red(r), green(g), blue(b) {}
Colour::Colour(std::string n, float r, float g, float b) :
		name(std::move(n)),
		red(r), green(g), blue(b) {}

std::ostream &operator<<(std::ostream &os, const Colour &colour) {
	os << colour.name << " ["
	   << colour.red << ", "
	   << colour.green << ", "
	   << colour.blue << "]";
	return os;
}
