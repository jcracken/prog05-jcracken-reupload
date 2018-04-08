#include "vect.h"

vect::vect() { //init values to 0
	this->arr[0] = 0;
	this->arr[1] = 0;
	this->arr[2] = 0;
}

vect::vect(float a, float b, float c) { //init values to input
	this->arr[0] = a;
	this->arr[1] = b;
	this->arr[2] = c;
}

void vect::addConst(float num) { //add constant to vect
	this->arr[0] = this->arr[0] + num;
	this->arr[1] = this->arr[1] + num;
	this->arr[2] = this->arr[2] + num;
}

void vect::multConst(float num) { //mult by constat
	this->arr[0] = this->arr[0] * num;
	this->arr[1] = this->arr[1] * num;
	this->arr[2] = this->arr[2] * num;
}

float * vect::getArr() { //return saved values
	return this->arr;
}

float vect::dotProduct(vect * other){ //dot product with another vector
	return this->arr[0] * (other->getArr()[0]) + this->arr[1] * (other->getArr()[1]) + this->arr[2] * (other->getArr()[2]);
}

void vect::add(vect * other){ //add two vectors
	this->arr[0] = this->arr[0] + other->getArr()[0];
	this->arr[1] = this->arr[1] + other->getArr()[1];
	this->arr[2] = this->arr[2] + other->getArr()[2];
}

vect vect::crossProduct(vect * other) { //cross product with another vector
	return vect(this->arr[1] * other->getArr()[2] - other->getArr()[1] * this->arr[2], other->getArr()[0] * this->arr[2] - this->arr[0] * other->getArr()[2], this->arr[0] * other->getArr()[1] - other->getArr()[0] * this->arr[1]);
}

bool vect::comp(vect other){ //if other matches this vector, return true; otherwise, return false
	float* temp = other.getArr();
	if(this->arr[0] == temp[0] && this->arr[1] == temp[1] && this->arr[2] == temp[2]) return true;
	else return false;
}

void vect::set(float a, float b, float c) {
	this->arr[0] = a;
	this->arr[1] = b;
	this->arr[2] = c;
}
