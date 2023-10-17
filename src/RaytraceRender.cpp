#include <glm/glm.hpp>
#include <ModelTriangle.h>
#include <vector>
#include <RayTriangleIntersection.h>
#include <DrawingWindow.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <map>
#include <TextureMap.h>

#include "generalFunctions.h"

Colour calculateReflectionColour(glm::vec3& start, RayTriangleIntersection& intersection, std::vector<ModelTriangle>& triangles, std::map<std::string, TextureMap>& textureMaps, std::vector<glm::vec3>& lightSources, float rayIndexOfRefraction, int reflectionCount, int renderChoice);
Colour calculateLighting(RayTriangleIntersection& intersection,
	std::vector<ModelTriangle>& triangles, 
	std::map<std::string, TextureMap>& textureMaps,
	std::vector<glm::vec3>& lightSources,
	glm::vec3& start,
	bool initializeVertexColour,
	int reflectionCount,
	float rayIndexOfRefraction,
	int renderChoice,
	float lightReduction);


glm::vec3 randomScatterRay(glm::vec3 reflectionRay) {
	float randomAngle = (float)rand() / (float)RAND_MAX * (M_PI/100);
	glm::mat3 rotationVecX{ 1.0, 0.0, 0.0, 0.0, cos(randomAngle), sin(randomAngle), 0.0, -sin(randomAngle), cos(randomAngle) };
	reflectionRay = rotationVecX * reflectionRay;
	glm::mat3 rotationVecY{ cos(randomAngle), 0.0, -sin(randomAngle), 0.0, 1.0, 0.0, sin(randomAngle), 0.0, cos(randomAngle) };
	reflectionRay = rotationVecY * reflectionRay;
	/*glm::mat3 rotationVecZ{ cos(randomAngle), sin(randomAngle), 0.0 , -sin(randomAngle), cos(randomAngle), 0.0, 0.0, 0.0, 1 };
	reflectionRay = rotationVecZ * reflectionRay;*/
	return reflectionRay;
	
}

bool getClosestIntersection(RayTriangleIntersection &intersection, std::vector<ModelTriangle>& triangles, glm::vec3 &start, glm::vec3 &rayDirection, int startTriangleIndex) {
	float maxDistance = std::numeric_limits<float>::max();
	intersection.distanceFromCamera = maxDistance;
	for (int i = 0; i < triangles.size(); i++) {
		glm::vec3 e0 = triangles[i].vertices[1] - triangles[i].vertices[0];
		glm::vec3 e1 = triangles[i].vertices[2] - triangles[i].vertices[0];
		glm::vec3 SPVector = start - triangles[i].vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		/*float det = glm::determinant(DEMatrix);
		if (det == 0) {
			continue;
		}*/
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
		//float distance = possibleSolution.x * glm::length(rayDirection);
		float distance = possibleSolution.x;
		float u = possibleSolution.y;
		float v = possibleSolution.z;

		if (distance < 0.f) continue;
		else if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0) && (u + v) <= 1.0) {
			if (distance < maxDistance && distance < intersection.distanceFromCamera && i != startTriangleIndex) {
				//intersection.intersectionPoint = triangles[i].vertices[0] + (u * e0) + (v * e1);
				intersection.intersectionPoint = start + distance * rayDirection;
				intersection.distanceFromCamera = distance;
				intersection.intersectedTriangle = triangles[i];
				intersection.triangleIndex = i;
				/*if (shadow) {
					break;
				}*/
			}
		}
	}
	if (intersection.distanceFromCamera != maxDistance) {
		return true;
	} 
	else return false;
}

void interpolatePoint(glm::vec3 &point, ModelTriangle &triangle, float &u, float& v, float& w ) {
	float triangleArea = glm::length(triangle.normal) / 2.0f;
	//edge1
	glm::vec3 edge1 = triangle.vertices[2] - triangle.vertices[1];
	glm::vec3 vertexPoint1 = point - triangle.vertices[1];
	glm::vec3 C = glm::cross(edge1, vertexPoint1);
	u = (glm::length(C) / 2.0f) / triangleArea;
	/*if (glm::dot(triangle.normal, C) < 0) {
		std::cout << std::endl;
	}*/
	//edge2
	glm::vec3 edge2 = triangle.vertices[0] - triangle.vertices[2];
	glm::vec3 vertexPoint2 = point - triangle.vertices[2];
	C = glm::cross(edge2, vertexPoint2);
	v = (glm::length(C) / 2.0f) / triangleArea;
	/*if (glm::dot(triangle.normal, C) < 0) {
		std::cout << std::endl;
	}
	if (u + v >= 1 || u <= 0 || v <= 0 || u == 0 || v == 0) {
		std::cout << std::endl;
	}*/
	w = (1.0f - u - v);
}




glm::vec3 calculateColour(float &ambient, glm::vec3 normal,
						  glm::vec3 &pointToLightRay,
						  RayTriangleIntersection& intersection,
						  glm::vec3& start,
						  glm::vec3& lightSource,
						  glm::vec3 &colour,
						  float lightReduction,
						  int renderChoice) {
	
	glm::vec3 lightColour{ 1.0,1.0,1.0 };
	float lightIntensity;
	
	lightIntensity = (3 / (4 * M_PI * ((pow(glm::length(pointToLightRay), 2)))));
	//lightIntensity = (0.1 / (4 * M_PI * ((pow(glm::length(pointToLightRay), 2)))));
	//lightIntensity = (0.05 / (4 * M_PI * ((pow(glm::length(pointToLightRay), 2)))));

	//diffuse
	float diffuse = lightIntensity *  std::max(glm::dot(glm::normalize(normal), glm::normalize(pointToLightRay)), 0.0f);

	//specular
	glm::vec3 lightToPointRay = glm::normalize(intersection.intersectionPoint - lightSource);
	glm::vec3 reflectionRay = lightToPointRay - (2.0f * glm::normalize(normal) * (glm::dot(lightToPointRay, glm::normalize(normal))));
	glm::vec3 pointToCameraRay = start - intersection.intersectionPoint;
	float specular = lightIntensity * pow(std::max(glm::dot(glm::normalize(pointToCameraRay), glm::normalize(reflectionRay)), 0.0f), 64);

	//float lighting = diffuse;
	glm::vec3 lighting =  (lightReduction * diffuse * colour) + (lightReduction * specular * lightColour);
	//glm::vec3 lighting = (ambient + diffuse + specular) * colorVec;
	/*lighting.x = std::min(1.0f, lighting.x);
	lighting.y = std::min(1.0f, lighting.y);
	lighting.z = std::min(1.0f, lighting.z);*/
	
	return lighting;
}

void calculateLightingPerVertex(std::vector<ModelTriangle>& triangles,
								std::map<std::string, TextureMap>& textureMaps,
								std::vector<glm::vec3>& lightSources, 
								glm::vec3& start, float rayIndexOfRefraction,
								int reflectionCount, 
								int renderChoice) {
	for (int i = 0; i < triangles.size(); i++) {
		for (int j = 0; j < 3; j++) {
			RayTriangleIntersection vertex;
			vertex.intersectionPoint = triangles[i].vertices[j];
			vertex.intersectedTriangle = triangles[i];
			vertex.triangleIndex = i;
			vertex.intersectionPointNormal = triangles[i].vertexNormals[j];
			Colour colour = calculateLighting(vertex, triangles, textureMaps, lightSources, start, true, reflectionCount, rayIndexOfRefraction, renderChoice, 1);
			triangles[i].vertexLightings[j] = colour;
		}
	}
}


Colour calculateReflectionColour(glm::vec3& start,
								 RayTriangleIntersection& intersection, 
								 std::vector<ModelTriangle>& triangles,
								 std::map<std::string, TextureMap>& textureMaps,
								 std::vector<glm::vec3>& lightSources, 
								 float rayIndexOfRefraction, 
								 int reflectionCount,
								 int renderChoice) {
	glm::vec3 normal = intersection.intersectedTriangle.normal;
	glm::vec3 startToPointRay = glm::normalize(intersection.intersectionPoint - start);
	float dotProduct = (glm::dot(startToPointRay, glm::normalize(normal)));
	if (dotProduct > 0) {
		normal = -normal;
	}

	glm::vec3 reflectionRay = startToPointRay - (2.0f * glm::normalize(normal) * dotProduct);
	glm::vec3 sumColour;
	
	if (intersection.intersectedTriangle.materialType.compare("metal") == 0) {
		for (int i = 0; i < 10; i++) {
			RayTriangleIntersection nextIntersection;
			if (getClosestIntersection(nextIntersection, triangles, intersection.intersectionPoint, glm::normalize(reflectionRay), intersection.triangleIndex)) {
				float lightReduction = intersection.intersectedTriangle.lightReduction;
				Colour reflectionColour = calculateLighting(nextIntersection, triangles, textureMaps, lightSources, intersection.intersectionPoint, false, reflectionCount - 1, rayIndexOfRefraction, renderChoice, lightReduction);
				sumColour += glm::vec3{ reflectionColour.red,reflectionColour.green,reflectionColour.blue };

			}
			reflectionRay = randomScatterRay(reflectionRay);

		}
		sumColour = sumColour / 10.0f;
	}
	else {
		RayTriangleIntersection nextIntersection;
		if (getClosestIntersection(nextIntersection, triangles, intersection.intersectionPoint, glm::normalize(reflectionRay), intersection.triangleIndex)) {
			float lightReduction = intersection.intersectedTriangle.lightReduction;
			Colour reflectionColour = calculateLighting(nextIntersection, triangles, textureMaps, lightSources, intersection.intersectionPoint, false, reflectionCount - 1, rayIndexOfRefraction, renderChoice, lightReduction);
			sumColour = glm::vec3{ reflectionColour.red,reflectionColour.green,reflectionColour.blue };

		}
	}
	

	return Colour{ sumColour.x,sumColour.y,sumColour.z };
	
}

Colour calculateRefractionColour(RayTriangleIntersection& intersection,
								 std::vector<ModelTriangle>& triangles,
								 std::map<std::string, TextureMap>& textureMaps,
								 std::vector<glm::vec3>& lightSources,
								 glm::vec3& start,
							     float startRayIndexOfRefraction,
								 int reflectionCount, 
								 int renderChoice) {
	

	/*float n1 = startRayIndexOfRefraction;
	float n2 = hitRayIndexOfRefraction;*/
	float n1 = 1;
	float n2 = 1.5;

	glm::vec3 normal = glm::normalize(intersection.intersectedTriangle.normal);
	glm::vec3 incidentRay = glm::normalize(intersection.intersectionPoint - start);
	float c1 = glm::dot(normal,incidentRay);
	c1 = std::max(-1.0f, std::min(c1, 1.0f));
	if (c1 < 0) {
		c1 = -c1;
	}
	else {
		normal = -normal;
		std::swap(n1, n2);

	}
	float nr = n1 / n2;
	float checkC2 = 1 - ((pow(nr,2)) * (1 - pow(c1,2)));
	if (checkC2 < 0) {
		Colour reflectionColour = calculateReflectionColour(start, intersection, triangles, textureMaps, lightSources, n1, 3, renderChoice);
		return reflectionColour;

	}

	glm::vec3 refractionRay = (nr * incidentRay) + (((nr * c1) - sqrt(checkC2)) * normal);
	RayTriangleIntersection nextIntersection;
	if (getClosestIntersection(nextIntersection, triangles, intersection.intersectionPoint, glm::normalize(refractionRay), intersection.triangleIndex)) {
		float lightReduction = intersection.intersectedTriangle.lightReduction;
		Colour refractionColour = calculateLighting(nextIntersection, triangles, textureMaps, lightSources, intersection.intersectionPoint, false, 3, n2, renderChoice, lightReduction);
		return refractionColour;
	}
	else {
		Colour refractionColour = Colour{ 0,0,0 };
		return refractionColour;
	}

	
}

Colour calculateLighting(RayTriangleIntersection& intersection,
						 std::vector<ModelTriangle>& triangles,
						 std::map<std::string, TextureMap>& textureMaps,
						 std::vector<glm::vec3>& lightSources, 
						 glm::vec3 &start, 
						 bool initializeVertexColour,
						 int reflectionCount,
						 float rayIndexOfRefraction,
						 int renderChoice, float lightReduction) {
	float ambient = 0.1;
	glm::vec3 objColour;
	if (intersection.intersectedTriangle.hasTexture && renderChoice != 4) {
		std::string textureKey = intersection.intersectedTriangle.textureKey;
		float u, v, w;
		interpolatePoint(intersection.intersectionPoint, intersection.intersectedTriangle, u, v, w);
		glm::vec2 t0 = glm::vec2{ intersection.intersectedTriangle.texturePoints[0].x, intersection.intersectedTriangle.texturePoints[0].y};
		glm::vec2 t1 = glm::vec2{ intersection.intersectedTriangle.texturePoints[1].x, intersection.intersectedTriangle.texturePoints[1].y};
		glm::vec2 t2 = glm::vec2{ intersection.intersectedTriangle.texturePoints[2].x, intersection.intersectedTriangle.texturePoints[2].y};
		glm::vec2 resultTexturePoint = (t0 * u) + (t1 * v) + (t2 * w);
		resultTexturePoint.x = resultTexturePoint.x * textureMaps[textureKey].width;
		resultTexturePoint.y = resultTexturePoint.y * textureMaps[textureKey].height;
		int textureIndex = ((int)resultTexturePoint.y * textureMaps[textureKey].width) + (int)resultTexturePoint.x;
		uint32_t colour = textureMaps[textureKey].pixels[textureIndex];
		//objColour = glm::vec3{ ((colour >> 16) & 0xff), ((colour >> 8) & 0xff), (colour & 0xff)};
		objColour = glm::vec3{ ((colour >> 16) & 0xff)/255.0f, ((colour >> 8) & 0xff)/255.0f, (colour & 0xff)/255.0f};
	}
	else {
		objColour = glm::vec3{intersection.intersectedTriangle.colour.red,intersection.intersectedTriangle.colour.green, intersection.intersectedTriangle.colour.blue };

	}

	//glm::vec3 objColour = glm::vec3{ intersection.intersectedTriangle.colour.red,intersection.intersectedTriangle.colour.green, intersection.intersectedTriangle.colour.blue };
	glm::vec3 sumColor = (ambient * objColour);

	//reflection
	if (renderChoice == 6 && intersection.intersectedTriangle.hasReflection && reflectionCount > 0) {
		Colour reflectionColour = calculateReflectionColour(start, intersection, triangles, textureMaps, lightSources, rayIndexOfRefraction, reflectionCount, renderChoice);
		return reflectionColour;

	}
	else if (renderChoice == 6 && intersection.intersectedTriangle.hasRefraction) {
		Colour refractionColour = calculateRefractionColour(intersection, triangles, textureMaps, lightSources, start, rayIndexOfRefraction,reflectionCount, renderChoice);
		return refractionColour;

	}

	//shadow
	//int notInShadowCount = lightSources.size();
	for (glm::vec3 lightSource : lightSources) {
		RayTriangleIntersection shadowRayIntersection;
		float shadowBias = 0.01;
		glm::vec3 shadowPoint = intersection.intersectionPoint + (glm::normalize(intersection.intersectedTriangle.normal) * shadowBias);
		glm::vec3 pointToLightRay = lightSource - shadowPoint;

		bool shadowRayIntersects = getClosestIntersection(shadowRayIntersection, triangles, shadowPoint, glm::normalize(pointToLightRay), intersection.triangleIndex);
		if (renderChoice != 6 && shadowRayIntersects && shadowRayIntersection.distanceFromCamera < glm::length(pointToLightRay) && !initializeVertexColour) {
			/*Colour colour;
			colour.red = std::min(1.0f, objColour.x * ambient);
			colour.green = std::min(1.0f, objColour.y * ambient);
			colour.blue = std::min(1.0f, objColour.z * ambient);
			return colour;*/
			sumColor -= 0.0005;
			//notInShadowCount--;

		}
	
		else if (renderChoice == 3 || renderChoice == 6){
			glm::vec3 normal = intersection.intersectedTriangle.normal;			
		    sumColor += calculateColour(ambient, normal, pointToLightRay, intersection, start, lightSource, objColour, lightReduction, renderChoice);
			
		}
		else if (renderChoice == 4){
			if (initializeVertexColour) {
				glm::vec3 normal = intersection.intersectionPointNormal;
				sumColor += calculateColour(ambient, normal, pointToLightRay, intersection, start, lightSource, objColour, lightReduction, renderChoice);

			}
			else {
				glm::vec3 point = intersection.intersectionPoint;
				ModelTriangle triangle = intersection.intersectedTriangle;
				float u, v, w;
				interpolatePoint(point, triangle, u, v, w);
			
				glm::vec3 colour1 = glm::vec3{ triangle.vertexLightings[0].red, triangle.vertexLightings[0].green, triangle.vertexLightings[0].blue };
				glm::vec3 colour2 = glm::vec3{ triangle.vertexLightings[1].red, triangle.vertexLightings[1].green, triangle.vertexLightings[1].blue };
				glm::vec3 colour3 = glm::vec3{ triangle.vertexLightings[2].red, triangle.vertexLightings[2].green, triangle.vertexLightings[2].blue };

				glm::vec3 resultColour = (colour1 * u) + (colour2 * v) + (colour3 * w);
				Colour interpolatedColour;
				interpolatedColour.red = resultColour.x;
				interpolatedColour.green = resultColour.y;
				interpolatedColour.blue = resultColour.z;
				return interpolatedColour;
			}
		}
		else if (renderChoice == 5) {
			glm::vec3 point = intersection.intersectionPoint;
			ModelTriangle triangle = intersection.intersectedTriangle;
			float u, v, w;
			interpolatePoint(point, triangle, u, v, w);
			glm::vec3 interpolatedNormal = (triangle.vertexNormals[0] * u) + (triangle.vertexNormals[1] * v) + (triangle.vertexNormals[2] * w);
			sumColor += calculateColour(ambient, interpolatedNormal, pointToLightRay, intersection, start, lightSource, objColour, lightReduction, renderChoice);
		}
	}
	//float shadowRatio = notInShadowCount / lightSources.size();
	Colour colour;
	colour.red = std::max(0.0f, std::min(sumColor.x, 1.0f));
	colour.green = std::max(0.0f, std::min(sumColor.y, 1.0f));
	colour.blue = std::max(0.0f, std::min(sumColor.z, 1.0f));
	/*colour.red = std::max(0.0f, std::min(shadowRatio * sumColor.x, 1.0f));
	colour.green = std::max(0.0f, std::min(shadowRatio * sumColor.y, 1.0f));
	colour.blue = std::max(0.0f, std::min(shadowRatio * sumColor.z, 1.0f));*/
	return colour;
}




void drawRayTraceScene(std::vector<ModelTriangle>& triangles, DrawingWindow& window, std::map<std::string, TextureMap> &textureMaps, glm::vec3& cameraPosition, glm::mat3 &cameraOrientation, float& focalLength, Canvas& canvas, std::vector<glm::vec3> &lightSources, int renderChoice) {
	//index for air
	float rayIndexOfRefraction = 1;
	int reflectionCount = 3;

	if (renderChoice == 4) {
		calculateLightingPerVertex(triangles, textureMaps, lightSources, cameraPosition, rayIndexOfRefraction, reflectionCount, renderChoice);
	}

	float scale = 200;
	for (float y = 0.0; y < canvas.height; y++) {
		for (float x = 0.0; x < canvas.width; x++) {
			RayTriangleIntersection intersection;
			float convertCoordinateX = (x - (canvas.width / 2)) * (1/scale);
			float convertCoordinateY = (y - (canvas.height / 2) ) * (1/scale);
			glm::vec3 cameraSpace{ convertCoordinateX, -convertCoordinateY, -focalLength };
			glm::vec3 worldSpace = (cameraSpace * glm::inverse(cameraOrientation)) + cameraPosition;
			glm::vec3 rayDirection = glm::normalize(worldSpace - cameraPosition);
			
			if (getClosestIntersection(intersection, triangles, cameraPosition, rayDirection, -1)) {
				
				Colour colour = calculateLighting(intersection, triangles, textureMaps, lightSources, cameraPosition, false, reflectionCount, rayIndexOfRefraction, renderChoice, 1);
				uint32_t pixelColour = (255 << 24) + ((int)(255 * (colour.red)) << 16) + ((int)(255 * (colour.green)) << 8) + (int)(255 * (colour.blue));
				window.setPixelColour(x, y, pixelColour);
			}
			else continue;
		}
	}
}