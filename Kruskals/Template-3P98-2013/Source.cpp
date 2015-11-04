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
#include <random>

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(-400, 400); // guaranteed unbiased



std::vector<Tuple> points_store;
std::vector<Edge> edge_list;



	//for (std::vector<Edge>::iterator i = edge_list.begin(); i != edge_list.end(); ++i){
	//	std::cout << i->_wht << "\n";
	//}




//setup data in global Tuple vector
void makePoints(int n){
	points_store.resize(n);

	for (int i = 0; i < n; i++){
		points_store[i] = Tuple(uni(rng), uni(rng));
	}

}

//function for algo std::sort
bool comparision(const Edge& a, const Edge& b)
{
	return a._wht < b._wht;
}


void makeEdges(){
	//edge_list.resize(45);
	for (int i = 0; i < points_store.size(); i++){
		for (int j = points_store.size(); j < points_store.size(); j++){
			if (i<j){	//do not calculate edge (u,u) , or edge pair (u, v) (v, u)
				edge_list.push_back(Edge(i, j, &points_store));
			}
		}
	}

	std::sort(edge_list.begin(), edge_list.end(), comparision);
}

void drawStuff(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex3i(0, 0, 0);

	for (int i = 0; i < points_store.size(); i++){

		glVertex3i(points_store.at(i)._x,
			points_store.at(i)._y,
			points_store.at(i)._z);
	}
	glEnd();
	glutSwapBuffers();
	glFlush();
}

int main(int argc, char **argv) {
	makePoints(500);
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Glut rotate");


	glutDisplayFunc(drawStuff);
	glutIdleFunc(drawStuff);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-500.0, 500.0, -500.0, 500.0, -500.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();
}