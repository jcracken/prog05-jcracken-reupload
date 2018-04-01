#ifndef VECT4_H
#define VECT4_H

#include "vect.h"

class vect4 {
private:
	float arr[4];
public:
	vect4(); //constructor
	vect4(float a, float b, float c, float d); //constructor that inits
	vect4(vect v, float w);
	void addConst(float num); //add a constant to the vector
	void multConst(float num); //multiply by a constant
	float* getArr(); //return the saved values
	float dotProduct(vect4* other); //perform a dot product with another vector
	void add(vect4* other); //add two vectors
	vect crossProduct(vect4* other); //perform a cross product with another vector
	bool comp(vect4 other); //compare to another vector; returns true if the values match, false otherwise
};
#endif