//
// Created by Maximilian Denninger on 15.08.18.
//

#include "Space.h"


void Space::calcDists(std::vector<Polygon>& polys){
	const auto size = m_data.getSize();
	for(unsigned int i = 0; i < size[0]; ++i){
		for(unsigned int j = 0; j < size[1]; ++j){
			for(unsigned int k = 0; k < size[2]; ++k){
				const dPoint point = getCenterOf(i,j,k);
				for(auto& poly : polys){
					double dist = poly.calcDistance(point);

				}
			}
		}
	}
}

dPoint Space::getCenterOf(unsigned int i, unsigned int j, unsigned int k){
	dPoint res;
	const auto size = m_data.getSize();
	res[0] = (i + 0.5) / (double) size[0] * m_size[0] + m_origin[0];
	res[1] = (j + 0.5) / (double) size[1] * m_size[1] + m_origin[1];
	res[2] = (k + 0.5) / (double) size[2] * m_size[2] + m_origin[2];
	return res;
}
