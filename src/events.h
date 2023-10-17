#ifndef EVENTS_H
#define EVENTS_H

#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include "generalFunctions.h"
void handleEvent(SDL_Event event, DrawingWindow& window, glm::vec3& cameraPosition, glm::vec3 &lightSource, glm::mat3& cameraOrientation, bool& orbit, Canvas& canvas, int &renderChoice);

#endif