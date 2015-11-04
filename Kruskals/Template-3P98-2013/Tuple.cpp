#include "Tuple.h"


Tuple::Tuple(){
	this->_x = 0;
	this->_y = 0;
	this->_z = 0;
}

//3d constructor
Tuple::Tuple(int x, int y, int z){
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

//2d constructor
Tuple::Tuple(int x, int y){
	this->_x = x;
	this->_y = y;
	this->_z = 0;
}

//distance
int Tuple::calcDist(Tuple t){
	int c1 = this->_x - t._x;
	int c2 = this->_y - t._y;
	int c3 = this->_z - t._z;
	return (c1*c1) + (c2*c2) + (c3*c3);
}

