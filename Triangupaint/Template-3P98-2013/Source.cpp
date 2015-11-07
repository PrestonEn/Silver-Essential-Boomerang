#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
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
vector<Edge> hull_store;

vector<int> hull_set;

int distPointToLine(Point& P1, Point& P2, Point& p){
	int num = (P2._y - P1._y)*p._x - (P2._x - P1._x)*p._y + (P2._x*P1._y) - (P2._y*P1._x);
	int den = P1.distance(P2);
	return num / (int)sqrt(den);
}

//test if point is left/right/colinear to line
double crossProd(Point& P1, Point& P2, Point& P3){
	cout << P1._x << "\t" << P1._y << "\t" << P2._x << "\t" << P2._y << "\t" << P3._x << "\t" << P3._y << "\n";
	cout << (long)(P2._x - P1._x)*(P3._y - P1._y) - (long)(P2._y - P1._y)*(P3._x - P1._x) << "\n";
	return (long)(P2._x - P1._x)*(P3._y - P1._y) - (long)(P2._y - P1._y)*(P3._x - P1._x);
}

bool isUnique(Point& point){
	for (std::vector<Point>::iterator i = point_store.begin(); i != point_store.end(); ++i){
		if (i->_x == point._x && i->_y == point._y){
			return false;
		}
	}
	return true;
}


void genPoints(int n){
	int i = 0;
	while (i < n){
		Point x = Point(uni(rng), uni(rng));
		if (isUnique(x)){
			i++;
			point_store.push_back(x);
		}
	}
	sort(point_store.begin(), point_store.end(), [](Point p, Point q){return p._x < q._x; });
}


void covexUpperHull(vector<Point> p_set, int u, int v){
	int d = 0;
	int pMax = -1;
	for (int i = 0; i < p_set.size(); i++){
		int dist = distPointToLine(point_store.at(u), point_store.at(v), point_store.at(i));
		if (dist > d){
			cout << "updated pMax\n";
			d = dist;
			pMax = i;
		}

	}
	if (pMax == -1){
		cout << "added edge\n";
		hull_store.push_back(Edge(u, v));
		return;
	}
	else{
		covexUpperHull(p_set, u, pMax);
		covexUpperHull(p_set, pMax, v);
	}
}



void display(){

}


int main(int argc, char** argv){

	glutInit(&argc, argv);
	genPoints(200);
	covexUpperHull(point_store, point_store.size() - 1, 0);
	covexUpperHull(point_store,0, point_store.size() - 1);

	cout << edge_store.size();
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