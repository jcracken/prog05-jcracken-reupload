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
#include "vect4.h"
#include "light.h"
#include "obj.h"
#include "matrix.h"

class scene { //main class, used to render everything
  private:
    int width;
    int height;
    vect4 eye;
    vect4 lookat;
    vect4 up;
	vect4 u;
	vect4 v;
	vect4 w;
	matrix M_vp;
	matrix M_per;
	matrix M_cam;
	matrix M;
	float r;
	float l;
	float t;
	float b;
    float angle;
	float nearDepth;
	vect** data;
	vect** pixelLoc;
	float farDepth;
	std::vector<light> lights;
	std::vector<obj> objects;
	vect shading(vect n, obj o);
	void genPixelData(float imageHeight, float imageWidth);
  public:
    scene();
    int returnWidth();
    int returnHeight();
	vect4 getEye();
	vect4 getLookAt();
	vect4 getUp();
    float** returnData(int type);
    float returnAngle();
    void acquireData(std::string name);
	void setup();
	void draw();
};
#endif
