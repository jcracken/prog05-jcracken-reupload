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
	this->norm = vect4(n, 0.0);
}

bool triangle::intersect(vect v) { //todo
	return false;
}
