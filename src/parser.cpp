#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <Colour.h>
#include <ModelTriangle.h>
#include <map>

#include "parser.h"

std::map<std::string, Colour> obj_material_parser(std::string filename) {
	std::ifstream inFile(filename);
	std::string nextLine;
	std::map<std::string, Colour> colourMap;
	std::string colourName;
	while (std::getline(inFile, nextLine)) {
		std::vector<std::string> splitted = split(nextLine, ' ');
		if (splitted[0].compare("newmtl") == 0) {
			colourName = splitted[1];
		}
		else if (splitted[0].compare("Kd") == 0) {
			colourMap[colourName] = Colour(colourName, std::stof(splitted[1]), std::stof(splitted[2]), std::stof(splitted[3]));
			
		}
	}
	
	return colourMap;
}


std::vector<ModelTriangle> obj_geometry_parser(std::string objFilename, std::string mtlFilename, std::string objectName, float scaler) {
	std::ifstream inFile(objFilename);
	std::string nextLine;
	std::vector<glm::vec3> vertices;
	std::vector<TexturePoint> textureVertices;
	std::vector<glm::vec3> vertexNormals;
	std::vector<std::vector<std::string>> facets;
	std::vector<ModelTriangle> triangleVector;
	std::map<std::string, Colour> colourMap = obj_material_parser(mtlFilename);
	std::string currentColour = "";
	std::string currentObjName = "";

	while (std::getline(inFile, nextLine)) {
		std::vector<std::string> splitted = split(nextLine, ' ');
		if (splitted[0] == "v") {
			glm::vec3 vertex = glm::vec3(scaler * std::stof(splitted[1]), scaler * std::stof(splitted[2]), scaler * std::stof(splitted[3]));
			vertices.push_back(vertex);
		}
		else if (splitted[0] == "o") {
			currentObjName = splitted[1];
		}
		else if (splitted[0] == "f") {
			std::vector<std::string>facet{ splitted[1], splitted[2], splitted[3], currentColour, currentObjName };
			facets.push_back(facet);
		}
		else if (splitted[0].compare("usemtl") == 0) {
			currentColour = splitted[1];
		}
		else if (splitted[0].compare("vt") == 0) {
			TexturePoint textureVertex = TexturePoint(fmod(std::stof(splitted[1]), 1), fmod(std::stof(splitted[2]), 1));
			textureVertices.push_back(textureVertex);
		}
		else if (splitted[0].compare("vn") == 0) {
			glm::vec3 vertexNormal = glm::vec3(scaler * std::stof(splitted[1]), scaler * std::stof(splitted[2]), scaler * std::stof(splitted[3]));
			vertexNormals.push_back(vertexNormal);

		}
	}

	for (int facet_index = 0; facet_index < facets.size(); facet_index++) {
		ModelTriangle triangle;
		int v1_index;
		int v2_index;
		int v3_index;
		for (int line_index = 0; line_index < 3; line_index++) {
			std::vector<std::string> splittedLine;
			float vertexIndex;
			std::string line = facets[facet_index][line_index];
			if (line.find("//") != std::string::npos){
				splittedLine = split(facets[facet_index][line_index], '//');
				vertexIndex = std::stof(splittedLine[0]) - 1;
				triangle.vertices[line_index] = vertices[vertexIndex];

				float vertexNormalIndex = std::stof(splittedLine[2]) - 1;
				triangle.vertexNormals[line_index] = glm::normalize(vertexNormals[vertexNormalIndex]);

			}
			else {
				splittedLine = split(facets[facet_index][line_index], '/');
				vertexIndex = std::stof(splittedLine[0]) - 1;
				triangle.vertices[line_index] = vertices[vertexIndex];

				if (splittedLine[1] != "") {
					//ignore vt points for logo
					if (currentObjName.compare("logo") == 0) {
						continue;
					}
					float textureVertexIndex = std::stof(splittedLine[1]) - 1;
					triangle.texturePoints[line_index] = textureVertices[textureVertexIndex];
					triangle.hasTexture = true;
					triangle.textureKey = objectName;
				}
			}
					
		}
		std::string colourName = facets[facet_index][3];
		if (objectName.compare("logo") == 0) {
			triangle.colour = Colour{ 1,0.5,0 };
		}
		else if (objectName.compare("sphere") == 0) {
			triangle.colour = Colour{ 1,0,0 };
		}
		else {
			triangle.colour = colourMap[colourName];

		}


		
		triangle.hasReflection = false;
		triangle.hasRefraction = false;
		triangle.lightReduction = 1;
		triangle.obj_name = facets[facet_index][4];
		//set materials
		if (triangle.obj_name.compare("left_wall") == 0 || triangle.obj_name.compare("right_wall") == 0) {
			triangle.hasReflection = true;
			triangle.materialType = "mirror";
		}
		else if (triangle.obj_name.compare("tall_box") == 0 || triangle.obj_name.compare("sphere") == 0 || triangle.obj_name.compare("logo") == 0 || triangle.obj_name.compare("short_box") == 0){
			triangle.hasRefraction = true;
			triangle.materialType = "glass";
			triangle.lightReduction = 0.8;
		}
		else if (triangle.obj_name.compare("floor") == 0) {
			triangle.hasReflection = true;
			triangle.materialType = "metal";
			triangle.lightReduction = 0.5;
		}

		triangleVector.push_back(triangle);
	}
	return triangleVector;
}