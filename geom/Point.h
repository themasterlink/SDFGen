//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_POINT_H
#define SDFGEN_POINT_H

#include <array>
#include <iostream>
#include <cmath>
#include "../util/Utility.h"

template<typename dataType>
class Point {
public:

	using InternalStorage = std::array<dataType, 3>;
	using Iterator = typename InternalStorage::iterator;
	using ConstIterator = typename InternalStorage::const_iterator;

	Point() : m_data({0, 0, 0}){};

	Point(InternalStorage data) : m_data(std::move(data)){};

	Point(InternalStorage&& data) : m_data(std::move(data)){};

	Point(dataType x, dataType y, dataType z) : m_data({x, y, z}){};

	template<typename differentType>
	Point(const Point<differentType>& rhs);

	dataType& operator[](int i){ return m_data[i]; }

	const dataType operator[](int i) const{ return m_data[i]; }

	Point<dataType> operator+(const Point<dataType>& rhs) const;

	Point<dataType> operator-(const Point<dataType>& rhs) const;

	Point<dataType> operator/(dataType rhs) const;

	Point<dataType> operator*(dataType rhs) const;

	Iterator begin(){ return m_data.begin(); }

	ConstIterator begin() const{ return m_data.begin(); }

	Iterator end(){ return m_data.end(); }

	ConstIterator end() const{ return m_data.end(); }

	double length() const{ return sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2]); }

	double sumEle() const{ return m_data[0] + m_data[1] + m_data[2]; };

	void normalizeThis(){
		const auto len = length();
		if(len > 0.0){
			for(unsigned int i = 0; i < 3; ++i){
				m_data[i] /= len;
			}
		}
	}

	Point<dataType> normalize() const {
		Point<dataType> res(m_data);
		const auto len = length();
		if(len > 0.0){
			for(unsigned int i = 0; i < 3; ++i){
				res.m_data[i] /= len;
			}
		}
		return res;
	}

private:

	InternalStorage m_data;
};

using dPoint = Point<double>;
using iPoint = Point<int>;
using uiPoint = Point<unsigned int>;


class Point3D : public Point<double> {
public:

	Point3D() : Point(), m_index(0), m_used(true){};

	Point3D(double x, double y, double z, unsigned int index) : Point<double>(x, y, z), m_index(index), m_used(true){};

	unsigned int getIndex();

	void setIndex(unsigned int index){ m_index = index; };

	bool used(){ return m_used; }

	void setUsed(bool used){ m_used = used; }

private:
	unsigned int m_index;

	bool m_used;
};

#define POINT_CMP_FCT(lhs, rhs, op, outputType) \
    outputType point; \
    for(unsigned int i = 0; i < 3; ++i){ \
        point[i] = lhs[i] op rhs[i] ? lhs[i] : rhs[i]; \
    } \


template<typename dataType, typename differentType>
static Point<dataType> eMax(const Point<dataType>& lhs, const Point<differentType>& rhs){
	POINT_CMP_FCT(lhs, rhs, >, Point<dataType>);
	return point;
}

template<typename dataType, typename differentType>
static Point<dataType> eMin(const Point<dataType>& lhs, const Point<differentType>& rhs){
	POINT_CMP_FCT(lhs, rhs, <, Point<dataType>);
	return point;
}

template<typename dataType, typename differentType>
static Point<dataType> eMinEqual(const Point<dataType>& lhs, const Point<differentType>& rhs){
	POINT_CMP_FCT(lhs, rhs, <=, Point<dataType>);
	return point;
}

template<typename dataType, typename differentType>
static Point<dataType> eMaxEqual(const Point<dataType>& lhs, const Point<differentType>& rhs){
	POINT_CMP_FCT(lhs, rhs, >=, Point<dataType>);
	return point;
}

template<typename dataType, typename differentType>
static Point<dataType> cross(const Point<dataType>& lhs, const Point<differentType>& rhs){
	Point<dataType> res;
	res[0] = lhs[1] * rhs[2] - rhs[1] * lhs[2];
	res[1] = lhs[2] * rhs[0] - rhs[2] * lhs[0];
	res[2] = lhs[0] * rhs[1] - rhs[0] * lhs[1];
	return res;
};

template<typename dataType, typename differentType>
static double dot(const Point<dataType>& lhs, const Point<differentType>& rhs){
	double res = 0.0;
	for(unsigned int i = 0; i < 3; ++i){
		res += lhs[i] * rhs[i];
	}
	return res;
};

#define __POINT_INTERNAL__

#include "Point_i.h"

#undef __POINT_INTERNAL__

#endif //SDFGEN_POINT_H
