#include "movement.h"
#include "generalFunctions.h"
void animation(std::vector<ModelTriangle>& objects, int& renderChoice, glm::vec3& cameraPosition, glm::mat3& cameraOrientation, bool &finish) {
	static float shiftOriginLeft = 0;
	static int animationCount = 0;
	static int animationStage = 0;
	static int sphereBounce = 0;
	glm::vec3 lookAtLocation;

	if (animationStage == 0) {
		rotate(-0.2, 'y', cameraPosition);
		lookAt(glm::vec3{ 0.0, 0.0 , 0.0 }, cameraPosition, cameraOrientation);
		animationCount++;
		if (animationCount == 30) {
			//reset camera position
			cameraPosition = glm::vec3{ 0,0,1.5 };
			cameraOrientation == glm::mat3{ 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
			lookAt(glm::vec3{ 0.0, 0.0, 0.0 }, cameraPosition, cameraOrientation);
			animationStage = 1;
			animationCount = 0;
		}

	}
	else if (animationStage == 1) {
		translateCamera(0, 0, -0.025, cameraPosition, cameraOrientation);
		std::string lookAtObject;
		if (animationCount < 10) {
			lookAtObject = "short_box";
		}
		else if (animationCount < 20) {
			lookAtObject = "sphere";
		}
		else if (animationCount < 30) {
			lookAtObject = "tall_box";
		}
		else if (animationCount < 35) {
			lookAtObject = "logo";
		}
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare(lookAtObject) == 0) {
				lookAt(objects[i].objectOrigin, cameraPosition, cameraOrientation);

			}
		}

		animationCount++;
		if (animationCount == 35) {
			animationStage = 2;
			animationCount = 0;
		}
	}
	else if (animationStage == 2) {
		translateCamera(0, 0, 0.025, cameraPosition, cameraOrientation);
		std::string lookAtObject;
		if (animationCount < 10) {
			lookAtObject = "logo";
		}
		else if (animationCount < 15) {
			lookAtObject = "tall_box";
		}
		else if (animationCount < 25) {
			lookAtObject = "sphere";
		}
		else if (animationCount < 35) {
			lookAtObject = "short_box";
		}
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare(lookAtObject) == 0) {
				lookAt(objects[i].objectOrigin, cameraPosition, cameraOrientation);

			}
		}

		animationCount++;
		if (animationCount == 35) {
			animationStage = 3;
			animationCount = 0;
			//reset
			cameraPosition = glm::vec3{ 0,0,1.5 };
			cameraOrientation == glm::mat3{ 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
			lookAt(glm::vec3{ 0.0, 0.0, 0.0 }, cameraPosition, cameraOrientation);
		}
	}
	else if (animationStage == 3) {
		translateCamera(0, 0, 0.075, cameraPosition, cameraOrientation);
		animationCount++;
		if (animationCount == 10) {
			animationStage = 4;
			animationCount = 0;
		}
	}

	else if (animationStage == 4) {
		renderChoice = 2;

		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare("front_wall") == 0) {
				if (objects[i].vertices[0].x != objects[i].vertices[1].x && shiftOriginLeft == 0) {
					shiftOriginLeft = abs(objects[i].vertices[1].x - objects[i].vertices[0].x) / 2;
				}
				else if (objects[i].vertices[0].x != objects[i].vertices[2].x && shiftOriginLeft == 0) {
					shiftOriginLeft = abs(objects[i].vertices[2].x - objects[i].vertices[0].x) / 2;
				}
				else if (objects[i].vertices[1].x != objects[i].vertices[2].x && shiftOriginLeft == 0) {
					shiftOriginLeft = abs(objects[i].vertices[2].x - objects[i].vertices[1].x) / 2;
				}
				float shiftedX = objects[i].objectOrigin.x - shiftOriginLeft;
				translateObject(objects[i], -shiftedX, -objects[i].objectOrigin.y, -objects[i].objectOrigin.z);
				rotateObject(objects[i], 'y', -0.1);
				translateObject(objects[i], shiftedX, objects[i].objectOrigin.y, objects[i].objectOrigin.z);
			}
		}
		animationCount++;
		if (animationCount == 30) {
			animationStage = 5;
			animationCount = 0;
		}
	}
	else if (animationStage == 5) {
		translateCamera(0, 0, -0.075, cameraPosition, cameraOrientation);
		animationCount++;
		if (animationCount == 10) {
			animationStage = 6;
			animationCount = 0;
		}
	}
	else if (animationStage == 6) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare("short_box") == 0) {
				translateObject(objects[i], -objects[i].objectOrigin.x, -objects[i].objectOrigin.y, -objects[i].objectOrigin.z);
				rotateObject(objects[i], 'y', -0.628);
				translateObject(objects[i], objects[i].objectOrigin.x, objects[i].objectOrigin.y, objects[i].objectOrigin.z);
			}
		}
		animationCount++;
		if (animationCount == 10) {
			animationStage = 7;
			animationCount = 0;
			//calculateNormal(objects);

		}
	}
	else if (animationStage == 7) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare("tall_box") == 0) {
				translateObject(objects[i], -objects[i].objectOrigin.x, -objects[i].objectOrigin.y, -objects[i].objectOrigin.z);
				rotateObject(objects[i], 'y', -0.628);
				translateObject(objects[i], objects[i].objectOrigin.x, objects[i].objectOrigin.y, objects[i].objectOrigin.z);
			}
		}
		animationCount++;
		if (animationCount == 10) {
			animationStage = 8;
			animationCount = 0;
			//calculateNormal(objects);
		}
	}
	else if (animationStage == 8) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare("logo") == 0) {
				translateObject(objects[i], -objects[i].objectOrigin.x, -objects[i].objectOrigin.y, -objects[i].objectOrigin.z);
				rotateObject(objects[i], 'z', -0.628);
				translateObject(objects[i], objects[i].objectOrigin.x, objects[i].objectOrigin.y, objects[i].objectOrigin.z);
			}
		}
		animationCount++;
		if (animationCount == 10) {
			animationStage = 9;
			animationCount = 0;
			//calculateNormal(objects);
		}
	}
	else if (animationStage == 9) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare("sphere") == 0) {
				if (sphereBounce == 0) {
					translateObject(objects[i], 0, 0.08, 0);
				}
				else if (sphereBounce == 1) {
					translateObject(objects[i], 0, -0.08, 0);
				}
			}
		}
		animationCount++;
		if (animationCount == 5) {
			sphereBounce = 1;
		}
		else if (animationCount == 10) {
			sphereBounce = 0;
		}
		else if (animationCount == 14) {
			sphereBounce = 1;
		}
		else if (animationCount == 18) {
			sphereBounce = 0;
		}
		else if (animationCount == 21) {
			sphereBounce = 1;
		}
		else if (animationCount == 24) {
			sphereBounce = 0;
		}
		else if (animationCount == 26) {
			sphereBounce = 1;
		}
		else if (animationCount == 28) {
			animationStage = 10;
			//animationStage = 13;
			animationCount = 0;
			//calculateNormal(objects);

		}
	}
	else if (animationStage == 10) {
		renderChoice = 3;
		animationStage = 11;
	}
	else if (animationStage == 11) {
		renderChoice = 4;
		animationStage = 12;
	}
	else if (animationStage == 12) {
		renderChoice = 5;
		animationStage = 13;
	}
	else if (animationStage == 13) {
		renderChoice = 6;
		animationStage = 14;
	}
	else if (animationStage == 14) {
		renderChoice = 2;
		animationStage = 15;
		animationCount = 0;

	}

	else if (animationStage == 15) {
		translateCamera(0, 0, 0.075, cameraPosition, cameraOrientation);
		animationCount++;
		if (animationCount == 10) {
			animationStage = 16;
			animationCount = 0;
		}
	}
	else if (animationStage == 16) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].obj_name.compare("front_wall") == 0) {
				float shiftedX = objects[i].objectOrigin.x - shiftOriginLeft;
				translateObject(objects[i], -shiftedX, -objects[i].objectOrigin.y, -objects[i].objectOrigin.z);
				rotateObject(objects[i], 'y', 0.1);
				translateObject(objects[i], shiftedX, objects[i].objectOrigin.y, objects[i].objectOrigin.z);
			}
		}
		animationCount++;
		if (animationCount == 30) {
			animationStage = 17;
			animationCount = 0;
		}
	}
	else if (animationStage == 17) {
		finish = true;
		return;
	}

}