#include "generalFunctions.h"
#include <ModelTriangle.h>
#define _USE_MATH_DEFINES
#include <math.h>
float calculateDistance(float& x1, float& x2, float& y1, float& y2) {
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}


std::vector<CanvasPoint> interpolateTwoPointsCanvasAndTexture(CanvasPoint from, CanvasPoint to, float numberOfSteps) {
	std::vector<CanvasPoint> interpolatedPoints;
	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float zDiff = to.depth - from.depth;
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	float zStepSize = zDiff / numberOfSteps;

	for (float i = 0.0; i < numberOfSteps; i++) {
		float x = from.x + (xStepSize * i);
		float y = from.y + (yStepSize * i);
		float z = from.depth + (zStepSize * i);
		CanvasPoint point{ x,y,z };
		point.texturePoint = findPerspectiveCorrectTexturePoint(from, to, point);
		interpolatedPoints.push_back(point);

		/*if (from.depth != NULL && to.depth != NULL) {
			float z = from.depth + (zStepSize * i);
			interpolatedPoints.push_back(CanvasPoint(x, y, z));
		}
		else {
			interpolatedPoints.push_back(CanvasPoint(x, y));
		}*/
	}
	return interpolatedPoints;
}

std::vector<CanvasPoint> interpolateTwoPointsCanvas(CanvasPoint from, CanvasPoint to, float numberOfSteps) {
	std::vector<CanvasPoint> interpolatedPoints;
	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float zDiff;
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	float zStepSize;
	
	zDiff = to.depth - from.depth;
	zStepSize = zDiff / numberOfSteps;
	
	for (float i = 0.0; i < numberOfSteps; i++) {
		float x = from.x + (xStepSize * i);
		float y = from.y + (yStepSize * i);
		
		float z = from.depth + (zStepSize * i);
		interpolatedPoints.push_back(CanvasPoint(x, y, z));
		
	}
	return interpolatedPoints;
}

std::vector<TexturePoint> interpolateTwoPointsTexture(TexturePoint from, TexturePoint to, float numberOfSteps) {
	std::vector<TexturePoint> interpolatedPoints;
	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	for (float i = 0.0; i < numberOfSteps; i++) {
		float x = from.x + (xStepSize * i);
		float y = from.y + (yStepSize * i);
		interpolatedPoints.push_back(TexturePoint(x, y));
	}
	return interpolatedPoints;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues) {
	std::vector<glm::vec3> v;
	float divValue = (1.0 / (numberOfValues - 1.0));
	glm::vec3 step = (to - from) * divValue;
	glm::vec3 currentValue = from;
	for (int loopTimes = 0; loopTimes < numberOfValues; loopTimes++) {
		v.push_back(currentValue);
		currentValue += step;
	}
	return v;

}


std::vector<CanvasPoint> sortTrianglePoints(CanvasTriangle& triangle) {
	std::vector<CanvasPoint> sortedTriangleVector{ triangle.v0(), triangle.v1(), triangle.v2() };
	if (sortedTriangleVector[1].y < sortedTriangleVector[0].y) std::swap(sortedTriangleVector[0], sortedTriangleVector[1]);
	if (sortedTriangleVector[2].y < sortedTriangleVector[1].y) std::swap(sortedTriangleVector[1], sortedTriangleVector[2]);
	if (sortedTriangleVector[1].y < sortedTriangleVector[0].y) std::swap(sortedTriangleVector[0], sortedTriangleVector[1]);

	if (sortedTriangleVector[0].y == sortedTriangleVector[1].y) {
		if (sortedTriangleVector[1].x < sortedTriangleVector[0].x) std::swap(sortedTriangleVector[0], sortedTriangleVector[1]);

	}
	else if (sortedTriangleVector[1].y == sortedTriangleVector[2].y) {
		if (sortedTriangleVector[2].x < sortedTriangleVector[1].x) std::swap(sortedTriangleVector[1], sortedTriangleVector[2]);
	}

	/*for (int i = 0; i < (3 - 1); i++) {
		int min_index = i;
		for (int j = i + 1; j < 3; j++) {
			if (sortedTriangleVector[j].y < sortedTriangleVector[min_index].y) {
				min_index = j;
			}
		}
		std::swap(sortedTriangleVector[min_index], sortedTriangleVector[i]);
	}*/
	return sortedTriangleVector;
}

TexturePoint findPerspectiveCorrectTexturePoint(CanvasPoint from, CanvasPoint to, CanvasPoint canvasPoint) {
	float texturePointX, texturePointY;
	bool calculateX = true;
	bool calculateY = true;
	CanvasPoint furthestVertex = from;
	CanvasPoint closestVertex = to;

	if (from.x == canvasPoint.x && from.y == canvasPoint.y) {
		return from.texturePoint;
	} else if (to.x == canvasPoint.x && to.y == canvasPoint.y) {
		return to.texturePoint;
	} else if (from.x == canvasPoint.x) {
		texturePointX = from.texturePoint.x;
		calculateX = false;
	} else if (from.y == canvasPoint.y) {
		texturePointY = from.texturePoint.y;
		calculateY = false;
	} else if (to.x == canvasPoint.x) {
		texturePointX = to.texturePoint.x;
		calculateX = false;
	} else if (to.y == canvasPoint.y) {
		texturePointY = to.texturePoint.y;
		calculateY = false;
	}

	float z0 = furthestVertex.depth;
	float z1 = closestVertex.depth;
	if (calculateX) {
		float c0 = furthestVertex.texturePoint.x;
		float c1 = closestVertex.texturePoint.x;
		float q = abs(from.x - canvasPoint.x) / abs(from.x - to.x);
		texturePointX = (((c0 / z0) * (1 - q)) + (c1 * q / z1)) / (((1 / z0) * (1 - q)) + (q / z1));
	}

	if (calculateY) {
		float c0 = furthestVertex.texturePoint.y;
		float c1 = closestVertex.texturePoint.y;
		float q = abs(from.y - canvasPoint.y) / abs(from.y - to.y);
		texturePointY = (((c0 / z0) * (1 - q)) + (c1 * q / z1)) / (((1 / z0) * (1 - q)) + (q / z1));
	}

	/*if (isnan(texturePointX) || isnan(texturePointY)) {
		std::cout << std::endl;
	}
	if (texturePointX> 1 || texturePointY > 1) {
		std::cout << std::endl;
	}*/
	return TexturePoint{ texturePointX,texturePointY };
}

void drawStrokedTriangles(CanvasTriangle& triangle, Colour& colour, DrawingWindow& window, Canvas& canvas) {
	drawLine(triangle.vertices[0], triangle.vertices[1], colour, window, canvas);
	drawLine(triangle.vertices[1], triangle.vertices[2], colour, window, canvas);
	drawLine(triangle.vertices[2], triangle.vertices[0], colour, window, canvas);
}

void drawFilledTriangles(CanvasTriangle& triangle, Colour& colour, DrawingWindow& window, Canvas& canvas) {
	std::vector<CanvasPoint> sortedTriangleVector = sortTrianglePoints(triangle);
	triangle.v0() = sortedTriangleVector[0];
	triangle.v1() = sortedTriangleVector[1];
	triangle.v2() = sortedTriangleVector[2];
	CanvasPoint extrapoint = findExtraPointCanvas(triangle);


	//find interpolated vertices between (v0,v1) and (v0,extrapoint)
	float distance_v0_v1 = calculateDistance(triangle.v0().x, triangle.v1().x, triangle.v0().y, triangle.v1().y);
	float distance_v0_extraPoint = calculateDistance(triangle.v0().x, extrapoint.x, triangle.v0().y, extrapoint.y);
	float numberOfSteps = std::max(abs(distance_v0_v1), abs(distance_v0_extraPoint));
	std::vector<CanvasPoint> interpolate_v0_v1 = interpolateTwoPointsCanvas(triangle.v0(), triangle.v1(), numberOfSteps);
	std::vector<CanvasPoint> interpolate_v0_extraPoint = interpolateTwoPointsCanvas(triangle.v0(), extrapoint, numberOfSteps);

	//fill top triangle with set colour
	for (float i = 0; i < numberOfSteps; i++) {
		drawLine(interpolate_v0_v1[i], interpolate_v0_extraPoint[i], colour, window, canvas);
	}

	//find interpolated vertices between (v1,v2) and (v1,extrapoint)
	float distance_v1_v2 = calculateDistance(triangle.v1().x, triangle.v2().x, triangle.v1().y, triangle.v2().y);
	float distance_extraPoint_v2 = calculateDistance(triangle.v2().x, extrapoint.x, triangle.v2().y, extrapoint.y);
	numberOfSteps = std::max(abs(distance_v1_v2), abs(distance_extraPoint_v2));
	std::vector<CanvasPoint> interpolate_v1_v2 = interpolateTwoPointsCanvas(triangle.v1(), triangle.v2(), numberOfSteps);
	std::vector<CanvasPoint> interpolate_extraPoint_v2 = interpolateTwoPointsCanvas(extrapoint, triangle.v2(), numberOfSteps);

	//fil bottom triangle with set colour
	for (float i = 0; i < numberOfSteps; i++) {
		drawLine(interpolate_v1_v2[i], interpolate_extraPoint_v2[i], colour, window, canvas);
	}
	/*Colour borderColour{ 1,1,1 };
	drawStrokedTriangles(triangle, borderColour, window, canvas);*/
	drawStrokedTriangles(triangle, colour, window, canvas);
}

void drawTexturedTriangle(CanvasTriangle& canvasTriangle, TexturePoint& t0, TexturePoint& t1, TexturePoint& t2, TextureMap& image, DrawingWindow& window, Canvas& canvas) {
	t0 = TexturePoint(t0.x * image.width, t0.y * image.height);
	t1 = TexturePoint(t1.x * image.width, t1.y * image.height);
	t2 = TexturePoint(t2.x * image.width, t2.y * image.height);
	canvasTriangle.v0().texturePoint = t0;
	canvasTriangle.v1().texturePoint = t1;
	canvasTriangle.v2().texturePoint = t2;
	std::vector<CanvasPoint> sortedTriangleVector = sortTrianglePoints(canvasTriangle);
	canvasTriangle.v0() = sortedTriangleVector[0];
	canvasTriangle.v1() = sortedTriangleVector[1];
	canvasTriangle.v2() = sortedTriangleVector[2];
	CanvasPoint canvasExtraPoint = findExtraPointCanvas(canvasTriangle);
	TexturePoint textureExtraPoint = findExtraPointTexture(canvasTriangle, canvasExtraPoint);

	float distance_v0_v1 = calculateDistance(canvasTriangle.v0().x, canvasTriangle.v1().x, canvasTriangle.v0().y, canvasTriangle.v1().y);
	float distance_v0_extraPoint = calculateDistance(canvasTriangle.v0().x, canvasTriangle.v0().y, canvasExtraPoint.x, canvasExtraPoint.y);
	float numberOfSteps = std::max(abs(distance_v0_v1), abs(distance_v0_extraPoint));
	std::vector<CanvasPoint> interpolate_v0_v1 = interpolateTwoPointsCanvas(canvasTriangle.v0(), canvasTriangle.v1(), numberOfSteps);
	std::vector<CanvasPoint> interpolate_v0_extraPoint = interpolateTwoPointsCanvas(canvasTriangle.v0(), canvasExtraPoint, numberOfSteps);
	std::vector<TexturePoint> interpolate_t0_t1 = interpolateTwoPointsTexture(canvasTriangle.v0().texturePoint, canvasTriangle.v1().texturePoint, numberOfSteps);
	std::vector<TexturePoint> interpolate_t0_extraPoint = interpolateTwoPointsTexture(canvasTriangle.v0().texturePoint, textureExtraPoint, numberOfSteps);
	for (float i = 0; i < numberOfSteps; i++) {
		drawTexturedLine(interpolate_v0_v1[i], interpolate_v0_extraPoint[i], interpolate_t0_t1[i], interpolate_t0_extraPoint[i], image, window, canvas);
	}

	float distance_v1_v2 = calculateDistance(canvasTriangle.v1().x, canvasTriangle.v2().x, canvasTriangle.v1().y, canvasTriangle.v2().y);
	float distance_extraPoint_v2 = calculateDistance(canvasExtraPoint.x, canvasTriangle.v2().y, canvasExtraPoint.y, canvasTriangle.v2().y);
	numberOfSteps = std::max(abs(distance_v1_v2), abs(distance_extraPoint_v2));
	std::vector<CanvasPoint> interpolate_v1_v2 = interpolateTwoPointsCanvas(canvasTriangle.v1(), canvasTriangle.v2(), numberOfSteps);
	std::vector<CanvasPoint> interpolate_extraPoint_v2 = interpolateTwoPointsCanvas(canvasExtraPoint, canvasTriangle.v2(), numberOfSteps);
	std::vector<TexturePoint> interpolate_t1_t2 = interpolateTwoPointsTexture(canvasTriangle.v1().texturePoint, canvasTriangle.v2().texturePoint, numberOfSteps);
	std::vector<TexturePoint> interpolate_extraPoint_t2 = interpolateTwoPointsTexture(textureExtraPoint, canvasTriangle.v2().texturePoint, numberOfSteps);
	for (float i = 0; i < numberOfSteps; i++) {
		drawTexturedLine(interpolate_v1_v2[i], interpolate_extraPoint_v2[i], interpolate_t1_t2[i], interpolate_extraPoint_t2[i], image, window, canvas);
	}
	drawStrokedTriangles(canvasTriangle, Colour(1, 1, 1), window, canvas);


}

void drawTexturedCorrectTriangle(CanvasTriangle& canvasTriangle, TexturePoint& t0, TexturePoint& t1, TexturePoint& t2, TextureMap& image, DrawingWindow& window, Canvas& canvas) {
	/*t0 = TexturePoint(t0.x * image.width, t0.y * image.height);
	t1 = TexturePoint(t1.x * image.width, t1.y * image.height);
	t2 = TexturePoint(t2.x * image.width, t2.y * image.height);*/
	canvasTriangle.v0().texturePoint = t0;
	canvasTriangle.v1().texturePoint = t1;
	canvasTriangle.v2().texturePoint = t2;
	std::vector<CanvasPoint> sortedTriangleVector = sortTrianglePoints(canvasTriangle);
	canvasTriangle.v0() = sortedTriangleVector[0];
	canvasTriangle.v1() = sortedTriangleVector[1];
	canvasTriangle.v2() = sortedTriangleVector[2];
	CanvasPoint canvasExtraPoint = findExtraPointCanvas(canvasTriangle);
	canvasExtraPoint.texturePoint = findPerspectiveCorrectTexturePoint(canvasTriangle.v0(), canvasTriangle.v2(), canvasExtraPoint);
	//Top triangle
	float distance_v0_v1 = calculateDistance(canvasTriangle.v0().x, canvasTriangle.v1().x, canvasTriangle.v0().y, canvasTriangle.v1().y);
	float distance_v0_extraPoint = calculateDistance(canvasTriangle.v0().x, canvasTriangle.v0().y, canvasExtraPoint.x, canvasExtraPoint.y);
	float numberOfSteps = std::max(abs(distance_v0_v1), abs(distance_v0_extraPoint));
	//float numberOfSteps =std::max(abs(canvasTriangle.v0().y - canvasTriangle.v1().y),abs(canvasTriangle.v0().y-canvasExtraPoint.y));
	std::vector<CanvasPoint> interpolate_v0_v1 = interpolateTwoPointsCanvasAndTexture(canvasTriangle.v0(), canvasTriangle.v1(), numberOfSteps);
	std::vector<CanvasPoint> interpolate_v0_extraPoint = interpolateTwoPointsCanvasAndTexture(canvasTriangle.v0(), canvasExtraPoint, numberOfSteps);

	/*for (float i = 0; i < numberOfSteps; i++) {
		TexturePoint texturePoint = findPerspectiveCorrectTexturePoint(canvasTriangle.v0(), canvasTriangle.v1(), interpolate_v0_v1[i]);
		interpolate_v0_v1[i].texturePoint = texturePoint;
	}

	for (float i = 0; i < numberOfSteps; i++) {
		TexturePoint texturePoint = findPerspectiveCorrectTexturePoint(canvasTriangle.v0(), canvasExtraPoint, interpolate_v0_extraPoint[i]);
		interpolate_v0_extraPoint[i].texturePoint = texturePoint;
	}*/


	for (float i = 0; i < numberOfSteps; i++) {
		drawPerspectiveCorrectTexturedLine(interpolate_v0_v1[i], interpolate_v0_extraPoint[i], image, window, canvas);
	}

	//bottom triangle
	float distance_v1_v2 = calculateDistance(canvasTriangle.v1().x, canvasTriangle.v2().x, canvasTriangle.v1().y, canvasTriangle.v2().y);
	float distance_extraPoint_v2 = calculateDistance(canvasExtraPoint.x, canvasTriangle.v2().y, canvasExtraPoint.y, canvasTriangle.v2().y);
	numberOfSteps = std::max(abs(distance_v1_v2), abs(distance_extraPoint_v2));
	//numberOfSteps = std::max(abs(canvasTriangle.v1().y - canvasTriangle.v2().y), abs(canvasTriangle.v2().y - canvasExtraPoint.y));

	std::vector<CanvasPoint> interpolate_v1_v2 = interpolateTwoPointsCanvasAndTexture(canvasTriangle.v1(), canvasTriangle.v2(), numberOfSteps);
	std::vector<CanvasPoint> interpolate_extraPoint_v2 = interpolateTwoPointsCanvasAndTexture(canvasExtraPoint, canvasTriangle.v2(), numberOfSteps);

	/*for (float i = 0; i < numberOfSteps; i++) {
		TexturePoint texturePoint = findPerspectiveCorrectTexturePoint(canvasTriangle.v1(), canvasTriangle.v2(), interpolate_v1_v2[i]);
		interpolate_v1_v2[i].texturePoint = texturePoint;
	}

	for (float i = 0; i < numberOfSteps; i++) {
		TexturePoint texturePoint = findPerspectiveCorrectTexturePoint(canvasExtraPoint, canvasTriangle.v2(), interpolate_extraPoint_v2[i]);
		interpolate_extraPoint_v2[i].texturePoint = texturePoint;
	}*/


	for (float i = 0; i < numberOfSteps; i++) {
		drawPerspectiveCorrectTexturedLine(interpolate_v1_v2[i], interpolate_extraPoint_v2[i], image, window, canvas);
	}

}

void drawLine(CanvasPoint& from, CanvasPoint& to, Colour& colour, DrawingWindow& window, Canvas& canvas) {
	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float zDiff;
	float numberOfSteps = std::max(abs(xDiff), abs(yDiff));
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	float zStepSize;

	
	zDiff = to.depth - from.depth;
	zStepSize = zDiff / numberOfSteps;
	
	for (float i = 0.0; i < numberOfSteps; i++) {
		int x = static_cast<int>((from.x + (xStepSize * i)) + 0.5f);
		int y = static_cast<int>((from.y + (yStepSize * i)) + 0.5f);
		if (x >= canvas.width || x < 0 || y >= canvas.height || y < 0) {
			continue;
		}
		
		float z = from.depth + (zStepSize * i);
		if (!check_depth_buffer(x, y, z)) {
			continue;
		}
		

		uint32_t lineColour = (255 << 24) + ((int)(255 * (colour.red)) << 16) + ((int)(255 * (colour.green)) << 8) + (int)(255 * (colour.blue));
		window.setPixelColour(x, y, lineColour);
	}
}

void drawTexturedLine(CanvasPoint& from, CanvasPoint& to, TexturePoint& textureFrom, TexturePoint& textureTo, TextureMap& image, DrawingWindow& window, Canvas& canvas) {
	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float zDiff;

	float numberOfSteps = std::max(abs(xDiff), abs(yDiff));
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	float zStepSize;

	
	zDiff = to.depth - from.depth;
	zStepSize = zDiff / numberOfSteps;
	
	float xTextureDiff = textureTo.x - textureFrom.x;
	float yTextureDiff = textureTo.y - textureFrom.y;
	/*if (isnan(xTextureDiff) || isnan(yTextureDiff)) {
		std::cout << std::endl;
	}*/
	float xTextureStepSize = xTextureDiff / numberOfSteps;
	float yTextureStepSize = yTextureDiff / numberOfSteps;
	for (float i = 0.0; i < numberOfSteps; i++) {
		int x = static_cast<int>((from.x + (xStepSize * i)) + 0.5f);
		int y = static_cast<int>((from.y + (yStepSize * i)) + 0.5f);
		int textureX = static_cast<int>(textureFrom.x + (xTextureStepSize * i) + 0.5f);
		int textureY = static_cast<int>(textureFrom.y + (yTextureStepSize * i) + 0.5f);
		int textureIndex = (textureY * image.width) + textureX;
		if (x >= canvas.width || x < 0 || y >= canvas.height || y < 0) {
			continue;
		}
		
		float z = from.depth + (zStepSize * i);
		if (!check_depth_buffer(x, y, z)) {
			continue;
		}
		
		uint32_t colour = image.pixels[textureIndex];
		window.setPixelColour(x, y, colour);
	}
}


void drawPerspectiveCorrectTexturedLine(CanvasPoint& from, CanvasPoint& to, TextureMap& image, DrawingWindow& window, Canvas& canvas) {
	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float numberOfSteps = std::max(abs(xDiff), abs(yDiff));
	std::vector<CanvasPoint>fromToInterpolate = interpolateTwoPointsCanvasAndTexture(from, to, numberOfSteps);

	for (float i = 0.0; i < numberOfSteps; i++) {
		CanvasPoint canvasPoint = fromToInterpolate[i];
		TexturePoint texturePoint = fromToInterpolate[i].texturePoint;
		texturePoint = TexturePoint(texturePoint.x * image.width, texturePoint.y * image.height);
		int textureIndex = ((int)texturePoint.y * image.width) + (int)texturePoint.x;

		if (canvasPoint.x >= canvas.width || canvasPoint.x < 0 || canvasPoint.y >= canvas.height || canvasPoint.y < 0) {
			continue;
		}

		if (!check_depth_buffer(canvasPoint.x, canvasPoint.y, canvasPoint.depth)) {
			continue;
		}
		uint32_t colour = image.pixels[textureIndex];
		window.setPixelColour((int)canvasPoint.x, (int)canvasPoint.y, colour);
	}
}

CanvasPoint findExtraPointCanvas(CanvasTriangle& triangle) {
	float xRatio = triangle.v0().x - triangle.v2().x;
	float yRatio = triangle.v0().y - triangle.v2().y;
	float zRatio = triangle.v0().depth - triangle.v2().depth;
	float yRatioExtraPointFromFirstPoint = triangle.v0().y - triangle.v1().y;
	float xRatioExtraPointFromFirstPoint = (yRatioExtraPointFromFirstPoint * xRatio) / yRatio;
	float yExtraPoint = triangle.v1().y;
	float xExtraPoint = triangle.v0().x - xRatioExtraPointFromFirstPoint;
	float zRationExtraPointFromFirstPoint = ((triangle.v0().y - yExtraPoint) * zRatio) / yRatio;
	float zExtraPoint = triangle.v0().depth - zRationExtraPointFromFirstPoint;
	return CanvasPoint(xExtraPoint, yExtraPoint, zExtraPoint);
}


TexturePoint findExtraPointTexture(CanvasTriangle& triangle, CanvasPoint& canvasExtraPoint) {
	float canvasDistance_v0_v2 = calculateDistance(triangle.v0().x, triangle.v2().x, triangle.v0().y, triangle.v2().y);
	float canvasDistance_v0_extra = calculateDistance(triangle.v0().x, canvasExtraPoint.x, triangle.v0().y, canvasExtraPoint.y);
	float textureDistance_t0_t2 = calculateDistance(triangle.v0().texturePoint.x, triangle.v2().texturePoint.x, triangle.v0().texturePoint.y, triangle.v2().texturePoint.y);
	float textureDistance_t0_extra = (canvasDistance_v0_extra * textureDistance_t0_t2) / canvasDistance_v0_v2;
	float xTextureExtraPoint = triangle.v0().texturePoint.x + ((textureDistance_t0_extra / textureDistance_t0_t2) * (triangle.v2().texturePoint.x - triangle.v0().texturePoint.x));
	float yTextureExtraPoint = triangle.v0().texturePoint.y + ((textureDistance_t0_extra / textureDistance_t0_t2) * (triangle.v2().texturePoint.y - triangle.v0().texturePoint.y));
	return TexturePoint(xTextureExtraPoint, yTextureExtraPoint);
}

void calculateNormal(std::vector<ModelTriangle>& triangles) {
	for (int i = 0; i < triangles.size(); i++) {
		glm::vec3 normal = glm::cross((triangles[i].vertices[1] - triangles[i].vertices[0]), (triangles[i].vertices[2] - triangles[i].vertices[0]));
		triangles[i].normal = normal;
		//triangles[i].vertexNormals = { normal, normal, normal };
	}
}

void calculateVertexNormal(std::vector<ModelTriangle>& triangles) {
	//choose each triangle
	for (int i = 0; i < triangles.size(); i++) {
		//loop through all 3 vertex
		for (int j = 0; j < 3; j++) {
			int sameVertexCount = 1;
			glm::vec3 sumNormals = triangles[i].normal;
			//loop through all other triangles
			for (int k = 0; k < triangles.size(); k++) {
				if (k == i) {
					continue;
				}
				//loop through all their vertex
				for (int l = 0; l < 3; l++) {
					if (triangles[i].vertices[j] == triangles[k].vertices[l]) {
						//dont calculate sphere normals
						if (triangles[i].obj_name != "sphere") {
							continue;
						}
						else {
							sumNormals += triangles[k].normal;
							sameVertexCount += 1;
						}

					}
				}
			}
			//calculate average
			glm::vec3 averagedNormal = sumNormals * (1.0f / sameVertexCount);
			triangles[i].vertexNormals[j] = averagedNormal;

		}
	}
}
void findObjectOrigin(std::vector<ModelTriangle>& object, std::string objectName) {
	glm::vec3 sumVertices = glm::vec3(0);
	float count = 0;
	for (int i = 0; i < object.size(); i++) {
		if (object[i].obj_name.compare(objectName) == 0) {
			for (int j = 0; j < 3; j++) {
				sumVertices += object[i].vertices[j];
				count++;
			}
		}

	}
	glm::vec3 origin = sumVertices / count;
	for (int i = 0; i < object.size(); i++) {
		if (object[i].obj_name.compare(objectName) == 0) {
			object[i].objectOrigin = origin;
		}
	}
}
std::vector<glm::vec3> createLightSources(glm::vec3 center) {
	std::vector<glm::vec3> lightVector{ center };

	//enables multiple light sources, create lights around the center light
	/*float radius, theta, phi, x, y, z;
	for (int i = 0; i <= 360; i += 90) {
		for (int j = 0; j <= 360; j += 90) {
			radius = 0.05;
			theta = (i * (M_PI / 180));
			phi = (j * (M_PI / 180));
			x = radius * sin(theta) * cos(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(theta);
			glm::vec3 light{ x + center.x, y + center.y, z + center.z };
			lightVector.push_back(light);
		}
	}*/
	return lightVector;
}