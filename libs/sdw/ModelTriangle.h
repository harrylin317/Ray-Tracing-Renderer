#pragma once

#include <glm/glm.hpp>
#include <string>
#include <array>
#include <glm/glm.hpp>
#include "Colour.h"
#include "TexturePoint.h"

struct ModelTriangle {
	std::string obj_name{};
	std::string materialType{};
	std::string textureKey{};
	bool hasTexture{};
	bool hasReflection{};
	bool hasRefraction{};
	float lightReduction;

	std::array<glm::vec3, 3> vertices{};
	std::array<TexturePoint, 3> texturePoints{};
	Colour colour{};
	glm::vec3 normal{};
	std::array<glm::vec3, 3> vertexNormals{};
	std::array<Colour, 3> vertexLightings{};

	glm::vec3 objectOrigin{};

	ModelTriangle();
	ModelTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, Colour trigColour);
	friend std::ostream &operator<<(std::ostream &os, const ModelTriangle &triangle);
};
