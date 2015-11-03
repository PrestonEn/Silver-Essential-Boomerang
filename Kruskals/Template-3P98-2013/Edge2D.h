#ifndef _EDGE_H_
#define _EDGE_H_

#include "Tuple2D.h"


class Edge2D{
private:
	Tuple2D _u;
	Tuple2D _v;
	int _wht;

public:
	Edge2D::Edge2D(Tuple2D u, Tuple2D v);
	
	void Edge2D::printEdge();

	Tuple2D Edge::get

	int Edge2D::getWht();
};

#endif