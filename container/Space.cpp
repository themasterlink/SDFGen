//
// Created by Maximilian Denninger on 15.08.18.
//

#include <cfloat>
#include "Space.h"
#include "../util/StopWatch.h"
#include "../geom/math/AvgNumber.h"


void Space::internCalcDist(const Polygons& polys, unsigned int start, unsigned int end,
						   const std::vector<unsigned int>& used){
	const auto size = m_data.getSize();
	for(unsigned int i = start; i < end; ++i){
		//printVar(i);
		for(unsigned int j = 0; j < size[1]; ++j){
			for(unsigned int k = 0; k < size[2]; ++k){
				const dPoint point = getCenterOf(i, j, k);
				double minDist = DBL_MAX;
				for(const auto& index : used){
					auto& poly = polys[index];
					const double dist = poly.calcDistanceConst(point);
					if(fabs(dist) < fabs(minDist)){
						minDist = dist;
					}
				}
				m_data(i, j, k) = minDist;
			}
		}
	}
}

void Space::internCalcDistForPoly(const Polygons& polys, const unsigned int startNot, const unsigned int endNot,
								  const std::vector<unsigned int>& notUsed){
	Array3D<double> temp(m_data.getSize());
	iPoint globalStart(m_data.getSize());
	iPoint globalEnd{0,0,0};
	for(unsigned int l = startNot; l < endNot; ++l){
		const unsigned int index = notUsed[l];
		const auto& poly = polys[index];
		auto bb = poly.getBB();
		const dPoint bbSize = bb.getSize();
		dPoint expandBB;
		const double minSize = 0.3;
		for(unsigned int i = 0; i < 3; ++i){
			if(bbSize[i] < minSize){
				expandBB[i] = minSize - bbSize[i];
			}
		}
		bb.expandBy(expandBB);
		iPoint start = getIndexOf(bb.min());
		iPoint end = getIndexOf(bb.max());
		unsigned int desiredSize = 50;
		for(unsigned int repeat = 0; repeat < desiredSize - 1; ++repeat){
			for(unsigned int i = 0; i < 3; ++i){
				if(end[i] - start[i] < desiredSize){
					if(end[i] < m_data.getSize()[i] - 1){
						end[i] += 1;
					}
					if(start[i] > 0){
						start[i] -= 1;
					}
				}
			}
		}
		for(int i = start[0]; i <= end[0]; ++i){
			for(int j = start[1]; j <= end[1]; ++j){
				for(int k = start[2]; k <= end[2]; ++k){
					const dPoint point = getCenterOf(i, j, k);
					const double dist = poly.calcDistanceConst(point);
					if(fabs(dist) < fabs(temp(i, j, k))){
						temp(i, j, k) = dist;
					}
				}
			}
		}
		globalStart = eMin(globalStart, start);
		globalEnd = eMax(globalEnd, end);
	}
	m_mutex.lock();
	for(int i = globalStart[0]; i <= globalEnd[0]; ++i){
		for(int j = globalStart[1]; j <= globalEnd[1]; ++j){
			for(int k = globalStart[2]; k <= globalEnd[2]; ++k){
				if(fabs(temp(i, j, k)) < fabs(m_data(i,j,k))){
					m_data(i,j,k) = temp(i, j, k);
				}
			}
		}
	}
	m_mutex.unlock();
}

void Space::calcDists(Polygons& polys){
	std::vector<unsigned int> used;
	std::vector<unsigned int> notUsed;
	for(unsigned int i = 0; i < polys.size(); ++i){
		if(polys[i].size() > 0.001){
			used.emplace_back(i);
		}else{
			notUsed.emplace_back(i);
		}
	}
	printVars(polys.size(), used.size());
	StopWatch sw;
	const auto size = m_data.getSize();
	for(auto& poly : polys){
		poly.calcNormal();
	}
	const unsigned int amountOfThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	for(unsigned int i = 0; i < amountOfThreads; ++i){
		unsigned int start = (unsigned int) (i * size[0] / (float) amountOfThreads);
		unsigned int end = (unsigned int) ((i + 1) * size[0] / (float) amountOfThreads);
		if(i + 1 == amountOfThreads){
			end = size[0];
		}
		threads.emplace_back(std::thread(&Space::internCalcDist, this, std::cref(polys), start, end, std::cref(used)));
	}
	for(auto& thread : threads){
		thread.join();
	}
	printVar(sw.elapsed_time());
	StopWatch swForSmallerPolys;
	std::vector<std::thread> threads2;
	for(unsigned int i = 0; i < amountOfThreads; ++i){
		unsigned int start = (unsigned int) (i * notUsed.size() / (float) amountOfThreads);
		unsigned int end = (unsigned int) ((i + 1) * notUsed.size() / (float) amountOfThreads);
		if(i + 1 == amountOfThreads){
			end = notUsed.size();
		}
		threads2.emplace_back(std::thread(&Space::internCalcDistForPoly, this, std::cref(polys), start, end, std::cref(notUsed)));
	}
	for(auto& thread : threads2){
		thread.join();
	}
	printVar(swForSmallerPolys.elapsed_time());
}

dPoint Space::getCenterOf(unsigned int i, unsigned int j, unsigned int k){
	dPoint res;
	const auto size = m_data.getSize();
	res[0] = (i + 0.5) / (double) size[0] * m_size[0] + m_origin[0];
	res[1] = (j + 0.5) / (double) size[1] * m_size[1] + m_origin[1];
	res[2] = (k + 0.5) / (double) size[2] * m_size[2] + m_origin[2];
	return res;
}

iPoint Space::getIndexOf(const dPoint& point) const{
	const iPoint id = iPoint(eMultiply(eDivide(point - m_origin, m_size), m_data.getSize()));
	return eMin(eMax(iPoint{0, 0, 0}, id), iPoint(m_data.getSize()) - iPoint(1, 1, 1));
}
