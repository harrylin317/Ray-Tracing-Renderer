#ifndef GENERAL_FUNCTION_H
#define GENERAL_FUNCTION_H

#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <TexturePoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include "Renderer.h"

struct Canvas
{
	int width;
	int height;
};

float calculateDistance(float& x1, float& x2, float& y1, float& y2);

std::vector<CanvasPoint> interpolateTwoPointsCanvas(CanvasPoint from, CanvasPoint to, float numberOfSteps);
std::vector<TexturePoint> interpolateTwoPointsTexture(TexturePoint from, TexturePoint to, float numberOfSteps);
std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues);
std::vector<CanvasPoint> interpolateTwoPointsCanvasAndTexture(CanvasPoint from, CanvasPoint to, float numberOfSteps);

std::vector<CanvasPoint> sortTrianglePoints(CanvasTriangle& triangle);
void drawStrokedTriangles(CanvasTriangle& triangle, Colour& colour, DrawingWindow& window, Canvas& canvas);
void drawFilledTriangles(CanvasTriangle& triangle, Colour& colour, DrawingWindow& window, Canvas& canvas);
void drawTexturedTriangle(CanvasTriangle& canvasTriangle, TexturePoint& t0, TexturePoint& t1, TexturePoint& t2, TextureMap& image, DrawingWindow& window, Canvas& canvas);
void drawTexturedCorrectTriangle(CanvasTriangle& canvasTriangle, TexturePoint& t0, TexturePoint& t1, TexturePoint& t2, TextureMap& image, DrawingWindow& window, Canvas& canvas);

void drawLine(CanvasPoint& from, CanvasPoint& to, Colour& colour, DrawingWindow& window, Canvas& canvas);
void drawTexturedLine(CanvasPoint& from, CanvasPoint& to, TexturePoint& textureFrom, TexturePoint& textureTo, TextureMap& image, DrawingWindow& window, Canvas& canvas);
void drawPerspectiveCorrectTexturedLine(CanvasPoint& from, CanvasPoint& to, TextureMap& image, DrawingWindow& window, Canvas& canvas);

CanvasPoint findExtraPointCanvas(CanvasTriangle& triangle);
TexturePoint findExtraPointTexture(CanvasTriangle& triangle, CanvasPoint& canvasExtraPoint);
TexturePoint findPerspectiveCorrectTexturePoint(CanvasPoint from, CanvasPoint to, CanvasPoint canvasPoint);

void calculateNormal(std::vector<ModelTriangle>& triangles);
void calculateVertexNormal(std::vector<ModelTriangle>& triangles);
void findObjectOrigin(std::vector<ModelTriangle>& object, std::string objectName);
std::vector<glm::vec3> createLightSources(glm::vec3 center);


#endif