#ifndef OBJ_H
#define OBJ_H

#include "vect.h"
#include "edge.h"
#include "triangle.h"
#include "color.h"
#include "vect4.h"
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include <sstream>

class obj {
private:
	std::vector<vect> points; //list of points read in from file
	std::vector<vect> faces; //list of faces read in from file
	std::vector<edge> edges; //list of edges generated from above
	std::vector<triangle> triangles; //list of triangles generated from above
	std::vector<std::vector<int>> pointConns; //kind of a jerry rig; 2D vector that stores all point connections
	std::vector<vect4> pointNorms;
	color ambient;
	color diffuse;
	color specular;
	float phong;
public:
	obj(); //constructor
	void readData(std::string filename); //read in data from file and store it in points and faces
	void writeData(std::string filename); //write data from points and faces
	void storeData(); //store data from points and faces in edges, triangles and pointconns
	void subdivide(); //loop subdivision
	void subdivide_ug(float lambda);
	void setAmbient(vect am);
	void setDiffuse(vect dif);
	void setSpecular(vect spec);
	void setPhong(float phong);
	void pointPopulate(); //populates the list of point and triangle normals
	color getAmbient();
	color getDiffuse();
	color getSpecular();
	float getPhong();
	std::vector<vect>* getPoints();
	std::vector<triangle> getTriangles();
	vect4 getPointNorm(int loc);
};
#endif