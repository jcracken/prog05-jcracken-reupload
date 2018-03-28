#ifndef COLOR_H
#define COLOR_H

#include "vect.h"

class color {
  private:
    vect col;
  public:
    color();
    color( const color &obj );
    vect getColor();
    void setColor(vect col);
};
#endif
