#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "Point.h"
#include "Edge.h"
#include <vector>
#include <algorithm>
class Triangle
{
public:
	int _p1;
	int _p2;
	int _p3;

	Triangle(int p1, int p2, int p3);
	bool Triangle::testInterior(std::vector<Point>& points, int p);
	Edge Triangle::sharedEdge(Triangle Ti);
	
};

Triangle::Triangle(int p1, int p2, int p3 )
{
	_p1 = p1;
	_p2 = p2;
	_p3 = p3;
}


bool Triangle::testInterior(std::vector<Point>& points, int p){
	Point* p0 = &points[_p1];
	Point* p1 = &points[_p2];	
	Point* p2 = &points[_p3];	
	Point* pp = &points[p];
	double area = 1 / 2 * (-p1->_y*p2->_x + p0->_y*(-p1->_x + p2->_x) + p0->_x*(p1->_y - p2->_y) + p1->_x*p2->_y);
	double s = 1 / (2 * area)*(p0->_y*p2->_x - p0->_x*p2->_y + (p2->_y - p0->_y) * pp->_x + (p0->_x - p2->_x)*pp->_y);
	double t = 1 / (2 * area)*(p0->_x*p1->_y - p0->_y*p1->_x + (p0->_y - p1->_y)*pp->_x + (p1->_x - p0->_x)*pp->_y);
	return (s > 0 && t > 0 && (s + t) > area);

}

Edge Triangle::sharedEdge(Triangle Ti){
	int thisT[3] = { _p1, _p2, _p3 };
	int TiT[3] = { Ti._p1, Ti._p2, Ti._p3 };
	int pot_edge[2];
	int index = 0;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (thisT[i] == TiT[j]){
				pot_edge[index] = thisT[i];
				index++;
			}
			if (index > 1){
				return Edge(pot_edge[0], pot_edge[1]);
			}
		}
	}
	return Edge(-1, -1);
}
#endif
