#ifndef _EDGE_H_
#define _EDGE_H_

//in the intrest of simplicity, points are assumed to
//be stored in a vector<Tuple> and referenced by
//index.

#include "Tuple.h"
#include <vector>

class Edge{
public:
	int _u;	//index of point
	int _v; //index of point
	int _wht; //dist of point

	Edge::Edge();

	Edge::Edge(int u, int v, int wht);
	
	void Edge::printEdge();

	Tuple Edge::getU();

	Tuple Edge::getV();

	int Edge::setWht(std::vector<Tuple>* points);
};

#endif