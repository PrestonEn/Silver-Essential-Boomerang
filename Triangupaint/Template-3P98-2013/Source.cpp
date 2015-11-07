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

int distPointToLine(Point& P1, Point& P2, Point& p){
	int num = (P2._y - P1._y)*p._x - (P2._x - P1._x)*p._y + (P2._x*P1._y) - (P2._y*P1._x);
	int den = P1.distance(P2);
	return num / (int)sqrt(den);
}

//if 0, p3 is colinear to O,P2
double crossProd(Point& O, Point& P2, Point& P3){
	return (long)(P2._x - O._x)*(P3._y - O._y) - (long)(P2._y - O._y)*(P3._x - O._x);
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


void covexHull(vector<Point> p_set, int u, int v){
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
		covexHull(p_set, u, pMax);
		covexHull(p_set, pMax, v);
	}
}

void quickHull(vector<Point> p_set, int u, int v){
	covexHull(point_store, point_store.size() - 1, 0);
	covexHull(point_store, 0, point_store.size() - 1);
}





void display(){

}


int main(int argc, char** argv){

	glutInit(&argc, argv);
	genPoints(200);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(750, 750);
	glutCreateWindow("Triangulate");
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(display);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 750, 0, 750, 0, 1);

	glutMainLoop();

	return 0;
}