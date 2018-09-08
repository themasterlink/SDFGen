//
// Created by Maximilian Denninger on 15.08.18.
//

#ifndef SDFGEN_SPACE_H
#define SDFGEN_SPACE_H


#include "Array3D.h"
#include "../geom/Polygon.h"
#include <thread>

class Space {
public:

	Space(const uiPoint& iSize, const dPoint& origin, const dPoint& size): m_data(iSize), m_origin(origin), m_size(size) {};

	void calcDists(Polygons& polys);

	double getI(unsigned int i, unsigned int j, unsigned int k) { return m_data(i,j,k); }


private:

	void internCalcDist(const Polygons& polys, unsigned int start, unsigned int end, const std::vector<unsigned int>& used);

	void internCalcDistForPoly(const Polygons& polys, const unsigned int startNot, const unsigned int endNot,
									  const std::vector<unsigned int>& notUsed);

	dPoint getCenterOf(unsigned int i, unsigned int j, unsigned int k);

	iPoint getIndexOf(const dPoint& point) const;

	Array3D<double> m_data;

	dPoint m_origin;
	dPoint m_size;

	std::mutex m_mutex;

};


#endif //SDFGEN_SPACE_H
