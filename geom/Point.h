//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_POINT_H
#define SDFGEN_POINT_H

#include <array>
#include <iostream>

template<typename dataType>
class Point{
public:

	using InternalStorage = std::array<dataType, 3>;
	using Iterator = typename InternalStorage::iterator;
	using ConstIterator = typename InternalStorage::const_iterator;

	Point() : m_data({0, 0, 0}){};

	Point(InternalStorage data) : m_data(std::move(data)){};

	Point(InternalStorage&& data) : m_data(std::move(data)){};

	Point(dataType x, dataType y, dataType z) : m_data({x, y, z}){};

	dataType& operator[](int i){ return m_data[i]; }

	const dataType operator[](int i) const{ return m_data[i]; }

	Point<dataType> operator+(const Point<dataType>& rhs);

	Point<dataType> operator-(const Point<dataType>& rhs);

	Point<dataType> operator/(dataType rhs);

	Point<dataType> operator*(dataType rhs);

	Iterator begin(){ return m_data.begin(); }

	ConstIterator begin() const{ return m_data.begin(); }

	Iterator end(){ return m_data.end(); }

	ConstIterator end() const{ return m_data.end(); }

private:

	InternalStorage m_data;
};

using dPoint = Point<double>;
using iPoint = Point<int>;
using uiPoint = Point<unsigned int>;


class Point3D : public Point<double>{
public:

	Point3D(): Point(), m_index(0) {};

	Point3D(double x, double y, double z, unsigned int index) : Point<double>(x, y, z), m_index(index){};

	unsigned int getIndex();

	void setIndex(unsigned int index){ m_index = index; };

private:
	unsigned int m_index;
};

#define __POINT_INTERNAL__

#include "Point_i.h"

#undef __POINT_INTERNAL__

#endif //SDFGEN_POINT_H
