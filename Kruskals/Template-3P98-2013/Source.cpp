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
std::uniform_int_distribution<int> uni(-400, 400);

///VECTOR
std::vector<Tuple> points_store;
std::vector<DisUnion> union_store;
std::vector<Edge> edge_list;
std::vector<Edge> kst_edges;

float angle = 0.0;
int xpos;
int ypos;
int sum_of_weights;


bool isUnique(Tuple point)
{
	for (std::vector<Tuple>::iterator i = points_store.begin(); i != points_store.end(); ++i){
		if (i->_x == point._x && i->_y == point._y && i->_z && point._z){
			return false;
		}
	}
	return true;
}


void makePoints(int n)
{
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
void makeEdges()
{
	for (int i = 0; i < points_store.size()-1; i++){
		for (int j = i+1; j < points_store.size(); j++){

				int wht = points_store.at(i).calcDist(points_store.at(j));
				edge_list.push_back(Edge(i, j, wht));
		}
	}
	std::cout << edge_list.size() << "\n";
	std::sort(edge_list.begin(), edge_list.end(), comparision);
}


void krustyTheKruskals()
{
	std::cout << "            6B@(7\n          e###KK#RRe\n          /QK####BBs/sQ\n    ^       eBBsQ///^///(      ~B@##R\n  K###@7  ^//////////////O   e#####R~\n  K#####  @///////////////( @K#####@\n /(##R##e #////////%////////#K####K@\n   G##R#RBBR@%//////7   @    B#KKK#\n   ##########R@///C      G  t B#K#~    BROCK UNIVERSITY IS NOT A CLOWN COLLEGE\n    ~@###RR#R#R@//R    /   ###BKKK    /\n     ####RR#R##R@//s/   Q%OOOO@#@    /\n      BR#R###@7#//e7t#CQ(/CQC///(Q \n       @####@////Q/~e//~//////////(\n        RBR###B///~~ ~Q~~/////~Q^\n          7R^  ///R~@@@/ @ ^ e\n               ////S%@@@@@@~/\n              R7////~RO#@@(G\n             (R#R@C~R~ROO@(^\n             @####RR@@%~~~BR\n            ^@RR#RR@RRR@@RQ@\n          (#@#@@RR#R@B@R@RRR\n        QRR#RR@RR@@@RR@R@@@@\n       RR#RRRRR@RR#RRRRR#RR#RG\n      RR#RRRRRRR@RRRRRRRRRRR@R#\n     ~RRRRRRRRRRR@RRR##RRR#RRR@\n     @#RRRRRRRRRR@RRR#RRRRR#RR@\n      B@RR#RRR#RR@RRRRRRRRRR#@B\n      S7%%%%77%@RRRRRRRRRRRRRRR\n      @%%%%%%%%@RRRRR#RR#R#RR#R@\n     @@%77%%%%%#RRRRRRRRR###RRRRG\n    /R@C77%%%%%@RRRRRRRRRRRRRR#R@/\n    RR#@%7%%%%%R#R#RRRRRR#####RR#@\n   G#RR@%%777%7RRRRRR#RRRR#RR#R#QR\n   /@##@7%%%%77BR#RR#RRR##RRRRR##R\n     C66s%7%R%  @#RRRRRRR#RRRRRRR#(\n     KC#   ^e6  @@#RRRR##RRRRRR#RB^\n      C66     (e766C6sOGSSGSGss/\n       #s/       @6C666666666C\n       (6@  ^ ^ ( K66C6666O@e\n       ^C  ^    G 6C66OKRss6@\n        6/Q  # 6O6s6C@6C6C66R\n       ^C6GQS#@6s6666R66666O@\n       /66s66OC666666S6666CCR\n       G6sssCss6ss666G6666C6#\n       #C66666O666666O666CC66\n       @s66C66666C6sOs6CC666/\n       GsC6s666sssC6K6666C66\n      (6s666s6666666@6666666\n      #Csss6s66OC6ss#666666#\n      #6s66O6666666Os666O6O@\n     RsRROOO6Os6666@s6OOOOR@#/\n   #C6sKR#Os6sGGGs66s@Gs66B@OCs#\n        6GSR@R#@@ReSKKSSSSSSeG@Q\n       ~eSSeeeSSSRSeSeeSeSeSSeSeeS@@@@R#RR@@RG\n      RSeeeSSeSSeSR@@#SeeSeeeSSeeSeeeeeSSeeSeeS@\n    6S@@@@@@@@@@KSeSQ     (R@#SGeeGSeGeeGSSRRO\n   /SSeSeSSSeSSeeeSG#\n    ReGSeSSeSeSSeSeeK^\n       (@##SSSSSS#@#";
	std::cout << "       (@##SSSSSS#@#\n    ReGSeSSeSeSSeSeeK^\n   /SSeSeSSSeSSeeeSG#\n    6S@@@@@@@@@@KSeSQ     (R@#SGeeGSeGeeGSSRRO\n      RSeeeSSeSSeSR@@#SeeSeeeSSeeSeeeeeSSeeSeeS@\n       ~eSSeeeSSSRSeSeeSeSeSSeSeeS@@@@R#RR@@RG\n        6GSR@R#@@ReSKKSSSSSSeG@Q\n   #C6sKR#Os6sGGGs66s@Gs66B@OCs#\n     RsRROOO6Os6666@s6OOOOR@#/\n      #6s66O6666666Os666O6O@\n      #Csss6s66OC6ss#666666#\n      (6s666s6666666@6666666\n       GsC6s666sssC6K6666C66\n       @s66C66666C6sOs6CC666/\n       #C66666O666666O666CC66\n       G6sssCss6ss666G6666C6#\n       /66s66OC666666S6666CCR\n       ^C6GQS#@6s6666R66666O@\n        6/Q  # 6O6s6C@6C6C66R\n       ^C  ^    G 6C66OKRss6@\n       (6@  ^ ^ ( K66C6666O@e\n       #s/       @6C666666666C\n      C66     (e766C6sOGSSGSGss/\n     KC#   ^e6  @@#RRRR##RRRRRR#RB^\n     C66s%7%R%  @#RRRRRRR#RRRRRRR#(\n   /@##@7%%%%77BR#RR#RRR##RRRRR##R\n   G#RR@%%777%7RRRRRR#RRRR#RR#R#QR\n    RR#@%7%%%%%R#R#RRRRRR#####RR#@\n    /R@C77%%%%%@RRRRRRRRRRRRRR#R@/\n     @@%77%%%%%#RRRRRRRRR###RRRRG\n      @%%%%%%%%@RRRRR#RR#R#RR#R@\n      S7%%%%77%@RRRRRRRRRRRRRRR\n      B@RR#RRR#RR@RRRRRRRRRR#@B\n     @#RRRRRRRRRR@RRR#RRRRR#RR@\n     ~RRRRRRRRRRR@RRR##RRR#RRR@\n      RR#RRRRRRR@RRRRRRRRRRR@R#\n       RR#RRRRR@RR#RRRRR#RR#RG\n        QRR#RR@RR@@@RR@R@@@@\n          (#@#@@RR#R@B@R@RRR\n            ^@RR#RR@RRR@@RQ@\n             @####RR@@%~~~BR\n             (R#R@C~R~ROO@(^\n              R7////~RO#@@(G\n               ////S%@@@@@@~/\n          7R^  ///R~@@@/ @ ^ e\n        RBR###B///~~ ~Q~~/////~Q^\n       @####@////Q/~e//~//////////(\n      BR#R###@7#//e7t#CQ(/CQC///(Q \n     ####RR#R##R@//s/   Q%OOOO@#@    /\n    ~@###RR#R#R@//R    /   ###BKKK    /\n   ##########R@///C      G  t B#K#~    BROCK UNIVERSITY IS NOT A CLOWN COLLEGE\n   G##R#RBBR@%//////7   @    B#KKK#\n /(##R##e #////////%////////#K####K@\n  K#####  @///////////////( @K#####@\n  K###@7  ^//////////////O   e#####R~\n    ^       eBBsQ///^///(      ~B@##R\n          /QK####BBs/sQ\n          e###KK#RRe\n            6B@(7";
	for (int i = 0; i < edge_list.size(); i++){
		int one = points_store.at(edge_list.at(i)._u)._frst;
		int two = points_store.at(edge_list.at(i)._v)._frst;
		if (one != two){
			union_store.at(one).unionSets(union_store.at(two), &points_store);
			kst_edges.push_back(edge_list.at(i));
		}
	}
}

void drawUnitVectors()
{
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


void drawStuff()
{
	drawUnitVectors();
	glLineWidth(1);
	glColor3ub(51, 255, 102);
	glBegin(GL_LINES);

	for (int i = 0; i < kst_edges.size(); i++){
		glColor3ub(points_store.at(kst_edges.at(i)._u)._x, 
				   points_store.at(kst_edges.at(i)._u)._y, 
				   points_store.at(kst_edges.at(i)._u)._z);

		glVertex3i(points_store.at(kst_edges.at(i)._u)._x,
				   points_store.at(kst_edges.at(i)._u)._y,
				   points_store.at(kst_edges.at(i)._u)._z);

		glVertex3i(points_store.at(kst_edges.at(i)._v)._x,
				   points_store.at(kst_edges.at(i)._v)._y,
				   points_store.at(kst_edges.at(i)._v)._z);
	}
	glEnd();

	glutSwapBuffers();
	glFlush();
}

//naive rotation on mouse
void motion(int x, int y) 
{
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

	if (x < 300 || x > 500 || y < 300 || y > 500){
		glutWarpPointer(400, 400);
		xpos = 0;
		ypos = 0;
	}
}

void cleanUp(){

}


int main(int argc, char **argv) 
{
	makePoints(8900);
	makeEdges();
	krustyTheKruskals();
	std::cout << sizeof(&points_store);
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Glut rotate");

	glutSetCursor(GLUT_CURSOR_NONE);
	glutDisplayFunc(drawStuff);
	glutIdleFunc(drawStuff);
	glutPassiveMotionFunc(motion);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-800.0, 800.0, -800.0, 800.0, -800.0, 800.0);

	glMatrixMode(GL_MODELVIEW);

	//glRotatef(30.0, 0.0, 0.0, 0.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();
}