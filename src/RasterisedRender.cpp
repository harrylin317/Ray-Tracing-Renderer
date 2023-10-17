#include <CanvasPoint.h>
#include <glm/glm.hpp>
#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <TextureMap.h>
#include <vector>
#include <map>

#include "RasterisedRender.h"
#include "generalFunctions.h"

CanvasPoint getCanvasIntersectionPoint(glm::vec3& cameraPosition, glm::vec3& vertexPosition, float &focalLength, DrawingWindow& window, glm::mat3& cameraOrientation, Canvas& canvas) {
	//glm::vec3 cameraToVertex = (cameraPosition - vertexPosition) * cameraOrientation;
	float scale = 200;
	glm::vec3 cameraToVertex = (vertexPosition - cameraPosition) * cameraOrientation;
	CanvasPoint canvasPoint;
	//canvasPoint.x = (focalLength * (-cameraToVertex.x / cameraToVertex.z)) + (canvas.width / 2.0);
	//canvasPoint.y = (focalLength * (cameraToVertex.y / cameraToVertex.z)) + (canvas.height / 2.0);
	canvasPoint.x = ((scale * focalLength * (-cameraToVertex.x / cameraToVertex.z)) + (canvas.width / 2.0)) ;
	canvasPoint.y = ((scale * focalLength * (cameraToVertex.y / cameraToVertex.z)) + (canvas.height / 2.0)) ;
	canvasPoint.depth = -cameraToVertex.z;
	
	return canvasPoint;

}

bool checkTriangleTooClose(ModelTriangle triangle, glm::vec3& cameraPosition) {
	float nearestVerticeDistance = std::numeric_limits<float>::max();
	for (auto vertice : triangle.vertices) {
		float dist = glm::distance(cameraPosition, vertice);
		if (dist < nearestVerticeDistance) {
			nearestVerticeDistance = dist;
		}
	}

	if (nearestVerticeDistance < 0.5) {
		return true;
	}
	else return false;
}

void drawWireframe(std::vector<ModelTriangle>& triangles, DrawingWindow& window, glm::vec3& cameraPosition, float &focalLength, glm::mat3& cameraOrientation, Canvas& canvas) {
	for (ModelTriangle triangle : triangles) {
		//if (checkTriangleTooClose(triangle, cameraPosition)) continue;
		CanvasPoint v1 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[0], focalLength, window, cameraOrientation, canvas);
		CanvasPoint v2 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[1], focalLength, window, cameraOrientation, canvas);
		CanvasPoint v3 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[2], focalLength, window, cameraOrientation, canvas);
		CanvasTriangle newTriangle = CanvasTriangle(v1, v2, v3);
		Colour colour = Colour(1, 1, 1);
		drawStrokedTriangles(newTriangle, colour, window, canvas);
	}
}

void drawRasterisedRender(std::vector<ModelTriangle>& triangles, DrawingWindow& window, glm::vec3& cameraPosition, float &focalLength, std::map<std::string, TextureMap>& textureMaps, glm::mat3& cameraOrientation, Canvas& canvas) {
	for (ModelTriangle triangle : triangles) {
		if (checkTriangleTooClose(triangle, cameraPosition)) continue;
		CanvasPoint v1 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[0], focalLength, window, cameraOrientation, canvas);
		CanvasPoint v2 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[1], focalLength, window, cameraOrientation, canvas);
		CanvasPoint v3 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[2], focalLength, window, cameraOrientation, canvas);
		CanvasTriangle newTriangle = CanvasTriangle(v1, v2, v3);
		if (triangle.hasTexture) {
			TextureMap textureMap = textureMaps[triangle.textureKey];
			
			//drawTexturedTriangle(newTriangle, triangle.texturePoints[0], triangle.texturePoints[1], triangle.texturePoints[2], textureMap, window, canvas);
			drawTexturedCorrectTriangle(newTriangle, triangle.texturePoints[0], triangle.texturePoints[1], triangle.texturePoints[2], textureMap, window, canvas);

		}
		else drawFilledTriangles(newTriangle, triangle.colour, window, canvas);
	}


}
