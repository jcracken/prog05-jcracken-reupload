#ifndef TRIANGLE_H
#define TRIANGLE_H

class edge; //forward declaration, think of this as an edge.h include
#include "vect.h"
#include "vect4.h"
#include <vector>

class triangle {
private:
	std::vector<vect*> points; //the three points that make up this triangle
	std::vector<edge*> edges; //the three edges that make up this triangle
	vect4 norm;
public:
	triangle(); //constructor
	vect thirdPoint(vect head, vect tail); //finds the third point that aren't the two passed in
	void populatePoint(vect* point); //add a point to the vector
	void populateEdge(edge* e); //add an edge to the vector
	vect getPoint(int num);
	edge* getEdge(int num);
	void populateNorm(vect n);
	bool intersect(vect v);
};
#endif