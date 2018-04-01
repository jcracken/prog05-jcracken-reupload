#include "light.h"

light::light(){ 
	//constructor
}

void light::setLoc(vect loc){ //location of light
	this->loc = vect4(loc, 1);
}

void light::setCol(vect col){ //color of light
  this->col.setColor(col);
}

vect4 light::getLoc(){
  return this->loc;
}

color light::getCol(){
  return this->col;
}

void light::setA(vect a){ //A vector for area lights
	this->a = vect4(a, 0);
}

void light::setA(vect4 a) {
	float* temp = a.getArr();
	this->a = vect4(temp[0], temp[1], temp[2], temp[3]);
}

void light::setB(vect b){ //B vector for area lights
	this->b = vect4(b, 0);
}

void light::setB(vect4 b) {
	float* temp = b.getArr();
	this->b = vect4(temp[0], temp[1], temp[2], temp[3]);
}

vect4 light::getA(){
  return this->a;
}

vect4 light::getB(){
  return this->b;
}
