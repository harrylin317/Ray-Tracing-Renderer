#ifndef RAYTRACE_RENDER_H
#define RAYTRACE_RENDER_H

#include <vector>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>
#include <DrawingWindow.h>
#include "generalFunctions.h"
#include <map>
#include <TextureMap.h>


void drawRayTraceScene(std::vector<ModelTriangle>& triangles, DrawingWindow& window, std::map<std::string, TextureMap> &textureMaps, glm::vec3& cameraPosition, glm::mat3 &cameraOrientation, float& focalLength, Canvas& canvas, std::vector<glm::vec3>& lightSources, int renderChoice);

#endif