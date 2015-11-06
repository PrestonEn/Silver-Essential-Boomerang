#include "Tuple.h"


Tuple::Tuple(){
	this->_x = 0;
	this->_y = 0;
	this->_z = 0;
}

//3d constructor
Tuple::Tuple(int x, int y, int z, int f){
	this->_x = x;
	this->_y = y;
	this->_z = z;
	_frst = f;
}

//2d constructor
Tuple::Tuple(int x, int y, int f){
	this->_x = x;
	this->_y = y;
	this->_z = 0;
	_frst = f;
}

//distance
int Tuple::calcDist(Tuple t){
	int c1 = this->_x - t._x;
	int c2 = this->_y - t._y;
	int c3 = this->_z - t._z;
	return (c1*c1) + (c2*c2) + (c3*c3);
}

