#pragma once

class Edge
{
public:
	int _u, _v;
	Edge(int u, int v);

};

Edge::Edge(int u, int v)
{
	_u = u;
	_v = v;
}

