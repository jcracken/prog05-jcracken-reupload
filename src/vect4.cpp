#include "vect4.h"

vect4::vect4() { //init values to 0
	this->arr[0] = 0;
	this->arr[1] = 0;
	this->arr[2] = 0;
	this->arr[3] = 0;
}

vect4::vect4(float a, float b, float c, float d) { //init values to input
	this->arr[0] = a;
	this->arr[1] = b;
	this->arr[2] = c;
	this->arr[3] = d;
}

vect4::vect4(vect v, float w) {
	float* temp = v.getArr();
	this->arr[0] = temp[0];
	this->arr[1] = temp[1];
	this->arr[2] = temp[2];
	this->arr[3] = w;
}

void vect4::addConst(float num) { //add constant to vect4
	this->arr[0] = this->arr[0] + num;
	this->arr[1] = this->arr[1] + num;
	this->arr[2] = this->arr[2] + num;
	this->arr[3] = this->arr[3] + num;
}

void vect4::multConst(float num) { //mult by constat
	this->arr[0] = this->arr[0] * num;
	this->arr[1] = this->arr[1] * num;
	this->arr[2] = this->arr[2] * num;
	this->arr[3] = this->arr[3] * num;
}

float * vect4::getArr() { //return saved values
	return this->arr;
}

float vect4::dotProduct(vect4 * other) { //dot product with another vect4or
	return this->arr[0] * (other->getArr()[0]) + this->arr[1] * (other->getArr()[1]) + this->arr[2] * (other->getArr()[2]) + this->arr[3] * (other->getArr()[3]);
}

void vect4::add(vect4 * other) { //add two vect4ors
	this->arr[0] = this->arr[0] + other->getArr()[0];
	this->arr[1] = this->arr[1] + other->getArr()[1];
	this->arr[2] = this->arr[2] + other->getArr()[2];
	this->arr[3] = this->arr[3] + other->getArr()[2];
}

vect vect4::crossProduct(vect4 * other) { //cross product with another vect4or
	return vect(this->arr[1] * other->getArr()[2] - other->getArr()[1] * this->arr[2], other->getArr()[0] * this->arr[2] - this->arr[0] * other->getArr()[2], this->arr[0] * other->getArr()[1] - other->getArr()[0] * this->arr[1]);
}

bool vect4::comp(vect4 other) { //if other matches this vect4or, return true; otherwise, return false
	float* temp = other.getArr();
	if (this->arr[0] == temp[0] && this->arr[1] == temp[1] && this->arr[2] == temp[2] && this->arr[3] == temp[3]) return true;
	else return false;
}
