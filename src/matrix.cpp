#include "matrix.h"

matrix::matrix(){
	this->mat.resize(4, std::vector<float>(4, 0));
}

matrix::matrix(unsigned int a, unsigned int b){
	this->mat.resize(a, std::vector<float>(b, 0));
}

void matrix::setVal(int x, int y, float val){
	this->mat[x][y] = val;
}

float matrix::getVal(int x, int y){
	return mat[x][y];
}

void matrix::mult(matrix * other, matrix * ret){
	int i, j, k = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			float temp = 0.0;
			for (k = 0; k < 4; k++) {
				temp = temp + this->mat[i][k] * other->getVal(k, j);
			}
			ret->setVal(i, j, temp);
		}
	}
}

void matrix::mult(vect4 * other, matrix * ret){
	int i, j;
	float temp;
	for (i = 0; i < 4; i++) {
		temp = 0;
		for (j = 0; j < 4; j++) {
			temp = temp + this->mat[i][j] * other->getArr()[i];
		}
		ret->setVal(i, 0, temp);
	}
}
