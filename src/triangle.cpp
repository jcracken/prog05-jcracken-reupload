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

bool triangle::intersect(vect v, float* z) { //todo
	return false;
}

float triangle::boundXMin() {
	float min = INT_MAX;
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[0] < min) min = points.at(i)->getArr()[0];
	}
	return min;
}

float triangle::boundYMin() {
	float min = INT_MAX;
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[1] < min) min = points.at(i)->getArr()[1];
	}
	return min;
}

float triangle::boundXMax() {
	float min = INT_MIN;
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[0] > min) min = points.at(i)->getArr()[0];
	}
	return min;
}

float triangle::boundYMax() {
	float min = INT_MIN;
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i)->getArr()[1] > min) min = points.at(i)->getArr()[1];
	}
	return min;
}

vect triangle::getNorm() {
	return norm;
}
