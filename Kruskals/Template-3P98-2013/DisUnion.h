#ifndef _DISU_H_
#define _DISU_H_

#include <vector>
#include "Tuple.h"

class DisUnion{
public:
	int el_presidente;	//representative member of the set
	int size;


	std::vector<int> members;

	DisUnion::DisUnion(int ins);

	int DisUnion::find(int search, std::vector<Tuple> *points);

	//NOTE: Elpresidente of this union is stronger!
	//IE: the representative of the reulting union is
	//the same as the set calling UnionSet
	void DisUnion::unionSets(DisUnion other, std::vector<Tuple> *points);

};

#endif