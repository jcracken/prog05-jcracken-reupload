#ifndef PIXEL_H
#define PIXEL_H

#include "color.h"

class pixel {
  private:
    color pixColor;
  public:
    pixel();
    vect getColor();
    void setColor(vect color);
};
#endif
