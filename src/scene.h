#ifndef SCENE_H
#define SCENE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

#include "vect.h"
#include "light.h"
#include "obj.h"

class scene { //main class, used to render everything
  private:
    int width;
    int height;
    vect eye;
    vect lookat;
    vect up;
    float angle;
	float nearDepth;
	float farDepth;
	std::vector<light> lights;
	std::vector<obj> objects;
  public:
    scene();
    int returnWidth();
    int returnHeight();
	vect getEye();
	vect getLookAt();
	vect getUp();
    float** returnData();
    float returnAngle();
    void acquireData(std::string name);
};
#endif
