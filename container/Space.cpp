//
// Created by Maximilian Denninger on 15.08.18.
//

#include <cfloat>
#include "Space.h"
#include "../util/StopWatch.h"


void Space::calcDists(Polygons& polys){
	printVar(polys.size());
	std::vector<double> sizes;
	sizes.reserve(polys.size());
	for(auto& poly : polys){
		sizes.push_back(poly.size());
	}
	std::vector<unsigned int> used;
	for(unsigned int i = 0; i < sizes.size(); ++i){
		if(sizes[i] > 0.001){
			used.emplace_back(i);
		}
	}
	printVar(used.size());
	StopWatch sw;
	const auto size = m_data.getSize();
	for(unsigned int i = 0; i < size[0]; ++i){
		printVar(i);
		for(unsigned int j = 0; j < size[1]; ++j){
			for(unsigned int k = 0; k < size[2]; ++k){
				const dPoint point = getCenterOf(i, j, k);
				double minDist = DBL_MAX;
				for(const auto& index : used){
					auto& poly = polys[index];
					const double dist = poly.calcDistance(point);
					if(fabs(dist) < fabs(minDist)){
						minDist = dist;
					}
				}
				m_data(i, j, k) = minDist;
			}
		}
	}
	printVar(sw.elapsed_time());
}

dPoint Space::getCenterOf(unsigned int i, unsigned int j, unsigned int k){
	dPoint res;
	const auto size = m_data.getSize();
	res[0] = (i + 0.5) / (double) size[0] * m_size[0] + m_origin[0];
	res[1] = (j + 0.5) / (double) size[1] * m_size[1] + m_origin[1];
	res[2] = (k + 0.5) / (double) size[2] * m_size[2] + m_origin[2];
	return res;
}
