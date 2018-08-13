//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_POINT_I_H
#define SDFGEN_POINT_I_H


#ifndef __POINT_INTERNAL__
#error "Don't include Point_i.h directly. Include Point.h instead."
#endif

template<typename dataType>
std::ostream& operator<<(std::ostream& os, const Point<dataType>& point){
	os << point[0] << ", " << point[1] << ", " << point[2];
	return os;
}

template<typename dataType>
std::istream& operator>>(std::istream& in, Point<dataType>& point){
	for(unsigned int i = 0; i < 3; ++i){
		in >> point[i];
	}
	return in;
}

#define rhsOperatorFunction(op) \
    m_data[0] op rhs.m_data[0], m_data[1] op rhs.m_data[1], m_data[2] op rhs.m_data[2]

#define rhsOperatorFunctionSingle(op) \
    m_data[0] op rhs, m_data[1] op rhs, m_data[2] op rhs

template<typename dataType>
Point<dataType> Point<dataType>::operator+(const Point<dataType>& rhs){
	return {rhsOperatorFunction(+)};
}

template<typename dataType>
Point<dataType> Point<dataType>::operator-(const Point<dataType>& rhs){
	return {rhsOperatorFunction(-)};
}

template<typename dataType>
Point<dataType> Point<dataType>::operator/(dataType rhs){
	return {rhsOperatorFunctionSingle(/)};
}

template<typename dataType>
Point<dataType> Point<dataType>::operator*(dataType rhs){
	return {rhsOperatorFunctionSingle(*)};
}

#endif //SDFGEN_POINT_I_H