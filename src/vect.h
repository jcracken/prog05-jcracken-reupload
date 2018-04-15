#ifndef VECT_H
#define VECT_H

#include<cmath>

class vect {
private:
	float arr[3];
public:
	vect(); //constructor
	vect(float a, float b, float c); //constructor that inits
	void addConst(float num); //add a constant to the vector
	void multConst(float num); //multiply by a constant
	float* getArr(); //return the saved values
	float dotProduct(vect* other); //perform a dot product with another vector
	void add(vect* other); //add two vectors
	vect crossProduct(vect* other); //perform a cross product with another vector
	bool comp(vect other); //compare to another vector; returns true if the values match, false otherwise
	void set(float a, float b, float c);
	void normalize(); //normalizes vector
};
#endif