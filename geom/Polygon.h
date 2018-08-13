//
// Created by Maximilian Denninger on 13.08.18.
//

#ifndef SDFGEN_POLYGON_H
#define SDFGEN_POLYGON_H


#include "Point.h"
#include <vector>

class Polygon {
public:
	explicit Polygon(std::vector<Point3D*> points): m_points(std::move(points)) {};

	Polygon(const iPoint& indices, std::vector<Point3D>& pointsRef);

	std::vector<Point3D*>& getPoints(){ return m_points; }

	const std::vector<Point3D*>& getPoints() const { return m_points; }

private:
	std::vector<Point3D*> m_points;
};


#endif //SDFGEN_POLYGON_H
