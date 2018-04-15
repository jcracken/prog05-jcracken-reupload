#include "triangle.h"

triangle::triangle(){
	//constructor
}

vect triangle::thirdPoint(vect head, vect tail){ //find the point that doesn't match the two passed in points
	unsigned int i = 0;
	for (i = 0; i < this->points.size(); i++) {
		if (!(head.comp(*(this->points.at(i)))) && !(tail.comp(*(this->points.at(i))))) return *(this->points.at(i));
	}
	return vect(0, 0, 0); //should never happen
}

void triangle::populatePoint(vect* point){ //add a point to the vector
	this->points.push_back(point);
}

void triangle::populateEdge(edge* e) { //add an edge to the vector
	this->edges.push_back(e);
}

vect triangle::getPoint(int num) { //easier than passing back a vector
	return *this->points.at(num);
}

edge * triangle::getEdge(int num) { //easier than passing back a vector
	return this->edges.at(num);
}

void triangle::populateNorm(vect n) {
	this->norm = n;
}

bool triangle::intersect(vect v, float* z, float* w0, float* w1, float* w2) { //barycentric calculations
	bool ret = true;

	float area = edgeFunction(*this->points.at(0), *this->points.at(1), *this->points.at(2)); //find area
	*w0 = edgeFunction(*this->points.at(1), *this->points.at(2), v); //find area of each of the smaller triangles in barycentric rep
	*w1 = edgeFunction(*this->points.at(2), *this->points.at(0), v);
	*w2 = edgeFunction(*this->points.at(0), *this->points.at(1), v);
	*w0 /= area; //normalize
	*w1 /= area;
	*w2 /= area;
	ret &= ((*w0) >= 0.0); //check to see all three barycentric values are greater than0
	ret &= ((*w1) >= 0.0);
	ret &= ((*w2) >= 0.0);

	//interpolate z-value
	*z = *w0 * (this->points.at(0)->getArr()[2]) + *w1 * (this->points.at(1)->getArr()[2]) + *w2 * (this->points.at(2)->getArr()[2]);

	return ret;
}

//the next four return bounding box values
float triangle::boundXMin() {
	float min = points.at(0)->getArr()[0];
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[0] < min) min = points.at(i)->getArr()[0];
	}
	return min;
}

float triangle::boundYMin() {
	float min = points.at(0)->getArr()[1];
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[1] < min) min = points.at(i)->getArr()[1];
	}
	return min;
}

float triangle::boundXMax() {
	float max = points.at(0)->getArr()[0];
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[0] > max) max = points.at(i)->getArr()[0];
	}
	return max;
}

float triangle::boundYMax() {
	float max = points.at(0)->getArr()[1];
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[1] > max) max = points.at(i)->getArr()[1];
	}
	return max;
}

vect triangle::getNorm() {
	return norm;
}

float triangle::edgeFunction(vect a, vect b, vect c) { //used for barycentric coordinates
	return (c.getArr()[0] - a.getArr()[0]) * (b.getArr()[1] - a.getArr()[1]) - (c.getArr()[1] - a.getArr()[1]) * (b.getArr()[0] - a.getArr()[0]);
}