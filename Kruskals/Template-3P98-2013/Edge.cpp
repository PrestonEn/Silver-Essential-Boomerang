#include "Edge.h"

Edge::Edge(){
	_u = 0;
	_v = 0;
	_wht = 0;
}

Edge::Edge(int u, int v, std::vector<Tuple>* points){
	_u = u;
	_v = v;
	_wht = this->setWht(points);
}

int Edge::setWht(std::vector<Tuple>* v){
	return v->at(this->_u).calcDist(v->at(this->_v));
}
