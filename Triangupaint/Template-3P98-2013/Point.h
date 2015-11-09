#ifndef _POINT_H_
#define _POINT_H_

class Point
{
public:
	int _x, _y;
	Point();
	Point(int x, int y);


	int Point::distance(Point& other);
	bool Point::colinear(Point& p1, Point& p2);
	int Point::dotProd(Point& p);
	Point operator-(const Point& p);
};

Point::Point(int x, int y){
	_x = x;
	_y = y;
}

Point Point::operator-(const Point& p){
	int x = _x - p._x;
	int y = _y - p._y;
	return Point(x, y);
}

int Point::distance(Point& other){
	return (_x - other._x)*(_x - other._x) + (_y - other._y)*(_y - other._y);
}

bool Point::colinear(Point& p1, Point& p2){
	return (_x*(p1._y - p2._y) + p1._x*(p2._y - _y) + p2._x * (_y - p1._y)==0); 
}

int Point::dotProd(Point& p){
	return _x*p._x + _y*p._y;
}
#endif