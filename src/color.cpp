#include "color.h"

color::color(){ 
	//constructor
}

vect color::getColor(){ //return color values as float array
  return this->col;
}

void color::setColor(vect col){ //set color using float array
	this->col = vect(col.getArr()[0], col.getArr()[1], col.getArr()[2]);
}
