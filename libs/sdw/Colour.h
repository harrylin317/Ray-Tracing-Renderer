#pragma once

#include <iostream>

struct Colour {
	std::string name;
	float red{};
	float green{};
	float blue{};
	Colour();
	Colour(float r, float g, float b);
	Colour(std::string n, float r, float g, float b);
};

std::ostream &operator<<(std::ostream &os, const Colour &colour);
