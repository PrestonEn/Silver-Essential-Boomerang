/*
 *	Preston Engstrom
 *	pe12nh
 *
 */
#include <stdlib.h>
#include <iostream>
#include <vector>		//vector	
#include <algorithm>	//sort
#include <freeglut.h>
#include <FreeImage.h>
#include <random>		//rng
#include "Tuple.h"
#include "Edge.h"


std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(-150, 150); // guaranteed unbiased
std::vector<Tuple> points_store;
std::vector<Edge> edge_list;

float angle = 0.0;

int xpos;
int ypos;

bool isUnique(Tuple point){
	for (std::vector<Tuple>::iterator i = points_store.begin(); i != points_store.end(); ++i){
		if (i->_x == point._x && i->_y == point._y && i->_z && point._z){
			return false;
		}
	}
	return true;
}


//	setup data in global Tuple vector
//	
void makePoints(int n){
	for (int i = 0; i < n; i++){
		Tuple x = Tuple(uni(rng), uni(rng), uni(rng));
		if (isUnique(x)){
			points_store.push_back(x);
		}
		else{
			std::cout << "nonunique \n";
		}
	}
}


//function for algo std::sort
bool comparision(const Edge& a, const Edge& b)
{
	return a._wht < b._wht;
}


void makeEdges(){
	for (int i = 0; i < points_store.size(); i++){
		for (int j = 0; j < points_store.size(); j++){
			if (i<j){	//do not calculate edge (u,u) , or edge pair (u, v) (v, u)
				edge_list.push_back(Edge(i, j, &points_store));
			}
		}
	}
	std::cout << edge_list.size();
	//std::sort(edge_list.begin(), edge_list.end(), comparision);
}


void drawStuff(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(5.0f);
	glLineWidth(4);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	//draw the unit vectors
	glVertex3i(0, 0, 0);
	glVertex3i(15, 0, 0);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	//draw the unit vectors
	glVertex3i(0, 0, 0);
	glVertex3i(0, 15, 0);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	//draw the unit vectors
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 15);
	glEnd();

	glLineWidth(3);
	glColor3ub(51, 255, 102);
	glBegin(GL_LINES);

	


	for (int i = 0; i < edge_list.size(); i++){
		glVertex3i(points_store.at(edge_list.at(i)._u)._x,
			points_store.at(edge_list.at(i)._u)._y,
			points_store.at(edge_list.at(i)._u)._z);

		glVertex3i(points_store.at(edge_list.at(i)._v)._x,
			points_store.at(edge_list.at(i)._v)._y,
			points_store.at(edge_list.at(i)._v)._z);
	}
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

	//printf("%3d, %3d, %f\n", btn, state, angle);
	//if (state == GLUT_DOWN) {
	//	if (btn == GLUT_LEFT_BUTTON) {
	//		angle = angle + 1.0;
	//	}
	//	else if (btn == GLUT_RIGHT_BUTTON) {
	//		angle = angle - 1.0;
	//	}
	//	else {
	//		angle = 0.0;
	//	}
	//}
}
//naive rotation on mouse
void motion(int x, int y) {
	std::cout << x << "\t" << y << "\n";
	if (ypos - y < 0){
		glRotatef(2.0, 1.0, 0.0, 0.0);
	}
	if (xpos - x < 0){
		glRotatef(2.0, 0.0, 1.0, 0.0);
	}

	if (ypos - y > 0){
		glRotatef(-2.0, 1.0, 0.0, 0.0);
	}
	if (xpos - x > 0){
		glRotatef(-2.0, 0.0, 1.0, 0.0);
	}
	xpos = x;
	ypos = y;


}

int main(int argc, char **argv) {
	makePoints(10);
	makeEdges();

	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Glut rotate");


	glutDisplayFunc(drawStuff);
	glutIdleFunc(drawStuff);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-200.0, 200.0, -200.0, 200.0, -200.0, 200.0);

	glMatrixMode(GL_MODELVIEW);

	//glRotatef(30.0, 0.0, 0.0, 0.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();
}