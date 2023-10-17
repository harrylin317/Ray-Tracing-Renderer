#ifndef RASTERISED_RENDER_H
#define RASTERISED_RENDER_H

#include <glm/glm.hpp>
#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include <vector>
#include "generalFunctions.h"

CanvasPoint getCanvasIntersectionPoint(glm::vec3& cameraPosition, glm::vec3& vertexPosition, float &focalLength, DrawingWindow& window, glm::mat3& cameraOrientation, Canvas& canvas);
void drawWireframe(std::vector<ModelTriangle>& triangles, DrawingWindow& window, glm::vec3& cameraPosition, float &focalLength, glm::mat3& cameraOrientation, Canvas& canvas);
void drawRasterisedRender(std::vector<ModelTriangle>& triangles, DrawingWindow& window, glm::vec3& cameraPosition, float &focalLength, std::map<std::string, TextureMap>& textureMaps, glm::mat3& cameraOrientation, Canvas& canvas);


#endif