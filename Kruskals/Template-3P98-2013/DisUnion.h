#ifndef _DISU_H_
#define _DISU_H_

#include <vector>


class DisUnion{
public:
	int el_presidente;	//representative member of the set
	int size;
	std::vector<DisUnion> *unions;

	std::vector<int> members;

	DisUnion::DisUnion(int in, std::vector<DisUnion> *all_unions);

	int DisUnion::find(int search);

	//NOTE: Elpresidente of this union is stronger!
	//IE: the representative of the reulting union is
	//the same as the set calling UnionSet
	void DisUnion::unionSets(DisUnion other);

};

#endif