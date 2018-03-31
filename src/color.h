#ifndef COLOR_H
#define COLOR_H

#include "vect.h"

class color {
  private:
    vect col;
  public:
    color();
    vect getColor();
    void setColor(vect col);
};
#endif
