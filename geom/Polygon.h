//
// Created by Maximilian Denninger on 13.08.18.
//

#ifndef SDFGEN_POLYGON_H
#define SDFGEN_POLYGON_H


#include "Point.h"
#include <vector>

struct Plane {

	Plane() {};

	explicit Plane(dPoint normal, const dPoint& pointOnPlane) : m_normal(std::move(normal)){
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

	double getDist(const dPoint& point){
		return dot(m_normal, point) - m_dist;
	}

	dPoint m_normal;

	double m_dist;
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


};


#endif //SDFGEN_POLYGON_H
