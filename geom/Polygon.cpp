//
// Created by Maximilian Denninger on 13.08.18.
//

#include "Polygon.h"


Polygon::Polygon(const iPoint& indices, std::vector<Point3D>& pointsRef){
	for(auto ele : indices){
		m_points.emplace_back(&pointsRef[ele]);
	}
}

