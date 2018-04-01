#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "vect4.h"

class matrix {
private:
	std::vector<std::vector<float>> mat;
public:
	matrix();
	matrix(unsigned int a, unsigned int b);
	void setVal(int x, int y, float val);
	float getVal(int x, int y);
	void mult(matrix* other, matrix* ret);
	void mult(vect4* other, matrix* ret);
};
#endif