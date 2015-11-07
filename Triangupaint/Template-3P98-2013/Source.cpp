#include <vector>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include "Edge.h"
#include "Point.h"
#include "Triangle.h"

using namespace std;
///INITIALIZE RANDOM NUMBER GENERATOR
random_device rd;     // only used once to initialise (seed) engine
mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
uniform_int_distribution<int> uni(50, 700);

vector<Point> point_store;

vector<Edge> edge_store;
Edge split_edge;
Point* p_maxX;
Point* p_minX;


int distPointToLine(Point& P1, Point& P2, Point& p){
	int num = abs((P2._y - P1._y)*p._x - (P2._x - P1._x)*p._y + (P2._x*P1._y) - (P2._y*P1._x));
	int den = P1.distance(P2);
	return num / (int) sqrt(den);
}


bool isUnique(Point& point){
	for (std::vector<Point>::iterator i = point_store.begin(); i != point_store.end(); ++i){
		if (i->_x == point._x && i->_y == point._y ){
			return false;
		}
	}
	return true;
}


void genPoints(int n){
	int minX = 100000;
	int maxX = -100000;
	int i = 0;
	while (i < n){
		Point x = Point(uni(rng), uni(rng));
		if (isUnique(x)){
			if (x._x > maxX){
				maxX = x._x;
				p_maxX = &x;
			}else if(x._x < minX){
				minX = x._x;
				p_minX = &x;
			}

			i++;
			point_store.push_back(x);
		}
	}
}


void convHullTop(){

}


void display(){
	glBegin(GL_POINTS);
	for (std::vector<Point>::iterator i = point_store.begin(); i != point_store.end(); ++i){
		glVertex2i(i->_x, i->_y);
	}
	glEnd();
	glFlush();
}


int main(int argc, char** argv){

	glutInit(&argc, argv);
	genPoints(200);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(750, 750);
	glutCreateWindow("SIMPLE DISPLAY");
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(display);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 750, 0, 750, 0, 1);

	glutMainLoop();

	return 0;
}