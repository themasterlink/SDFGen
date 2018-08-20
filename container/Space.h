//
// Created by Maximilian Denninger on 15.08.18.
//

#ifndef SDFGEN_SPACE_H
#define SDFGEN_SPACE_H


#include "Array3D.h"
#include "../geom/Polygon.h"


class Space {
public:

	void calcDists(std::vector<Polygon>& polys);



private:

	dPoint getCenterOf(unsigned int i, unsigned int j, unsigned int k);

	Array3D<double> m_data;

	dPoint m_origin;
	dPoint m_size;

};


#endif //SDFGEN_SPACE_H
