/**
 *	Preston Engstrom
 *	pe12nh
 *
 *
 *
 *
 */
#include <stdlib.h>
#include <iostream>
#include <vector>	
#include <algorithm>
#include <freeglut.h>
#include <FreeImage.h>
#include <random>

#include "Tuple.h"
#include "Edge.h"
#include "DisUnion.h"

enum DIM_MODE
{
	_2D_MODE_, _3D_MODE_
};

///INITIALIZE RANDOM NUMBER GENERATOR
 std::random_device rd;     // only used once to initialise (seed) engine
 std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
 std::uniform_int_distribution<int> uni(-75, 75);
///VECTOR
 std::vector<Tuple> points_store;
 std::vector<DisUnion> union_store;
 std::vector<Edge> edge_list;
 std::vector<Edge> kst_edges;

 float angle = 0.0;
 int xpos;
 int ypos;
 int sum_of_weights;

bool isUnique(Tuple point){
	for (std::vector<Tuple>::iterator i = points_store.begin(); i != points_store.end(); ++i){
		if (i->_x == point._x && i->_y == point._y && i->_z && point._z){
			return false;
		}
	}
	return true;
}



void makePoints(int n){
	int i = 0;
	while (i < n){
		Tuple x = Tuple(uni(rng), uni(rng), uni(rng), i);
		if (isUnique(x)){
			i++;
			points_store.push_back(x);
			//Each point starts as a distinct union
			union_store.push_back(DisUnion(points_store.size() - 1));
		}
	}
}


//function for algo std::sort
bool comparision(const Edge& a, const Edge& b)
{
	return a._wht < b._wht;
}

//given an initial set of points, create a list of edges
void makeEdges(){
	for (int i = 0; i < points_store.size()-1; i++){
		for (int j = i+1; j < points_store.size(); j++){

				int wht = points_store.at(i).calcDist(points_store.at(j));
				edge_list.push_back(Edge(i, j, wht));
		}
	}
	std::cout << edge_list.size() << "\n";
	std::sort(edge_list.begin(), edge_list.end(), comparision);
}

//generate the tuple based on x y coodinates
//Note that x and y must be mapped from the 
//values returned by the mouse function
//to values within the range of our
//orthonginal veiw
//void addNewPoint(int x, int y){
//
//	Tuple newTuple = Tuple(x, y);
//	for (int i = 0; i < points_store.size(); i++){
//		edge_list.push_back(Edge(i, points_store.size(), &points_store));
//	}
//	points_store.push_back(newTuple);
//	union_store.push_back(DisUnion(points_store.size() - 1, &union_store));
//}

//search over the set of disjoint unions for



void krustyTheKruskals(){
	for (int i = 0; i < edge_list.size(); i++){
		int one = points_store.at(edge_list.at(i)._u)._frst;
		int two = points_store.at(edge_list.at(i)._v)._frst;
		if (one != two){
			union_store.at(one).unionSets(union_store.at(two), &points_store);
			kst_edges.push_back(edge_list.at(i));
		}
	}
}

void drawUnitVectors(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(5.0f);
	glLineWidth(4);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(15, 0, 0);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 15, 0);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 15);
	glEnd();

}


void drawStuff(){
	drawUnitVectors();
	glLineWidth(1);
	glColor3ub(51, 255, 102);
	glBegin(GL_LINES);

	for (int i = 0; i < kst_edges.size(); i++){
		glColor3ub(points_store.at(kst_edges.at(i)._u)._x, points_store.at(kst_edges.at(i)._u)._y, points_store.at(kst_edges.at(i)._u)._z);
		glVertex3i(points_store.at(kst_edges.at(i)._u)._x,
			points_store.at(kst_edges.at(i)._u)._y,
			points_store.at(kst_edges.at(i)._u)._z);

		glVertex3i(points_store.at(kst_edges.at(i)._v)._x,
			points_store.at(kst_edges.at(i)._v)._y,
			points_store.at(kst_edges.at(i)._v)._z);
	}
	glEnd();

	//glBegin(GL_POINTS);
	//for (int i = 0; i < points_store.size(); i++){
	//	glVertex3i(points_store.at(i)._x,
	//		points_store.at(i)._y,
	//		points_store.at(i)._z);
	//}
	//glEnd();
	glutSwapBuffers();
	glFlush();
}
void mouse(int btn, int state, int x, int y) {

 
}
//naive rotation on mouse
void motion(int x, int y) {
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

	//if (x < 300 || x > 500 || y < 300 || y > 500){
	//	glutWarpPointer(400, 400);
	//	xpos = 0;
	//	ypos = 0;
	//}
}

void keyboard(unsigned char key, int x, int y){
	if (key == '+'){
		glScaled(0.0, 0.0, 10.0);
	}
}

int main(int argc, char **argv) {
	makePoints(5000);
	makeEdges();
	krustyTheKruskals();
	std::cout << sizeof(&points_store);
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Glut rotate");

	//glutSetCursor(GLUT_CURSOR_NONE);
	glutDisplayFunc(drawStuff);
	glutIdleFunc(drawStuff);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-200.0, 200.0, -200.0, 200.0, -200.0, 200.0);

	glMatrixMode(GL_MODELVIEW);

	//glRotatef(30.0, 0.0, 0.0, 0.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();
}