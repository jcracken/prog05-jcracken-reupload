#ifndef TRIANGLE_H
#define TRIANGLE_H

class edge; //forward declaration, think of this as an edge.h include
#include "edge.h"
#include "vect.h"
#include "vect4.h"
#include <vector>

class triangle {
private:
	std::vector<vect*> points; //the three points that make up this triangle
	std::vector<edge*> edges; //the three edges that make up this triangle
	vect norm;
	float edgeFunction(vect a, vect b, vect c);
public:
	triangle(); //constructor
	vect thirdPoint(vect head, vect tail); //finds the third point that aren't the two passed in
	void populatePoint(vect* point); //add a point to the vector
	void populateEdge(edge* e); //add an edge to the vector
	vect getPoint(int num);
	edge* getEdge(int num);
	void populateNorm(vect n);
	bool intersect(vect v, float* z, float* w0, float* w1, float* w2); //returns if a ray intersects or not, as well as the interpolated z value and the barycentric coordinates
	float boundXMin(); //these four give the bounding box
	float boundYMin();
	float boundXMax();
	float boundYMax();
	vect getNorm();
};
#endif