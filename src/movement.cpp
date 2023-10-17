#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "movement.h"
void lookAt(glm::vec3 targetPoint, glm::vec3& cameraPosition, glm::mat3& cameraOrientation) {
	glm::vec3 newForward = glm::normalize(cameraPosition - targetPoint);
	glm::vec3 newRight = glm::cross(glm::vec3{ 0, 1, 0 }, newForward);
	glm::vec3 newUp = glm::cross(newForward, newRight);
	cameraOrientation = glm::mat3{ newRight, newUp, newForward };
}

void rotate(float angle, char axis, glm::vec3& cameraPosition) {
	if (axis == 'x') {
		glm::mat3 rotationVecX{ 1.0, 0.0, 0.0, 0.0, cos(angle), sin(angle), 0.0, -sin(angle), cos(angle) };
		cameraPosition = rotationVecX * cameraPosition;
	}
	else if (axis == 'y') {
		glm::mat3 rotationVecY{ cos(angle), 0.0, -sin(angle), 0.0, 1.0, 0.0, sin(angle), 0.0, cos(angle) };
		cameraPosition = rotationVecY * cameraPosition;
	}
}

void translateCamera(float directionX, float directionY, float directionZ, glm::vec3& cameraPosition, glm::mat3 cameraOrientation) {
	glm::vec3 translationVec{ directionX, directionY, directionZ };
	//cameraPosition = (cameraPosition + translationVec) * cameraOrientation;
	cameraPosition = (cameraPosition + translationVec);
}

void rotateCamera(float angle, char axis, glm::vec3& cameraPosition, glm::mat3& cameraOrientation) {
	if (axis == 'x') {
		glm::mat3 rotationVecX{ 1.0, 0.0, 0.0, 0.0, cos(angle), sin(angle), 0.0, -sin(angle), cos(angle) };
		cameraOrientation = rotationVecX * cameraOrientation;
		for (int i = 0; i < 3; i++) {
			cameraOrientation[i] = glm::normalize(cameraOrientation[i]);
		}
	}
	else if (axis == 'y') {
		glm::mat3 rotationVecY{ cos(angle), 0.0, -sin(angle), 0.0, 1.0, 0.0, sin(angle), 0.0, cos(angle) };
		cameraOrientation = rotationVecY * cameraOrientation;
		for (int i = 0; i < 3; i++) {
			cameraOrientation[i] = glm::normalize(cameraOrientation[i]);
		}
	}
}

void translateObject(ModelTriangle& object, float directionX, float directionY, float directionZ) {
	glm::vec3 translationVec{ directionX, directionY, directionZ };

	for (int i = 0; i < 3; i++) {
		object.vertices[i] = object.vertices[i] + translationVec;
	}
	
	return;
}


void rotateObject(ModelTriangle& object, char axis, float angle) {
	glm::mat3 rotationMatrix;
	if (axis == 'x') {
		rotationMatrix = glm::mat3{ 1.0, 0.0, 0.0, 0.0, cos(angle), sin(angle), 0.0, -sin(angle), cos(angle) };
	}
	else if (axis == 'y') {
		rotationMatrix = glm::mat3{ cos(angle), 0.0, -sin(angle), 0.0, 1.0, 0.0, sin(angle), 0.0, cos(angle) };
	}
	else if (axis == 'z') {
		rotationMatrix = glm::mat3{ cos(angle), sin(angle), 0, -sin(angle), cos(angle), 0.0, 0.0, 0.0, 1 };

	}
	for (int i = 0; i < 3; i++) {
		/*std::cout << glm::to_string(rotationMatrix) << std::endl;

		std::cout << glm::to_string(object.vertices[i]) << std::endl;*/
		object.vertices[i] = rotationMatrix * object.vertices[i];
		//std::cout << glm::to_string(object.vertices[i]) << std::endl;


	}
	return;
}