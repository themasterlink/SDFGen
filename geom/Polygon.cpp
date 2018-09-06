//
// Created by Maximilian Denninger on 13.08.18.
//

#include <cfloat>
#include "Polygon.h"
#include "../util/Utility.h"
#include "BoundingBox.h"

Polygon::Polygon(const iPoint& indices, Points& pointsRef): m_calcNormal(false){
	for(const auto& ele : indices){
		m_points.emplace_back(pointsRef[ele - 1]);
	}
}

double Polygon::calcDistance(const dPoint& point){
	if(m_calcNormal){
		double dist = m_main.getDist(point);
		std::array<double, 3> newDists = {0, 0, 0};
		for(unsigned int i = 0; i < 3; ++i){
			newDists[i] = m_edgePlanes[i].getDist(point);
		}
		for(unsigned int i = 0; i < 3; ++i){
			if(newDists[i] < 0.0){ // is outside
				const double firstBorder = m_borderPlanes[i][0].getDist(point);
				const double secondBorder = m_borderPlanes[i][1].getDist(point);
				if(firstBorder < 0.0){
					// use the point dist to the first point
					return (point - m_borderPlanes[i][0].m_pointOnPlane).length();
				}else if(secondBorder < 0.0){
					// use the point dist to second point
					return (point - m_borderPlanes[i][1].m_pointOnPlane).length();
				}else{
					return m_edgeLines[i].getDist(point);
				}
			}
		}
		return dist;

//		int insideCounter = 0;
//		for(const auto& val : newDists){
//			if(val < 0){
//				++insideCounter;
//			}
//		}
//		if(insideCounter == 1){
//			for(unsigned int i = 0; i < 3; ++i){
//				if(newDists[i] < 0){
//					return m_edgeLines[i].getDist(point);
//				}
//			}
//		}else if(insideCounter == 2){
//			int usedPointIndex = -1;
//			for(unsigned int i = 0; i < 3; ++i){
//				if(newDists[i] >= 0){
//					usedPointIndex = i;
//					break;
//				}
//			}
//			return (point - m_points[usedPointIndex]).length();
//		}
//		return dist;
	}else{
		calcNormal();
		if(m_calcNormal){
			return calcDistance(point);
		}
	}
	return -1;
}


double Polygon::calcDistance2(const dPoint& point){
	dPoint diff = point - m_points[0];
    dPoint edge0 = m_points[1] - m_points[0];
    dPoint edge1 = m_points[2] - m_points[0];
    double a00 = dot(edge0, edge0);
    double a01 = dot(edge0, edge1);
    double a11 = dot(edge1, edge1);
    double b0 = -dot(diff, edge0);
    double b1 = -dot(diff, edge1);
    double const zero = (double)0;
    double const one = (double)1;
    double det = a00 * a11 - a01 * a01;
    double t0 = a01 * b1 - a11 * b0;
    double t1 = a01 * b0 - a00 * b1;
    if (t0 + t1 <= det)
    {
        if (t0 < zero)
        {
            if (t1 < zero)  // region 4
            {
                if (b0 < zero)
                {
                    t1 = zero;
                    if (-b0 >= a00)  // V1
                    {
                        t0 = one;
                    }
                    else  // E01
                    {
                        t0 = -b0 / a00;
                    }
                }
                else
                {
                    t0 = zero;
                    if (b1 >= zero)  // V0
                    {
                        t1 = zero;
                    }
                    else if (-b1 >= a11)  // V2
                    {
                        t1 = one;
                    }
                    else  // E20
                    {
                        t1 = -b1 / a11;
                    }
                }
            }
            else  // region 3
            {
                t0 = zero;
                if (b1 >= zero)  // V0
                {
                    t1 = zero;
                }
                else if (-b1 >= a11)  // V2
                {
                    t1 = one;
                }
                else  // E20
                {
                    t1 = -b1 / a11;
                }
            }
        }
        else if (t1 < zero)  // region 5
        {
            t1 = zero;
            if (b0 >= zero)  // V0
            {
                t0 = zero;
            }
            else if (-b0 >= a00)  // V1
            {
                t0 = one;
            }
            else  // E01
            {
                t0 = -b0 / a00;
            }
        }
        else  // region 0, interior
        {
            double invDet = one / det;
            t0 *= invDet;
            t1 *= invDet;
        }
    }
    else
    {
        double tmp0, tmp1, numer, denom;

        if (t0 < zero)  // region 2
        {
            tmp0 = a01 + b0;
            tmp1 = a11 + b1;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)  // V1
                {
                    t0 = one;
                    t1 = zero;
                }
                else  // E12
                {
                    t0 = numer / denom;
                    t1 = one - t0;
                }
            }
            else
            {
                t0 = zero;
                if (tmp1 <= zero)  // V2
                {
                    t1 = one;
                }
                else if (b1 >= zero)  // V0
                {
                    t1 = zero;
                }
                else  // E20
                {
                    t1 = -b1 / a11;
                }
            }
        }
        else if (t1 < zero)  // region 6
        {
            tmp0 = a01 + b1;
            tmp1 = a00 + b0;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)  // V2
                {
                    t1 = one;
                    t0 = zero;
                }
                else  // E12
                {
                    t1 = numer / denom;
                    t0 = one - t1;
                }
            }
            else
            {
                t1 = zero;
                if (tmp1 <= zero)  // V1
                {
                    t0 = one;
                }
                else if (b0 >= zero)  // V0
                {
                    t0 = zero;
                }
                else  // E01
                {
                    t0 = -b0 / a00;
                }
            }
        }
        else  // region 1
        {
            numer = a11 + b1 - a01 - b0;
            if (numer <= zero)  // V2
            {
                t0 = zero;
                t1 = one;
            }
            else
            {
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)  // V1
                {
                    t0 = one;
                    t1 = zero;
                }
                else  // 12
                {
                    t0 = numer / denom;
                    t1 = one - t0;
                }
            }
        }
    }
    diff = point - m_points[0] + edge0 * t0 + edge1 * t1;
    return  dot(diff, diff);
}

void Polygon::calcNormal(){
	if(m_points.size() == 3){
		const auto vec01 = m_points[1] - m_points[0];
		const auto vec02 = m_points[2] - m_points[0];

		m_main.calcNormal(vec01, vec02);
		m_main.calcNormalDist(m_points[0]);

		// edgeId = 0 -> edge between 0 and 1
		for(unsigned int edgeId = 0; edgeId < 3; ++edgeId){
			const unsigned int nextId = (edgeId + 1) % 3;
			const unsigned int nextNextId = (edgeId + 2) % 3;
			const auto first = m_points[nextNextId] - m_points[nextId];
			const auto second = m_points[edgeId] - m_points[nextId];
			const double alpha = acos(dot(first, second) / (first.length() * second.length()));
			dPoint normal;
			if(fabs(alpha - M_PI_2) > 1e-5){
				const double length = cos(alpha) * second.length();
				const auto dir = first.normalize();
				auto newPoint = dir * length + m_points[nextId];
				normal = m_points[edgeId] - newPoint;
			}else{ // they are perpendicular
				normal = second;
			}
			m_edgePlanes[edgeId] = Plane(normal, m_points[nextId]);
			m_edgeLines[edgeId] = Line(m_points[nextNextId], m_points[nextId]);
			m_borderPlanes[edgeId][0] = Plane(first, m_points[nextId]);
			m_borderPlanes[edgeId][1] = Plane(first * (-1), m_points[nextNextId]);
		}

		m_calcNormal = true;
	}else{
		printError("Polygons have to be triangles!");
	}
}

Polygons removePolygonsOutOfFrustum(Polygons& polys){
	BoundingBox cube({-1, -1, -1}, {1, 1, 1});
	dPoint zero = {0,0,0};
	Polygons newPolys;
	for(unsigned int i = 0; i < polys.size(); ++i){
		bool isIn = false;
		for(auto& point : polys[i].getPoints()){
			if(cube.isPointIn(point)){
				isIn = true;
				break;
			}
		}
		if(isIn){
		 	newPolys.emplace_back(polys[i].getPoints());
		}else{
			const double dist = polys[i].calcDistance(zero);
			if(dist <= M_SQRT2){
				newPolys.emplace_back(polys[i].getPoints());
			}
		}
	}
	return newPolys;
}
