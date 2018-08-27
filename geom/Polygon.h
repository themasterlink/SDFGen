//
// Created by Maximilian Denninger on 13.08.18.
//

#ifndef SDFGEN_POLYGON_H
#define SDFGEN_POLYGON_H


#include "Point.h"
#include <vector>

struct Plane {

	Plane() = default;

	explicit Plane(dPoint normal, const dPoint& pointOnPlane) : m_normal(std::move(normal)), m_dist(0.0){
		calcNormalDist(pointOnPlane);
	}

	Plane(dPoint normal, double dist) : m_normal(std::move(normal)), m_dist(dist){};

	void calcNormal(const dPoint& first, const dPoint& second){
		m_normal = cross(first, second);
		m_normal.normalizeThis();
	}

	void calcNormalDist(const dPoint& pointOnPlane){
		m_dist = -dot(m_normal, pointOnPlane);
	}

	double getDist(const dPoint& point) const {
		return dot(m_normal, point) - m_dist;
	}

	dPoint m_normal;

	double m_dist;
};

struct Line {

	Line() = default;

	Line(Point3D& first, Point3D& second): m_first(&first), m_second(&second) {
		m_normal = *m_second - *m_first;
		m_normal.normalizeThis();
		printVar(m_normal);
	};

	double getDist(const dPoint& toPoint){
		const auto vecToPoint = *m_first - toPoint;
		return (vecToPoint - m_normal * dot(vecToPoint, m_normal)).length();
	}

	Point3D* m_first;
	Point3D* m_second;
	dPoint m_normal;
};

class Polygon {
public:
	explicit Polygon(std::vector<Point3D*> points) : m_points(std::move(points)), m_calcNormal(false){};

	Polygon(const iPoint& indices, std::vector<Point3D>& pointsRef);

	std::vector<Point3D*>& getPoints(){ return m_points; }

	const std::vector<Point3D*>& getPoints() const{ return m_points; }

	double calcDistance(const dPoint& point);

private:

	void calcNormal();

	std::vector<Point3D*> m_points;

	bool m_calcNormal;

	Plane m_main;

	std::array<Plane, 3> m_edgePlanes;
	std::array<Line, 3> m_edgeLines;


};


#endif //SDFGEN_POLYGON_H
