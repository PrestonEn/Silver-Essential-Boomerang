#include "Edge.h"

Edge::Edge(){
	_u = 0;
	_v = 0;
	_wht = 0;
}

Edge::Edge(int u, int v, int wht){
	_u = u;
	_v = v;
	_wht = wht;
}
