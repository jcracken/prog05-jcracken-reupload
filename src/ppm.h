#ifndef PPM_H
#define PPM_H

#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>

class ppm {
  private:
    unsigned char* data;
    int width;
    int height;
    int maxVal;
  public:
    ppm();
    void readData(std::string name);
    void writeData(std::string name);
    unsigned char* returnData();
    int returnWidth();
    int returnHeight();
    int returnMaxVal();
    void setData(unsigned char* data);
    void setData(float** data, int height, int width);
    void setWidth(int width);
    void setHeight(int height);
};
#endif
