#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include <map>
#include <glm/gtx/string_cast.hpp>
#include "parser.h"
#include "events.h"
#include "movement.h"
#include "Renderer.h"
#include "RasterisedRender.h"
#include "RaytraceRender.h"
#include "animation.h"

//#define WIDTH 320
//#define HEIGHT 240
#define WIDTH 640
#define HEIGHT 480


bool check_depth_buffer(int x, int y, float depth, bool reset) {
	static std::vector<std::vector<float>> depth_buffer(HEIGHT, std::vector<float>(WIDTH, 0.0));
	if (reset) {
		for (size_t y = 0; y < HEIGHT; y++) {
			for (size_t x = 0; x < WIDTH; x++) {
				depth_buffer[y][x] = 0.0;
			}
		}
		return false;
	}

	//depth = depth * -1.0;
	if (depth_buffer[y][x] < 1/depth) {
		depth_buffer[y][x] = 1/depth;
		return true;
	}
	return false;
}



void draw(std::vector<ModelTriangle>& obj_vector, std::map<std::string, TextureMap>& textureMaps, DrawingWindow& window, glm::vec3& cameraPosition, float& focalLength, glm::mat3& cameraOrientation, bool& orbit, Canvas& canvas, int renderChoice, std::vector<glm::vec3> &lightSources) {
	if (renderChoice == 1) {
		check_depth_buffer(0, 0, 0, true);
		drawWireframe(obj_vector, window, cameraPosition, focalLength, cameraOrientation, canvas);
	}
	else if (renderChoice == 2) {
		check_depth_buffer(0, 0, 0, true);
		drawRasterisedRender(obj_vector, window, cameraPosition, focalLength, textureMaps, cameraOrientation, canvas);
	}
	else {
		drawRayTraceScene(obj_vector, window, textureMaps, cameraPosition, cameraOrientation, focalLength, canvas, lightSources, renderChoice);
	}

	if (orbit) {
		glm::vec3 targetPoint{ 0.0, 0.0, 0.0 };
		rotate(-0.01, 'y', cameraPosition);
		lookAt(targetPoint, cameraPosition, cameraOrientation);
	}
}



int main(int argc, char *argv[]) {
	glm::vec3 cameraPosition{ 0.0, 0.0, 1.5};
	//glm::vec3 cameraPosition{ 0.0, 0.0, 2};
	float focalLength = 2;
	glm::mat3 cameraOrientation{ 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
	bool orbit = false;
	Canvas canvas{ WIDTH, HEIGHT};
	glm::vec3 lightCenter{0,0,0.4};
	std::vector<glm::vec3> lightSources = createLightSources(lightCenter);
	int renderChoice = 1;
	std::map<std::string, TextureMap> TextureMaps;

	std::string objectName = "cornell_box";
	TextureMap image = TextureMap("../../../src/chess.ppm");
	TextureMaps[objectName] = image;

	//render with or without cover (used covered when running animation)
	//std::vector<ModelTriangle> cornell_box = obj_geometry_parser("../../../src/cornell-box-with-front-wall.obj", "../../../src/textured-cornell-box.mtl", objectName, 0.17);
	std::vector<ModelTriangle> cornell_box = obj_geometry_parser("../../../src/textured-cornell-box.obj", "../../../src/textured-cornell-box.mtl", objectName, 0.17);

	findObjectOrigin(cornell_box, "short_box");
	findObjectOrigin(cornell_box, "tall_box");
	findObjectOrigin(cornell_box, "front_wall");
	calculateNormal(cornell_box);
	calculateVertexNormal(cornell_box);

	objectName = "logo";
	image = TextureMap("../../../src/logo/texture.ppm");
	TextureMaps["logo"] = image;
	std::vector<ModelTriangle> logo = obj_geometry_parser("../../../src/logo/logo.obj", "../../../src/materials.mtl", objectName, 0.001);
	for (int i = 0; i < logo.size(); i++) {
		translateObject(logo[i], -0.3, -0.1, -0.4);
	}
	findObjectOrigin(logo, "logo");
	calculateNormal(logo);
	calculateVertexNormal(logo);

	objectName = "sphere";
	std::vector<ModelTriangle> sphere = obj_geometry_parser("../../../src/sphere.obj", "../../../src/textured-cornell-box.mtl", objectName, 0.17);
	for (int i = 0; i < sphere.size(); i++) {
		translateObject(sphere[i], -0.25, -0.55, 0.1);
	}
	calculateNormal(sphere);
	findObjectOrigin(sphere, "sphere");

	std::vector<ModelTriangle> obj_vector;

	//render cornell box
	//obj_vector = cornell_box;
	
	//render logo
	//obj_vector = logo;


	//render sphere
	//obj_vector = sphere;

	//render sphere and cornell box
	/*obj_vector.reserve(cornell_box.size() + sphere.size());
	obj_vector.insert(obj_vector.end(), cornell_box.begin(), cornell_box.end());
	obj_vector.insert(obj_vector.end(), sphere.begin(), sphere.end());*/
	
	//render cornell and logo
	//obj_vector.reserve(cornell_box.size() + logo.size());
	//obj_vector.insert(obj_vector.end(), cornell_box.begin(), cornell_box.end());
	//obj_vector.insert(obj_vector.end(), logo.begin(), logo.end());

	//render all
	obj_vector.reserve(cornell_box.size() + logo.size() + sphere.size());
	obj_vector.insert(obj_vector.end(), cornell_box.begin(), cornell_box.end());
	obj_vector.insert(obj_vector.end(), logo.begin(), logo.end());
	obj_vector.insert(obj_vector.end(), sphere.begin(), sphere.end());

	DrawingWindow window = DrawingWindow(canvas.width, canvas.height, false);
	SDL_Event event;

	//variables for animation
	bool finish = false;
	int frameCount = 1;


	//key bindings:
	/*arrow keys to translate camera, shift to move forward, ctrl to move backwards
	wasd to rotate camera orientation
	space to orbit box
	yghj to rotate about x and y axis
	c to look at origin
	1 to switch to wireframe
	2 to switch to rasterize
	3 to switch to raytracing (to enable soft shadow, go to createLightSources() function)
	4 to switch to goround shading
	5 to switch to phong shading
	6 to switch on reflection and refraction, disables shadow*/
	
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window, cameraPosition, lightCenter, cameraOrientation, orbit, canvas, renderChoice);
		window.clearPixels();
		draw(obj_vector, TextureMaps, window, cameraPosition, focalLength, cameraOrientation, orbit, canvas, renderChoice, lightSources);

		//display light
		//for (glm::vec3 light : lightSources) {
		//	CanvasPoint lightVertex = getCanvasIntersectionPoint(cameraPosition, light, focalLength, window, cameraOrientation, canvas);
		//	uint32_t lineColour = (255 << 24) + (255 << 16) + (255 << 8) + (255);
		//	//std::cout << glm::to_string(light) << std::endl;
		//	window.setPixelColour(lightVertex.x, lightVertex.y, lineColour);
		//}

		//enable animation
		//animation(obj_vector, renderChoice, cameraPosition,cameraOrientation, finish);
		
		window.renderFrame();
		
		//capture frames
		/*std::string directory = "capture/";
		std::string count = std::to_string(frameCount);
		directory.append(count);
		directory.append(".bmp");
		window.saveBMP(directory);
		frameCount++;*/

		//stop rendering if animation ends
		if (finish) {
			break;
		}

	}
	return 0;
}
