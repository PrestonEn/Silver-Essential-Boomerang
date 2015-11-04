/*
Preston Engstrom
pe12nh

*/

#include <stdlib.h>
#include <iostream>
#include <vector>	//vector	
#include <algorithm>	//sort
#include <freeglut.h>
#include <FreeImage.h>
#include <random>	//rng
#include "Tuple.h"
#include "Edge.h"


std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(-150, 150); // guaranteed unbiased



std::vector<Tuple> points_store;
std::vector<Edge> edge_list;
float angle = 0.0;



	//for (std::vector<Edge>::iterator i = edge_list.begin(); i != edge_list.end(); ++i){
	//	std::cout << i->_wht << "\n";
	//}




//setup data in global Tuple vector
void makePoints(int n){
	points_store.resize(n);

	for (int i = 0; i < n; i++){
		points_store[i] = Tuple(uni(rng), uni(rng), uni(rng));
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
	glRotatef(angle, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(5.0f);
	
	glBegin(GL_LINES);
		glVertex3i(100,-100,-100);
		glVertex3i(100, 100, 100);
	glEnd();

	glBegin(GL_POINTS);
		for (int i = 0; i < points_store.size(); i++){
			glVertex3i(points_store.at(i)._x,
					   points_store.at(i)._y,
					   points_store.at(i)._z);
		}
	glEnd();
	glutSwapBuffers();
	glFlush();
}
void mouse(int btn, int state, int x, int y) {

	printf("%3d, %3d, %f\n", btn, state, angle);
	if (state == GLUT_DOWN) {
		if (btn == GLUT_LEFT_BUTTON) {
			angle = angle + 1.0;
		}
		else if (btn == GLUT_RIGHT_BUTTON) {
			angle = angle - 1.0;
		}
		else {
			angle = 0.0;
		}
	}
}
int main(int argc, char **argv) {
	makePoints(500);
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Glut rotate");


	glutDisplayFunc(drawStuff);
	glutIdleFunc(drawStuff);
	glutMouseFunc(mouse);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-200.0, 200.0, -200.0, 200.0, -200.0, 200.0);

	glMatrixMode(GL_MODELVIEW);

	//glRotatef(30.0, 0.0, 0.0, 0.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();
}