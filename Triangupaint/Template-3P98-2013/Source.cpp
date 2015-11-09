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
uniform_int_distribution<int> uni(50, 450);

vector<Point> point_store;
vector<Edge> hull_store;
vector<Triangle> triangle_store;
vector<Triangle> trisect_store;
 
int numPoints, input_mode, cog_index;

void clearVectors(){
	point_store.clear();
	hull_store.clear();
	triangle_store.clear();
	trisect_store.clear();
}


int initStatus(){
	cout << "Triangulation\n"
		<< "1: Enter N Random Data Points\n"
		<< "2: Select Points Using Mouse\n"
		<< "3: Select Points Using Lattice\n"
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
		cout << "Enter Number Of Points To Generate per row>\n"
			<< "INVALID INPUT DEFAULTS TO 10";
		cin >> numPoints;
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

void latticePoints(int n,int w, int h){
	int mul = w / n;
	for (int i = 1; i   < n + 1; i++){
		for (int j =1; j < n+1; j++){
			addPoint(i*mul/2, j*mul/2);
		}
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
	int dist = 100000000000000;
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
		if (Ti.testInterior(point_store, i) == INTERIOR_TO_T || Ti.testInterior(point_store, i)  == COLINEAR_TO_T
			&& Ti.testInterior(point_store, i) != VERTEX_OF_T){
			p = i;
			flag = Ti.testInterior(point_store, i);
			break;
		}
	}

	switch (flag){
	case -1:
		if (Ti._p1 != Ti._p2 && Ti._p1 != Ti._p3 &&Ti._p2 != Ti._p3)
		trisect_store.push_back(Ti);
		return;
		break;

	case INTERIOR_TO_T:
	{
						  flag = 1;
						  trisect(Triangle(Ti._p1, Ti._p2, p));
						  trisect(Triangle(Ti._p2, Ti._p3, p));
						  trisect(Triangle(Ti._p3, Ti._p1, p));
						  break;
	}

	case COLINEAR_TO_T:
	{
		flag = 1;

		Triangle points = Ti.getNonColinPoint(point_store, p);
		trisect(Triangle(points._p1, points._p2, p));
		trisect(Triangle(points._p1, points._p3, p));
		break;
	}

	}
}


///
///
///
///
int  triCleanUp(){
	Edge *e;
	int flag = 0;
	for (int i = 0; i < trisect_store.size(); i++){
		for (int j = 0; j < trisect_store.size(); j++){
			if (i != j &&  i < j){
				e = &trisect_store[i].sharedEdge(trisect_store[j]);
				if (e->_u != -1){
					Triangle &t1 = trisect_store[i];
					Triangle &t2 = trisect_store[j];
					int t1_p, t2_p;
					
					if (t1._p1 == e->_u && t1._p2 == e->_v){ t1_p = t1._p3; }
					if (t1._p2 == e->_u && t1._p1 == e->_v){ t1_p = t1._p3; }
					if (t1._p3 == e->_u && t1._p2 == e->_v){ t1_p = t1._p1; }
					if (t1._p2 == e->_u && t1._p3 == e->_v){ t1_p = t1._p1; }
					if (t1._p1 == e->_u && t1._p3 == e->_v){ t1_p = t1._p2; }
					if (t1._p3 == e->_u && t1._p1 == e->_v){ t1_p = t1._p2; }

					if (t2._p1 == e->_u && t2._p2 == e->_v){ t2_p = t2._p3; }
					if (t2._p2 == e->_u && t2._p1 == e->_v){ t2_p = t2._p3; }
					if (t2._p3 == e->_u && t2._p2 == e->_v){ t2_p = t2._p1; }
					if (t2._p2 == e->_u && t2._p3 == e->_v){ t2_p = t2._p1; }
					if (t2._p1 == e->_u && t2._p3 == e->_v){ t2_p = t2._p2; }
					if (t2._p3 == e->_u && t2._p1 == e->_v){ t2_p = t2._p2; }

					int d1 = abs(point_store[e->_u].distance(point_store[e->_v]));
					int d2 = abs(point_store[t1_p].distance(point_store[t2_p]));
					int p2 = distPointToLine(point_store[e->_u], point_store[t1_p], point_store[t2_p]);
					int p4 = distPointToLine(point_store[e->_v], point_store[t1_p], point_store[t2_p]);

					if (d2 < d1 && ((p2>0 && p4 < 0) || (p2<0 && p4 > 0))){
				
						trisect_store[i]._p1 = e->_u;
						trisect_store[i]._p2 = t1_p;
						trisect_store[i]._p3 = t2_p;

						trisect_store[j]._p1 = e->_v;
						trisect_store[j]._p2 = t1_p;
						trisect_store[j]._p3 = t2_p;
						flag++;

					}
				}
			}
		}
	}
	return flag;
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
		cout << "asscancer\t" << hull_store.size() << endl; 
		for (int i = 0; i < hull_store.size(); i++){
			if (i != cog_index){
				triangle_store.push_back(Triangle(hull_store[i]._u, hull_store[i]._v, cog_index));
			}
			
		}
		triangle_store.push_back(Triangle(hull_store[hull_store.size()-1]._v, hull_store[0]._u, cog_index));
		cout << "asscancer\t" << triangle_store.size() << endl;
	}

	for (auto t : triangle_store){
		trisect(t);
	}
	cout << trisect_store.size();

	int flag = 1;
	while (flag > 0){
		flag = triCleanUp();
		cout << flag << "\n";
	}

}

void drawTrisect_store(){

		glBegin(GL_TRIANGLES);
		for (auto t : trisect_store){
			glVertex2i(point_store[t._p1]._x, point_store[t._p1]._y);
			glVertex2i(point_store[t._p2]._x, point_store[t._p2]._y);
			glVertex2i(point_store[t._p3]._x, point_store[t._p3]._y);
		}
		glEnd();
		glFlush();
}

void drawHull(){
	if (point_store.size() > 3){
		glBegin(GL_LINE_LOOP);
		for (auto p : hull_store){

			glVertex2i(point_store[p._u]._x, point_store[p._u]._y);
			glVertex2i(point_store[p._v]._x, point_store[p._v]._y);
		}
		glEnd();
		glFlush();
	}
}





void display(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (point_store.size() > 0){
		glBegin(GL_POINTS);
		for (auto p : point_store){
			glVertex2i(p._x, p._y);
		}
		glEnd();
	}

	drawHull();
	drawTrisect_store();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 0x1B:
		case'q':
		case 'Q':
			exit(0);
			break;
		case 'm':{
				input_mode = -1;
				triangulate();
				glutPostRedisplay();
				break;
		}
	}
}//keyboard

void mouse(int btn, int state, int x, int y) {
	int yy;
	yy = glutGet(GLUT_WINDOW_HEIGHT);
	y = yy - y;   /* In Glut, Y coordinate increases from top to bottom */
	if (input_mode == INPUT_DATA){
		if (state == GLUT_DOWN) {
			addPoint(x, y);
			glutPostRedisplay();
		}
	}
}


int main(int argc, char** argv){
	int windowW = 500;
	int windowH = 500;
	input_mode = initStatus();
	if (input_mode == RANDOM_DATA){
		randomPoints(numPoints);
	}

	if (input_mode == LATTICE){
		latticePoints(numPoints, windowW, windowH);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Triangulate");
	glShadeModel(GL_SMOOTH);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 500, 0, 500, 0, 1);

	glutMainLoop();

	return 0;
}
