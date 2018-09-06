//
// Created by Maximilian Denninger on 9/3/18.
//

#ifndef SDFGEN_LINE_H
#define SDFGEN_LINE_H


struct Line {
	Line() = default;

	Line(Point3D& first, Point3D& second): m_first(&first), m_second(&second) {
		m_normal = *m_second - *m_first;
		m_normal.normalizeThis();
	};

	double getDist(const dPoint& toPoint){
		const auto vecToPoint = *m_first - toPoint;
		return (vecToPoint - m_normal * dot(vecToPoint, m_normal)).length();
	}

	Point3D* m_first;
	Point3D* m_second;
	dPoint m_normal;
};


#endif //SDFGEN_LINE_H
