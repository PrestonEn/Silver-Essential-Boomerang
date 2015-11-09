#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "Point.h"
#include "Edge.h"
#include <vector>
#include <algorithm>

enum {
	INTERIOR_TO_T, COLINEAR_TO_T, VERTEX_OF_T, NOT_IN_T
};

class Triangle
{
public:
	int _p1;
	int _p2;
	int _p3;

	Triangle(int p1, int p2, int p3);
	int Triangle::testInterior(std::vector<Point>& points, int p);
	Edge Triangle::sharedEdge(Triangle Ti);
	Triangle Triangle::getNonColinPoint(std::vector<Point>& points, int p);
};

Triangle::Triangle(int p1, int p2, int p3 )
{
	_p1 = p1;
	_p2 = p2;
	_p3 = p3;
}

int Triangle::testInterior(std::vector<Point>& points, int p){
	if (p == _p1 || p == _p2 || p == _p3){
		return VERTEX_OF_T;
	}

	Point v0 = points[_p3] - points[_p1];
	Point v1 = points[_p2] - points[_p1];
	Point v2 = points[p] - points[_p1];

	int d00 = v0.dotProd(v0);
	int d01 = v0.dotProd(v1);
	int d02 = v0.dotProd(v2);
	int d11 = v1.dotProd(v1);
	int d12 = v1.dotProd(v2);
	float invDen = 1.f / ((float)d00*(float)d11 - (float)d01*(float)d01);
	float u = ((float)d11*(float)d02 - (float)d01*(float)d12)*invDen;
	float v = ((float)d00*(float)d12 - (float)d01*(float)d02)*invDen;

	if (u >= 0 && v >= 0 && u + v < 1){
		if (points[p].colinear(points[_p1], points[_p2]) ||
			points[p].colinear(points[_p2], points[_p3]) ||
			points[p].colinear(points[_p3], points[_p1])){
			return COLINEAR_TO_T;
		}
		else{
			return INTERIOR_TO_T;
		}
	}
	else{
		return NOT_IN_T;
	}
}


Edge Triangle::sharedEdge(Triangle Ti){
	if (_p1 == Ti._p1 && _p2 == Ti._p2){
		return Edge(_p1, _p2);
	}

	if (_p2 == Ti._p2 && _p3 == Ti._p3){
		return Edge(_p2, _p3);
	}

	if (_p1 == Ti._p1 && _p3 == Ti._p3){
		return Edge(_p1, _p3);
	}
	return Edge(-1, -1);
}

Triangle Triangle::getNonColinPoint(std::vector<Point>& points, int p){
	bool p1, p2, p3;
	if (points[p].colinear(points[_p1], points[_p2])){
		p1 = false;
		p2 = false;
	}
	if(points[p].colinear(points[_p2], points[_p3])){
		p2 = false;
		p3 = false;
	}
	if(points[p].colinear(points[_p3], points[_p1])){
		p1 = false;
		p3 = false;
	}
	//first element is noncoliner point
	if (p1){ return Triangle(_p1, _p2, _p3); }
	if (p2){ return Triangle(_p2, _p1, _p3); }
	if (p3){ return Triangle(_p3, _p1, _p2); }
}


#endif
