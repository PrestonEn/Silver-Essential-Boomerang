class Point
{
public:
	int _x, _y;
	Point();
	Point(int x, int y);


	int Point::distance(Point& other);

};

Point::Point(int x, int y){
	_x = x;
	_y = y;
}

int Point::distance(Point& other){
	return (_x - other._x)*(_x - other._x) + (_y - other._y)*(_y - other._y);
}