#include "DisUnion.h"
#include <vector>

DisUnion::DisUnion(int in, std::vector<DisUnion> *all_unions){
	el_presidente = in;
	members.push_back(in);
	unions = all_unions;
}

int DisUnion::find(int search){
	for (int i = 0; i < members.size(); i++){
		if (members.at(i) == search)
			return i;
	}
	return -1;
}

void DisUnion::unionSets(DisUnion u){
	this->members.insert(members.end(), u.members.begin(), u.members.end());
}