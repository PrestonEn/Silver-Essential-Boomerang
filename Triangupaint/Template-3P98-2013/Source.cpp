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
vector<int> hull_store;
vector<Triangle> triangle_store;
vector<Triangle> trisect_store;
int numPoints, input_mode;

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

//if 0, p3 is colinear to vector(O,P2)
double crossProd(Point& O, Point& P2, Point& P3){
	return (long)(P2._x - O._x)*(P3._y - O._y) - (long)(P2._y - O._y)*(P3._x - O._x);
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
		hull_store.push_back(u);
		hull_store.push_back(v);
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
	int dist = 1000000;
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
		sumx += point_store[hull_store[i]]._x;
		sumy += point_store[hull_store[i]]._y;
		count++;
	}
	return closestPoint(Point(sumx / count, sumy / count));
}


void trisect(Triangle& Ti){
	int sumX = point_store[Ti._p1]._x +
			   point_store[Ti._p2]._x +
			   point_store[Ti._p3]._x;
	
	int sumY = point_store[Ti._p1]._y +
		       point_store[Ti._p2]._y +
			   point_store[Ti._p3]._y;
	
	int p =  closestPoint(Point(sumX/3, sumY/3));

	if (!Ti.testInterior(point_store, p)){
		trisect_store.push_back(Ti);
		return;
	}
	else{
		trisect(Triangle(Ti._p1, Ti._p2, p));
		trisect(Triangle(Ti._p2, Ti._p3, p));
		trisect(Triangle(Ti._p3, Ti._p1, p));
	}
}

bool triCleanUp(){
	bool change_made = false;
	for (int i = 0; i < trisect_store.size() - 2; i++){
		for (int j = i+1; j < trisect_store.size() - 1; j++){
			if (i != j){
				Edge test = trisect_store[i].sharedEdge(trisect_store[j]);
				if (test._u >= 0 && i != j){

					int ti_p, tj_p;
					int d1, d2, s1, s2;

					if (trisect_store[i]._p1 != test._u && trisect_store[i]._p1 != test._v){ ti_p = trisect_store[i]._p1; }
					else if (trisect_store[i]._p2 != test._u && trisect_store[i]._p2 != test._v){ ti_p = trisect_store[i]._p2; }
					else{ ti_p = trisect_store[i]._p3; }

					if (trisect_store[j]._p1 != test._u && trisect_store[j]._p1 != test._v){ tj_p = trisect_store[j]._p1; }
					else if (trisect_store[j]._p2 != test._u && trisect_store[j]._p2 != test._v){ tj_p = trisect_store[j]._p2; }
					else { tj_p = trisect_store[j]._p3; }

					d1 = abs(point_store[test._u].distance(point_store[test._v]));
					d2 = abs(point_store[ti_p].distance(point_store[tj_p]));
					s1 = distPointToLine(point_store[ti_p], point_store[tj_p], point_store[test._u]);
					s2 = distPointToLine(point_store[ti_p], point_store[tj_p], point_store[test._v]);

					bool op_signs = (s1 > 0 && s2 < 0 || s1 < 0 && s2 > 0);
					if (d1 > d2 && op_signs){
						change_made = true;
						trisect_store[i] = Triangle(test._u, ti_p, tj_p);
						trisect_store[j] = Triangle(test._v, ti_p, tj_p);
					}
				}
				}
			}
		}
	return change_made;
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
		int cog_index = hullCOG();
		int j;
		for (int i = 0; i < hull_store.size(); i++){
			if (i == hull_store.size()-1){ j = 0; }
			else{
				j = i + 1;
			}
			triangle_store.push_back(Triangle(hull_store[i], hull_store[j], cog_index));
		}
	}
	for (auto T : triangle_store){
		trisect(T);
	}
	bool flag = true;
	while (flag){
		flag = triCleanUp();
	}
}




void display(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (auto T : trisect_store){
		glVertex2i(point_store[T._p1]._x, point_store[T._p1]._y);
		glVertex2i(point_store[T._p2]._x, point_store[T._p2]._y);
		glVertex2i(point_store[T._p3]._x, point_store[T._p3]._y);
	}
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
	randomPoints(1000);
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