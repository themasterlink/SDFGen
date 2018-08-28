//
// Created by Maximilian Denninger on 13.08.18.
//

#include <cfloat>
#include "Polygon.h"
#include "../util/Utility.h"

Polygon::Polygon(const iPoint& indices, std::vector<Point3D>& pointsRef) : m_calcNormal(false){
	for(auto ele : indices){
		m_points.emplace_back(&pointsRef[ele]);
	}
}

double Polygon::calcDistance(const dPoint& point){
	if(m_calcNormal){
		double dist = m_main.getDist(point);
		std::array<double, 3> newDists = {0,0,0};
		for(unsigned int i = 0; i < 3; ++i){
			newDists[i] = m_edgePlanes[i].getDist(point);
		}
		int insideCounter = 0;
		for(const auto& val : newDists){
			if(val < 0){
				++insideCounter;
			}
		}
		if(insideCounter == 1){
			for(unsigned int i = 0; i < 3; ++i){
				if(newDists[i] < 0){
					return m_edgeLines[i].getDist(point);
				}
			}
		}else if(insideCounter == 2){
			int usedPointIndex = -1;
			for(unsigned int i = 0; i < 3; ++i){
				if(newDists[i] >= 0){
					usedPointIndex = i;
					break;
				}
			}
			return (point - *m_points[usedPointIndex]).length();
		}
		return dist;
	}else{
		calcNormal();
		if(m_calcNormal){
			return calcDistance(point);
		}
	}
	return -1;
}

void Polygon::calcNormal(){
	if(m_points.size() == 3){
		const auto vec01 = *m_points[1] - *m_points[0];
		const auto vec02 = *m_points[2] - *m_points[0];

		m_main.calcNormal(vec01, vec02);
		m_main.calcNormalDist(*m_points[0]);

		// edgeId = 0 -> edge between 0 and 1
		for(unsigned int edgeId = 0; edgeId < 3; ++edgeId){
			const auto first = *m_points[(edgeId + 2) % 3] - *m_points[(edgeId + 1) % 3];
			const auto second = *m_points[(edgeId) % 3] - *m_points[(edgeId + 1) % 3];
			const double alpha = acos(dot(first, second) / (first.length() * second.length()));
			dPoint normal;
			if(fabs(alpha - M_PI_2) > 1e-5){
				const double length = cos(alpha) * second.length();
				const auto dir = first.normalize();
				auto newPoint = dir * length + *m_points[(edgeId + 1) % 3];
				normal = *m_points[(edgeId) % 3] - newPoint;
			}else{ // they are perpendicular
				normal = second;
			}
			normal.normalizeThis();
			m_edgePlanes[edgeId] = Plane(normal, *m_points[(edgeId + 1) % 3]);
			m_edgeLines[edgeId] = Line(*m_points[(edgeId + 2) % 3], *m_points[(edgeId + 1) % 3]);
		}
		m_calcNormal = true;
	}else{
		printError("Polygons have to be triangles!");
	}
}