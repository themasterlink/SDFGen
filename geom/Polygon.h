//
// Created by Maximilian Denninger on 13.08.18.
//

#ifndef SDFGEN_POLYGON_H
#define SDFGEN_POLYGON_H


#include "math/Point.h"
#include <vector>
#include "Line.h"
#include "Plane.h"
#include "math/Transform.h"
#include <fstream>
#include <list>

using Points = std::vector<Point3D>;

class Polygon {
public:
	explicit Polygon(Points points): m_points(std::move(points)), m_calcNormal(false){
	};

	Polygon(const iPoint& indices, std::vector<Point3D>& pointsRef);

	Points& getPoints(){
		return m_points;
	}

	const Points& getPoints() const{
		return m_points;
	}

	double calcDistance(const dPoint& point);

	double calcDistance2(const dPoint& point);

	double size(){
		const auto first = m_points[0] - m_points[1];
		const auto second = m_points[2] - m_points[1];
		const double d1 = first[1] * second[2] - first[2] * second[1];
		const double d2 = first[2] * second[0] - first[0] * second[2];
		const double d3 = first[0] * second[1] - first[1] * second[0];
		return 0.5 * sqrt(d1 * d1 + d2 * d2 + d3 * d3);
	}

private:

	void calcNormal();

	Points m_points;

	bool m_calcNormal;

	Plane m_main;

	std::array<Plane, 3> m_edgePlanes;

	// for the perpendicular borders at the points
	std::array<std::array<Plane, 2>, 3> m_borderPlanes;

	std::array<Line, 3> m_edgeLines;

};

using Polygons = std::vector<Polygon>;


static void writeToDisc(Polygons& polygons, const std::string& filePath){

	std::ofstream output2(filePath, std::ios::out);

	int counter = 1;
//	BoundingBox cube({-2, -2, -2}, {2, 2, 2});
	for(auto& poly : polygons){
//		bool isInside = false;
//		for(auto& point : poly.getPoints()){
//			if(cube.isPointIn(point)){
//				isInside = true;
//				break;
//			}
//		}
////		isInside = true;
//		if(isInside){
		for(auto& point : poly.getPoints()){
			output2 << "v " << (point)[0] << " " << (point)[1] << " " << (point)[2] << "\n";
			point.setIndex(counter);
			counter += 1;
		}
//		}
	}
	int insideCounter = 0;
	for(auto& poly : polygons){
//		bool isInside = false;
//		for(auto& point : poly.getPoints()){
//			if(cube.isPointIn(point)){
//				isInside = true;
//				break;
//			}
//		}
//
////		isInside = true;
//		if(isInside){
		++insideCounter;
		output2 << "f";
		for(auto& point : poly.getPoints()){
			output2 << " " << point.getIndex();
		}
		output2 << "\n";

//		}
	}

	output2.close();
}

static void transformPoints(Polygons& polygons, const dTransform& trans){
	for(auto& poly: polygons){
		auto copyPoints = poly.getPoints();
		for(unsigned int i = 0; i < 3; ++i){
			auto& point = poly.getPoints()[i];
			trans.transform(point);
		}
	}
}

Polygons removePolygonsOutOfFrustum(Polygons& polys);


#endif //SDFGEN_POLYGON_H
