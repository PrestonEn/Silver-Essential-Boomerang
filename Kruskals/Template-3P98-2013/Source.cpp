/*
Preston Engstrom
pe12nh

*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <freeglut.h>
#include <FreeImage.h>

#include "Tuple.h"
#include "Edge.h"

void display(){

}

void makeEdges(){

}

bool comparision(const Edge& a, const Edge& b)
{
	// smallest comes first
	return a._wht < b._wht;
}


int main(int argc, char** argv)
{
	std::vector<Tuple> points_store;
	std::vector<Edge> edge_list;
	points_store.resize(10);

	for (int i = 0; i < 10; i++){
		points_store[i] = Tuple(rand() % 500, rand() % 500);
	}

	//edge_list.resize(45);
	int x = 0;
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			if (i<j){
				edge_list.push_back(Edge(i, j, &points_store));
				++x;
			}
		}
	}
	
	std::cout << edge_list.size() << "STUFF\n";
	std::sort(edge_list.begin(), edge_list.end(), comparision);
	
	/*for (auto n : edge_list) {
		std::cout << n._wht << "\n";
	}*/

	for (std::vector<Edge>::iterator i = edge_list.begin(); i != edge_list.end(); ++i){
		std::cout << i->_wht << "\n";
	}
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glutInitWindowSize(500, 500);
	//glutInitWindowPosition(100, 100);
	//glutCreateWindow(".::Conway's Game of Life::. The Glider");
	//init();
	//glutDisplayFunc(display);
	//glutMainLoop();

	getchar();
	return 0;
}