#include "light.h"

light::light(){ 
	//constructor
}

void light::setLoc(vect loc){ //location of light
	this->loc = vect(loc.getArr()[0], loc.getArr()[1], loc.getArr()[2]);
}

void light::setCol(vect col){ //color of light
  this->col.setColor(col);
}

vect light::getLoc(){
  return this->loc;
}

color light::getCol(){
  return this->col;
}

void light::setA(vect a){ //A vector for area lights
	this->a = vect(a.getArr()[0], a.getArr()[1], a.getArr()[2]);
}

void light::setB(vect b){ //B vector for area lights
	this->b = vect(b.getArr()[0], b.getArr()[1], b.getArr()[2]);
}

vect light::getA(){
  return this->a;
}

vect light::getB(){
  return this->b;
}
