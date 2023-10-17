#ifndef PARSER_H
#define PARSER_H

std::map<std::string, Colour> obj_material_parser(std::string filename);
std::vector<ModelTriangle> obj_geometry_parser(std::string objFilename, std::string mtlFilename, std::string objectName, float scaler);

#endif
