//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_OBJREADER_H
#define SDFGEN_OBJREADER_H

#include <string>
#include <fstream>
#include "../util/Utility.h"
#include "../geom/Point.h"
#include <vector>
#include "../geom/Polygon.h"

class ObjReader {
public:
	ObjReader() = default;

	void read(const std::string& filePath);

private:
	std::vector<Point3D> m_points;
	std::vector<Polygon> m_polygons;

	bool startsWith(const std::string& line, const std::string& start);

	void removeStartAndTrailingWhiteSpaces(std::string& line);

};


#endif //SDFGEN_OBJREADER_H
