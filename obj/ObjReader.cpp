//
// Created by Maximilian Denninger on 09.08.18.
//

#include "ObjReader.h"
#include <sstream>


void ObjReader::read(const std::string& filePath){
	std::ifstream stream(filePath);
	if(stream.is_open()){
		std::string line;
		while(std::getline(stream, line)){
			removeStartAndTrailingWhiteSpaces(line);
			if(startsWith(line, "v ")){
				Point3D p;
				line = line.substr(2, line.length() - 2);
				std::stringstream ss;
				ss << line;
				ss >> p;
				m_points.push_back(p);
			}else if(startsWith(line, "f ")){
				line = line.substr(2, line.length() - 2);
				removeStartAndTrailingWhiteSpaces(line);
				while(line.find('/') != std::string::npos){
					auto pos = line.find('/');
					auto nextPos = line.find(' ', pos);
					nextPos = nextPos > pos ? nextPos : std::string::npos;
					line.erase(pos, nextPos - pos);
				}
				std::stringstream ss;
				ss << line;
				iPoint pointIds;
				ss >> pointIds;
				m_polygons.emplace_back(pointIds, m_points);
				m_box.addPolygon(m_polygons.back());
			}
		}
		std::cout << m_box << std::endl;
	}else{
		printError("File \"" << filePath << "\" could not be read");
	}
}

void ObjReader::removeStartAndTrailingWhiteSpaces(std::string& line){
	if(line.length() > 0){
		long pos = 0;
		while(pos < line.length() && (line[pos] == ' ' || line[pos] == '\t')){
			++pos;
		}
		if(pos != 0){
			line.erase(0, pos);
		}
		pos = line.length() - 1;
		while(pos >= 0 && (line[pos] == ' ' || line[pos] == '\t')){
			--pos;
		}
		if(pos != line.length() - 1){
			line.erase(pos, std::string::npos);
		}
	}
}

bool ObjReader::startsWith(const std::string& line, const std::string& start){
	if(start.length() > line.length()){
		return false;
	}
	for(unsigned int i = 0; i < start.length(); ++i){
		if(start[i] != line[i]){
			return false;
		}
	}
	return true;
}
