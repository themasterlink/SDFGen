//
// Created by Maximilian Denninger on 13.08.18.
//

#ifndef SDFGEN_BOUNDINGBOX_H
#define SDFGEN_BOUNDINGBOX_H


#include "math/Point.h"
#include "Polygon.h"

class BoundingBox {
public:

	BoundingBox() = default;

	BoundingBox(dPoint min, dPoint max): m_min(std::move(min)), m_max(std::move(max)){
	};

	bool isPointIn(const dPoint& point){
		for(unsigned int i = 0; i < 3; ++i){
			if(point[i] < m_min[i] || point[i] > m_max[i]){
				return false;
			}
		}
		return true;
	}

	void addPolygon(const Polygon& poly){
		for(const auto& point : poly.getPoints()){
			if(point.used()){
				addPoint(point);
			}
		}
	}

	template<typename dataType>
	void addPoint(const Point<dataType>& point){
		m_min = eMin(m_min, point);
		m_max = eMax(m_max, point);
	}

	const dPoint& min() const{
		return m_min;
	}

	const dPoint& max() const{
		return m_max;
	}

	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

private:
	dPoint m_min;
	dPoint m_max;
};

std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

#endif //SDFGEN_BOUNDINGBOX_H
