#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <vector>
#include <glm/glm.hpp>
#include <ModelTriangle.h>

void lookAt(glm::vec3 targetPoint, glm::vec3& cameraPosition, glm::mat3& cameraOrientation);
void rotate(float angle, char axis, glm::vec3& cameraPosition);
void translateCamera(float directionX, float directionY, float directionZ, glm::vec3& cameraPosition, glm::mat3 cameraOrientation);
void rotateCamera(float angle, char axis, glm::vec3& cameraPosition, glm::mat3& cameraOrientation);
void translateObject(ModelTriangle& object, float directionX, float directionY, float directionZ);
void rotateObject(ModelTriangle& object, char axis, float angle);

#endif