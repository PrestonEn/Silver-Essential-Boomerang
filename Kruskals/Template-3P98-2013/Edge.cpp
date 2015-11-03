#include "Edge2D.h"



Edge2D::Edge2D(Tuple2D u, Tuple2D v){
	_u = u;
	_v = v;
	_wht = u.calcDist(v);
}

int Edge2D::getWht(){
	return _wht;
}

void Edge2D::printEdge(){

}
