#ifndef _TUPLE_H_
#define _TUPLE_H_

class Tuple{
public:
	int _x;
	int _y;
	int _z;
	Tuple::Tuple(); //default makes (0, 0, 0)
	Tuple::Tuple(int x, int y, int z);
	Tuple::Tuple(int x, int y); //2D

	int calcDist(Tuple t);
	
};

#endif