#include <vector>

#include <algorithm>
#include <random>
#include <iostream>
#include <freeglut.h>
#include <FreeImage.h>
#include "Edge.h"
#include "Point.h"
#include "Triangle.h"

using namespace std;

enum mode{
	RANDOM_DATA, INPUT_DATA, LATTICE
};

///INITIALIZE RANDOM NUMBER GENERATOR
random_device rd;     // only used once to initialise (seed) engine
mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
uniform_int_distribution<int> uni(50, 750);

vector<Point> point_store;
vector<Edge> hull_store;
vector<Triangle> triangle_store;
vector<Triangle> trisect_store;
int numPoints, input_mode, cog_index;

int initStatus(){
	cout << "Triangulation\n"
		<< "1: Enter N Random Data Points\n"
		<< "2: Select Points Using Mouse\n"
		<< "3: Select Points Using Mouse\n"
		<< "BAD INPUT DEFAULTS TO RANDOM POINTS\n"
		<< "Please Select From The Given Options>";
	int input;
	cin >> input;

	switch (input)
	{
	case 2:
		return INPUT_DATA;
		break;
	case 3:
		return LATTICE;
		break;
	default:
		cout << "Enter Number Of Points To Generate>\n"
			<< "INVALID INPUT DEFAULTS TO 100";
		cin >> numPoints;
		if (numPoints == 0){
			numPoints = 100;
		}
		return RANDOM_DATA;
		break;
	}
}

//signed distance of point
int distPointToLine(Point& P1, Point& P2, Point& p){
	int num = (P2._y - P1._y)*p._x - (P2._x - P1._x)*p._y + (P2._x*P1._y) - (P2._y*P1._x);
	int den = P1.distance(P2);
	if (den == 0){
		den = 1;
	}
	return num / (int)sqrt(den);
}

//Test uniqueness of point
//switch to hash function 
//in future branch
bool isUnique(Point& point){
	for (auto P : point_store){
		if (P._x == point._x && P._y == point._y){
			return false;
		}
	}
	return true;
}


void addPoint(int x, int y){
	Point p = Point(x,y);
	if (isUnique(p)){
		point_store.push_back(p);
	}	
}

void randomPoints(int n){
	for (int i = 0; i < n; i++){
		addPoint(uni(rng), uni(rng));
	}
}


void covexHull(vector<Point>& p_set, int u, int v){
	int d = 0;
	int pMax = -1;
	for (int i = 0; i < p_set.size(); i++){
		int dist = distPointToLine(point_store.at(u), point_store.at(v), point_store.at(i));
		if (dist > d){
			d = dist;
			pMax = i;
		}
	}
	if (pMax == -1){
		
		hull_store.push_back(Edge(u,v));

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

int closestPoint(Point p){
	int dist = 100000000;
	int index = -1;
	for (int i = 0; i < point_store.size(); i++){
		if (point_store[i].distance(p) < dist){
			dist = point_store[i].distance(p);
			index = i;
		}
	}
	return index;
}

int hullCOG(){
	int count = 0;
	int sumx, sumy;
	sumx = sumy=0;
	for (int i = 0; i < hull_store.size(); i++){
		sumx += point_store[hull_store[i]._u]._x;
		sumy += point_store[hull_store[i]._u]._y;
		count++;
	}
	return closestPoint(Point(sumx / count, sumy / count));
}


void trisect(Triangle& Ti){
//INTERIOR_TO_T, COLINEAR_TO_T, VERTEX_OF_T, NOT_IN_T
	
	int p;
	int flag = -1;
	for (int i = 0; i < point_store.size(); i++){
		flag = -1;
		cout << Ti.testInterior(point_store, i);
		switch (Ti.testInterior(point_store, i)){
			case INTERIOR_TO_T:
			{
				cout << " interior to t \n";
				trisect(Triangle(Ti._p1, Ti._p2, i));
				trisect(Triangle(Ti._p2, Ti._p3, i));
				trisect(Triangle(Ti._p3, Ti._p1, i));
				return;
				break;
			}

			case COLINEAR_TO_T:
			{
				cout << "colinear in t \n";
				flag = true;
				Triangle points = Ti.getNonColinPoint(point_store, i);
				trisect(Triangle(points._p1, points._p2, i));
				trisect(Triangle(points._p1, points._p3, i));
				return;
				break;
			}

			case NOT_IN_T:
				cout << "not in t \n";
				break;
			}
	}
		
	if (flag == -1){
		cout << "ADDING TO TRISECT_SET \n";
		trisect_store.push_back(Ti);
		return;
	}

}


///
///
///
///
bool triCleanUp(){
	Edge *e;
	for (int i = 0; i < trisect_store.size(); i++){
		for (int j = 0; j < trisect_store.size(); j++){
			if (i != j){
				e = &trisect_store[i].sharedEdge(trisect_store[j]);
				if (e->_u != -1){


				}
			}
		}
	}
	return true;
}


void triangulate(){
	sort(point_store.begin(), point_store.end(), [](Point p, Point q){return p._x < q._x; });
	quickHull(point_store, 0, point_store.size() - 1);
	//if there are enough eges so that no points
	// are interior to the hull, test for colinearity
	//select interior point
	if (point_store.size() == hull_store.size()){
		//must test if for edges e1 e2, if e1.v e2.u are colinear 

	}
	else{
		cog_index = hullCOG();
		for (int i = 0; i < hull_store.size(); i++){

			if (i != cog_index){
				triangle_store.push_back(Triangle(hull_store[i]._u, hull_store[i]._v, cog_index));
			}
			
		}
		triangle_store.push_back(Triangle(hull_store[hull_store.size()-1]._v, hull_store[0]._u, cog_index));
	}
	trisect(triangle_store[0]);
	for (auto t : triangle_store){
	trisect(t);
	}

	//triCleanUp();
}



void display(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);
	for (auto T : triangle_store){
		glColor3f(0.0, 1.0, 0.0);
		glVertex2i(point_store[T._p1]._x, point_store[T._p1]._y);
		glVertex2i(point_store[T._p2]._x, point_store[T._p2]._y);
		glVertex2i(point_store[T._p3]._x, point_store[T._p3]._y);
	}
	glEnd();
	glFlush();

	glBegin(GL_TRIANGLES);
	for (auto T : trisect_store){
		glColor3f(0.0, 0.0, 1.0);
		glVertex2i(point_store[T._p1]._x, point_store[T._p1]._y);
		glVertex2i(point_store[T._p2]._x, point_store[T._p2]._y);
		glVertex2i(point_store[T._p3]._x, point_store[T._p3]._y);
	}
	glEnd();
	glFlush();

	glPointSize(2);
	
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	for (auto P : hull_store){
		glVertex2i(point_store[P._u]._x, point_store[P._u]._y);
		glVertex2i(point_store[P._v]._x, point_store[P._v]._y);
	}
	glEnd();
	glFlush();

	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	for (auto P : point_store){
		glVertex2i(P._x,P._y);
	}
	glEnd();
	glFlush();

	glBegin(GL_POINTS);
	glColor3f(0.0, 1.0, 0.0);
		glVertex2i(point_store[cog_index]._x, point_store[cog_index]._y);
	glEnd();
	glFlush();
	
}

void mouse(int btn, int state, int x, int y) {
	int yy;
	yy = glutGet(GLUT_WINDOW_HEIGHT);
	y = yy - y;   /* In Glut, Y coordinate increases from top to bottom */
	if (input_mode == INPUT_DATA){
		if (state == GLUT_DOWN) {
			addPoint(x, y);
		}
	}
}


int main(int argc, char** argv){
	//input_mode = initStatus();
	randomPoints(150);
	triangulate();
	glutInit(&argc, argv);

	//quickHull(point_store , 0, point_store.size() - 1);


	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(750, 750);
	glutCreateWindow("Triangulate");
	glShadeModel(GL_SMOOTH);
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 0, 800, 0, 1);

	glutMainLoop();

	return 0;
}