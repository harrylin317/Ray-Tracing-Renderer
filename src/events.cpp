#include <fstream>
#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include <Colour.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <glm/gtx/string_cast.hpp>

#include "generalFunctions.h"
#include "events.h"
#include "movement.h"

void handleEvent(SDL_Event event, DrawingWindow& window, glm::vec3& cameraPosition, glm::vec3 &lightSource, glm::mat3& cameraOrientation, bool& orbit, Canvas& canvas, int &renderChoice) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			std::cout << "LEFT" << std::endl;
			translateCamera(-0.25, 0, 0, cameraPosition, cameraOrientation);
			std::cout << "camera position " << glm::to_string(cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT" << std::endl;
			translateCamera(0.25, 0, 0, cameraPosition, cameraOrientation);
			std::cout << "camera position " << glm::to_string(cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_UP) {
			std::cout << "UP" << std::endl;
			translateCamera(0, 0.25, 0, cameraPosition, cameraOrientation);
			std::cout << "camera position " << glm::to_string(cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			std::cout << "DOWN" << std::endl;
			translateCamera(0, -0.25, 0, cameraPosition, cameraOrientation);
			std::cout << "camera position " << glm::to_string(cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_LSHIFT) {
			std::cout << "LSHIFT" << std::endl;
			translateCamera(0, 0, -0.10, cameraPosition, cameraOrientation);
			//translate(0, 0, -0.1, lightSource, cameraOrientation);
			std::cout << "camera position " << glm::to_string(cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_LCTRL) {
			std::cout << "LCTRL" << std::endl;
			translateCamera(0, 0, 0.10, cameraPosition, cameraOrientation);
			//translate(0, 0, 0.1, lightSource, cameraOrientation);
			std::cout << "camera position " << glm::to_string(cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_y) {
			std::cout << "y" << std::endl;
			rotate(-0.25, 'x', cameraPosition);
		}
		else if (event.key.keysym.sym == SDLK_g) {
			std::cout << "g" << std::endl;
			rotate(-0.25, 'y', cameraPosition);
		}
		else if (event.key.keysym.sym == SDLK_h) {
			std::cout << "h" << std::endl;
			rotate(0.25, 'x', cameraPosition);
		}
		else if (event.key.keysym.sym == SDLK_j) {
			std::cout << "j" << std::endl;
			rotate(0.25, 'y', cameraPosition);
		}
		else if (event.key.keysym.sym == SDLK_w) {
			std::cout << "w" << std::endl;
			rotateCamera(0.1, 'x', cameraPosition, cameraOrientation);
		}
		else if (event.key.keysym.sym == SDLK_a) {
			std::cout << "a" << std::endl;
			rotateCamera(0.1, 'y', cameraPosition, cameraOrientation);
		}
		else if (event.key.keysym.sym == SDLK_s) {
			std::cout << "s" << std::endl;
			rotateCamera(-0.1, 'x', cameraPosition, cameraOrientation);
		}
		else if (event.key.keysym.sym == SDLK_d) {
			std::cout << "d" << std::endl;
			rotateCamera(-0.1, 'y', cameraPosition, cameraOrientation);
		}
		else if (event.key.keysym.sym == SDLK_u) {
			Colour colour = Colour(rand() % 256, rand() % 256, rand() % 256);
			CanvasPoint point1 = CanvasPoint(rand() % canvas.width, rand() % canvas.height);
			CanvasPoint point2 = CanvasPoint(rand() % canvas.width, rand() % canvas.height);
			CanvasPoint point3 = CanvasPoint(rand() % canvas.width, rand() % canvas.height);
			CanvasTriangle triangle = CanvasTriangle(point1, point2, point3);
			drawStrokedTriangles(triangle, colour, window, canvas);
		}
		else if (event.key.keysym.sym == SDLK_f) {
			Colour colour = Colour(rand() % 256, rand() % 256, rand() % 256);
			CanvasPoint point1 = CanvasPoint(rand() % canvas.width, rand() % canvas.height);
			CanvasPoint point2 = CanvasPoint(rand() % canvas.width, rand() % canvas.height);
			CanvasPoint point3 = CanvasPoint(rand() % canvas.width, rand() % canvas.height);
			CanvasTriangle triangle = CanvasTriangle(point1, point2, point3);
			drawFilledTriangles(triangle, colour, window, canvas);
		}
		else if (event.key.keysym.sym == SDLK_SPACE) {
			if (orbit) {
				orbit = false;
			}
			else orbit = true;
		}
		else if (event.key.keysym.sym == SDLK_c) {
			std::cout << "c" << std::endl;
			lookAt(glm::vec3{ 0,0,0 }, cameraPosition, cameraOrientation);
		}
		else if (event.key.keysym.sym == SDLK_1) {
			std::cout << "switch to wire frame render" << std::endl;
			renderChoice = 1;
		}
		else if (event.key.keysym.sym == SDLK_2) {
			std::cout << "switch to rasterised render" << std::endl;
			renderChoice = 2;
		}
		else if (event.key.keysym.sym == SDLK_3) {
			std::cout << "switch to raytracing render (normal lighting)" << std::endl;
			renderChoice = 3;
		}
		else if (event.key.keysym.sym == SDLK_4) {
			std::cout << "switch to raytracing render (Gouraud Shading)" << std::endl;
			renderChoice = 4;
		}
		else if (event.key.keysym.sym == SDLK_5) {
			std::cout << "switch to raytracing render (Phong Shading)" << std::endl;
			renderChoice = 5;
		}
		else if (event.key.keysym.sym == SDLK_6) {
		std::cout << "turn on glass and mirror, switch off shadow" << std::endl;
		renderChoice = 6;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
	std::cout << "saved" << std::endl;
		window.savePPM("capture/output.ppm");
		window.saveBMP("capture/output.bmp");
	}
}
