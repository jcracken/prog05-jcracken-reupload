#include "edge.h"

edge::edge(){
	//constructor
	this->head = NULL;
	this->tail = NULL;
}

bool edge::isSame(vect head, vect tail){ //is another edge same as this one?
	if ((this->head->comp(head) && this->tail->comp(tail)) || (this->head->comp(tail) && this->tail->comp(head))) return true;
	else return false;
}

void edge::loopHelp(float* ret){ //used for loop subdivision
	if (triang.size() == 2) { //if not a boundary edge
		float* temp1 = triang.at(0)->thirdPoint(*head, *tail).getArr(); //get the two points c and d
		float* temp2 = triang.at(1)->thirdPoint(*head, *tail).getArr();
		//run the calculations for odd points
		ret[0] = (3.0 / 8.0) * (this->head->getArr()[0] + this->tail->getArr()[0]) + (1.0 / 8.0) * (temp1[0] + temp2[0]);
		ret[1] = (3.0 / 8.0) * (this->head->getArr()[1] + this->tail->getArr()[1]) + (1.0 / 8.0) * (temp1[1] + temp2[1]);
		ret[2] = (3.0 / 8.0) * (this->head->getArr()[2] + this->tail->getArr()[2]) + (1.0 / 8.0) * (temp1[2] + temp2[2]);
	}
	else {
		//if it is a boundary edge, we don't need to worry about c or d
		ret[0] = (1.0 / 2.0) * (this->head->getArr()[0] + this->tail->getArr()[0]);
		ret[1] = (1.0 / 2.0) * (this->head->getArr()[1] + this->tail->getArr()[1]);
		ret[2] = (1.0 / 2.0) * (this->head->getArr()[2] + this->tail->getArr()[2]);
	}
}

void edge::populatePoint(vect* point){ //store head or tail point
	if (this->head == NULL) this->head = point;
	else this->tail = point;
}

void edge::populateTriangle(triangle* t) { //add triangle to the list
	this->triang.push_back(t);
}

void edge::setOdd(int odd){ //set location of generated odd point within vector in obj
	this->odd = odd;
}

int edge::getOdd() {
	return this->odd;
}

vect * edge::getHead() {
	return this->head;
}

vect * edge::getTail() {
	return this->tail;
}

bool edge::isPart(vect point) { //is the point passed in a head or a tail of this edge?
	return (this->head->comp(point) || this->tail->comp(point));
}

vect edge::getVecRep() {
	return vect(this->head->getArr()[0] - this->tail->getArr()[0], this->head->getArr()[1] - this->tail->getArr()[1], this->head->getArr()[2] - this->tail->getArr()[2]);
}