#include "pixel.h"

pixel::pixel(){ //constructor
  this->pixColor = color();
}

vect pixel::getColor(){ //return color values as float array
  return this->pixColor.getColor();
}

void pixel::setColor(vect color){ //set color values as float array
  this->pixColor.setColor(color);
}
