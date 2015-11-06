#include "DisUnion.h"
#include "Tuple.h"
#include <vector>
#include <iostream>
DisUnion::DisUnion(int in){
	el_presidente = in;
	members.push_back(in);
}



void DisUnion::unionSets(DisUnion u, std::vector<Tuple> *points){
	this->members.insert(members.end(), u.members.begin(), u.members.end());
	for (int i = 0; i < members.size(); i++){
		points->at(members.at(i))._frst = el_presidente;
	}

}