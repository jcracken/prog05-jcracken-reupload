#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "vect.h"

class light {
  private:
    vect loc;
    color col;
    vect a;
    vect b;
  public:
    light();
    void setLoc(vect loc);
    void setCol(vect col);
    void setA(vect a);
    void setB(vect b);
    vect getLoc();
    color getCol();
    vect getA();
	vect getB();
};
#endif
