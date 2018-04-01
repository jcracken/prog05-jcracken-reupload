#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "vect4.h"

class light {
  private:
    vect4 loc;
    color col;
    vect4 a;
    vect4 b;
  public:
    light();
    void setLoc(vect loc);
    void setCol(vect col);
    void setA(vect a);
	void setA(vect4 a);
    void setB(vect b);
	void setB(vect4 b);
    vect4 getLoc();
    color getCol();
    vect4 getA();
	vect4 getB();
};
#endif
