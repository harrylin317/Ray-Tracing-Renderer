#ifndef ANIMATION_H
#define ANIMATION_H
#include <ModelTriangle.h>
#include <glm/glm.hpp>

void animation(std::vector<ModelTriangle>& objects, int& renderChoice, glm::vec3& cameraPosition, glm::mat3& cameraOrientation, bool &finish);

#endif