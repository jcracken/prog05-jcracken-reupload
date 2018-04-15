#include "obj.h"

obj::obj() {
	//constructor
	triangles.reserve(50000); //done to prevent pointers breaking; see readme
	points.reserve(50000);
	faces.reserve(50000);
	edges.reserve(50000);
}

void obj::readData(std::string filename) { //reads in data from filename
	std::ifstream input(filename, std::ifstream::in);
	std::string parse; //used for parsing a line at a time

	float a = 0, b = 0, c = 0; //used to store values being parsed
	char type = '\0'; //to see if a line is about a face, a point, or can be ignored

	if (!(input.is_open())) {
		std::cout << "invalid filename" << std::endl;
		exit(EXIT_FAILURE);
	}

	while (!input.eof()) {
		if (getline(input, parse)) { //if line is empty, it should skip

			std::istringstream iss(parse);
			iss >> type; //get line type

			switch (type) {
				case '#':
					//skip comments
				break;
				case 'v': //it's a point, store the values in the point vector
					iss >> a >> b >> c;
					this->points.push_back(vect(a, b, c));
				break;
				case 'f': //it's a face, store it in the face vector
					iss >> a >> b >> c;
					this->faces.push_back(vect(a - 1, b - 1, c - 1));
				break;
				default:
					//skip white space
				break;
			}
		}
	}
	input.close(); //close input file
}

void obj::writeData(std::string filename) { //writes data to an output file
	std::ofstream out(filename, std::ofstream::out);

	unsigned int i; //used to loop

	for (i = 0; i < this->points.size(); i++) { //output all points
		out << "v " << this->points.at(i).getArr()[0] << " " << this->points.at(i).getArr()[1] << " " << this->points.at(i).getArr()[2] << std::endl;
	}
	out << std::endl; //empty line to break up points and faces

	for (i = 0; i < this->faces.size(); i++) { //output all faces
		out << "f " << (int)(this->faces.at(i).getArr()[0] + 1) << " " << (int)(this->faces.at(i).getArr()[1] + 1) << " " << (int)(this->faces.at(i).getArr()[2] + 1) << std::endl;
	}
	out.close(); //close output file
}

void obj::storeData(){ //stores acquired data in new data structure
	unsigned int i = 0; //used for looping
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int m = 0;
	unsigned int firstedgeExist = 0, secedgeExist = 0, thirdedgeExist = 0; //used for edge generation

	bool firstexist = false; //used for edge generation
	bool secexist = false;
	bool thirdexist = false;
	bool pointOne = false;
	bool pointTwo = false;

	this->edges.clear(); //clear the vectors for when we run multiple iterations
	this->triangles.clear();
	this->pointConns.clear();

	for (i = 0; i < this->points.size(); i++) { //fill dummy data in the 2D vector so that we can establish height without having to specify width
		std::vector<int> dummy(1, -1);
		pointConns.push_back(dummy);
	}

	for (i = 0; i < this->faces.size(); i++) {
		
		/*this chunk of code basically goes through the points connected to the current face and checks to see if the
		  connections estabilished by this face already exist in pointConns; if not, then it stores it in this format:
		  EX: face with points 1, 2, 3. pointConns will then put values 2 and 3 in the first location, 1 and 3 in
			  the second location, and 1 and 2 in the third location. */
		for (m = 0; m < this->pointConns.at(this->faces.at(i).getArr()[0]).size(); m++) {
			if (this->pointConns.at(this->faces.at(i).getArr()[0]).at(m) == this->faces.at(i).getArr()[1]) pointOne = true;
			if (this->pointConns.at(this->faces.at(i).getArr()[0]).at(m) == this->faces.at(i).getArr()[2]) pointTwo = true;
		}
		if (!pointOne) this->pointConns.at(this->faces.at(i).getArr()[0]).push_back(this->faces.at(i).getArr()[1]);
		if (!pointTwo) this->pointConns.at(this->faces.at(i).getArr()[0]).push_back(this->faces.at(i).getArr()[2]);
		pointOne = false;
		pointTwo = false;

		for (m = 0; m < this->pointConns.at(this->faces.at(i).getArr()[1]).size(); m++) {
			if (this->pointConns.at(this->faces.at(i).getArr()[1]).at(m) == this->faces.at(i).getArr()[0]) pointOne = true;
			if (this->pointConns.at(this->faces.at(i).getArr()[1]).at(m) == this->faces.at(i).getArr()[2]) pointTwo = true;
		}
		if (!pointOne) this->pointConns.at(this->faces.at(i).getArr()[1]).push_back(this->faces.at(i).getArr()[0]);
		if (!pointTwo) this->pointConns.at(this->faces.at(i).getArr()[1]).push_back(this->faces.at(i).getArr()[2]);
		pointOne = false;
		pointTwo = false;

		for (m = 0; m < this->pointConns.at(this->faces.at(i).getArr()[2]).size(); m++) {
			if (this->pointConns.at(this->faces.at(i).getArr()[2]).at(m) == this->faces.at(i).getArr()[1]) pointOne = true;
			if (this->pointConns.at(this->faces.at(i).getArr()[2]).at(m) == this->faces.at(i).getArr()[0]) pointTwo = true;
		}
		if (!pointOne) this->pointConns.at(this->faces.at(i).getArr()[2]).push_back(this->faces.at(i).getArr()[1]);
		if (!pointTwo) this->pointConns.at(this->faces.at(i).getArr()[2]).push_back(this->faces.at(i).getArr()[0]);
		pointOne = false;
		pointTwo = false;

		//establish each face in the triangles vector and add the related points to that object
		this->triangles.push_back(triangle());
		this->triangles.at(k).populatePoint(&this->points.at(this->faces.at(i).getArr()[0]));
		this->triangles.at(k).populatePoint(&this->points.at(this->faces.at(i).getArr()[1]));
		this->triangles.at(k).populatePoint(&this->points.at(this->faces.at(i).getArr()[2]));

		//go through the list of vectors to see if the related edges already exist; if so, find their location in the edges vector
		for (j = 0; j < this->edges.size(); j++) {
			//first and second point
			if (this->edges.at(j).isSame(this->points.at(this->faces.at(i).getArr()[0]), this->points.at(this->faces.at(i).getArr()[1]))) {
				firstexist = true;
				firstedgeExist = j;
			}
			//second and third point
			if (this->edges.at(j).isSame(this->points.at(this->faces.at(i).getArr()[1]), this->points.at(this->faces.at(i).getArr()[2]))) {
				secexist = true;
				secedgeExist = j;
			}
			//first and third point
			if (this->edges.at(j).isSame(this->points.at(this->faces.at(i).getArr()[0]), this->points.at(this->faces.at(i).getArr()[2]))) {
				thirdexist = true;
				thirdedgeExist = j;
			}
		}

		//if the edges don't already exist, create a new edge and add it to the vector; otherwise, add the triangle to the existing edge
		if (!firstexist) {
			edge e = edge();
			e.populatePoint(&(this->points.at(this->faces.at(i).getArr()[0])));
			e.populatePoint(&(this->points.at(this->faces.at(i).getArr()[1])));
			e.populateTriangle(&(this->triangles.at(k)));
			this->edges.push_back(e);
			this->triangles.at(k).populateEdge(&(this->edges.at(this->edges.size() - 1)));
		}
		else {
			this->edges.at(firstedgeExist).populateTriangle(&(this->triangles.at(k)));
			this->triangles.at(k).populateEdge(&(this->edges.at(firstedgeExist)));
		}
		if (!secexist) {
			edge e = edge();
			e.populatePoint(&(this->points.at(this->faces.at(i).getArr()[1])));
			e.populatePoint(&(this->points.at(this->faces.at(i).getArr()[2])));
			e.populateTriangle(&(this->triangles.at(k)));
			this->edges.push_back(e);
			this->triangles.at(k).populateEdge(&(this->edges.at(this->edges.size() - 1)));
		}
		else {
			this->edges.at(secedgeExist).populateTriangle(&(this->triangles.at(k)));
			this->triangles.at(k).populateEdge(&(this->edges.at(secedgeExist)));
		}
		if (!thirdexist) {
			edge e = edge();
			e.populatePoint(&(this->points.at(this->faces.at(i).getArr()[0])));
			e.populatePoint(&(this->points.at(this->faces.at(i).getArr()[2])));
			e.populateTriangle(&(this->triangles.at(k)));
			this->edges.push_back(e);
			this->triangles.at(k).populateEdge(&(this->edges.at(this->edges.size() - 1)));
		}
		else {
			this->edges.at(thirdedgeExist).populateTriangle(&(this->triangles.at(k)));
			this->triangles.at(k).populateEdge(&(this->edges.at(thirdedgeExist)));
		}
		firstexist = false;
		secexist = false;
		thirdexist = false; 
		k++;
	}

	//delete the dummy data from the connections 2D vector
	for (i = 0; i < pointConns.size(); i++) {
		pointConns.at(i).erase(pointConns.at(i).begin());
	}
}

void obj::subdivide() { //loop subdivision
	std::vector<vect> newPoints; //will store new values in these vectors
	std::vector<vect> newFaces;

	unsigned int i, j; //used for iteration
	float weight = 0.0; //used for even vertices

	for (i = 0; i < this->points.size(); i++) { //even vertices
		float temp[3] = { 0 };

		//change the weight depending on how many points are connected; dictated by document in readme
		if (this->pointConns.at(i).size() == 2) { //boundary condition
			temp[0] = (1.0 / 8.0) * (this->points.at(this->pointConns.at(i).at(0)).getArr()[0] + this->points.at(this->pointConns.at(i).at(1)).getArr()[0]) + (3.0 / 4.0) * this->points.at(i).getArr()[0];
			temp[1] = (1.0 / 8.0) * (this->points.at(this->pointConns.at(i).at(0)).getArr()[1] + this->points.at(this->pointConns.at(i).at(1)).getArr()[1]) + (3.0 / 4.0) * this->points.at(i).getArr()[1];
			temp[2] = (1.0 / 8.0) * (this->points.at(this->pointConns.at(i).at(0)).getArr()[2] + this->points.at(this->pointConns.at(i).at(1)).getArr()[2]) + (3.0 / 4.0) * this->points.at(i).getArr()[2];
		} else if (this->pointConns.at(i).size() == 3) { //if three connections
			weight = (3.0 / 16.0);
			temp[0] = this->points.at(i).getArr()[0] * (1.0 - 3.0 * weight);
			temp[1] = this->points.at(i).getArr()[1] * (1.0 - 3.0 * weight);
			temp[2] = this->points.at(i).getArr()[2] * (1.0 - 3.0 * weight);
			for (j = 0; j < this->pointConns.at(i).size(); j++) {
				temp[0] = temp[0] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[0]) * weight;
				temp[1] = temp[1] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[1]) * weight;
				temp[2] = temp[2] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[2]) * weight;
			}
		} else { //if more than three connections
			weight = (3.0 / (8.0 * this->pointConns.at(i).size()));
			temp[0] = this->points.at(i).getArr()[0] * (1.0 - this->pointConns.at(i).size() * weight);
			temp[1] = this->points.at(i).getArr()[1] * (1.0 - this->pointConns.at(i).size() * weight);
			temp[2] = this->points.at(i).getArr()[2] * (1.0 - this->pointConns.at(i).size() * weight);
			for (j = 0; j < this->pointConns.at(i).size(); j++) {
				temp[0] = temp[0] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[0]) * weight;
				temp[1] = temp[1] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[1]) * weight;
				temp[2] = temp[2] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[2]) * weight;
			}
		}
		newPoints.push_back(vect(temp[0], temp[1], temp[2])); //store the new point in the vector
	}

	for (i = 0; i < this->edges.size(); i++) { //odd vertices
		float temp[3];
		this->edges.at(i).loopHelp(temp); //carries all the heavy lifting here
		newPoints.push_back(vect(temp[0], temp[1], temp[2]));
		this->edges.at(i).setOdd(newPoints.size() - 1);
	}

	//populating the newFaces vector--probably the hardest part of the assignment, required me to overhaul my data structure
	for (i = 0; i < this->triangles.size(); i++) { //for every triangle, create four new triangles
		int pointA, pointB, pointC; //these are the even points

		for (j = 0; j < this->points.size(); j++) { //find where the even points are on the points vector
			if (this->points.at(j).comp((this->triangles.at(i).getPoint(0)))) {
				pointA = j;
			}
			else if (this->points.at(j).comp((this->triangles.at(i).getPoint(1)))) {
				pointB = j;
			}
			else if (this->points.at(j).comp((this->triangles.at(i).getPoint(2)))) {
				pointC = j;
			}
		}

		//the next chunk of code finds the edges associated with each even point, and then creates triangles with the associated odd points

		//pointA
		if (this->triangles.at(i).getEdge(0)->isPart(this->points.at(pointA)) && this->triangles.at(i).getEdge(1)->isPart(this->points.at(pointA))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(1)->getOdd(), pointA));
		} else if (this->triangles.at(i).getEdge(0)->isPart(this->points.at(pointA)) && this->triangles.at(i).getEdge(2)->isPart(this->points.at(pointA))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd(), pointA));
		} else if (this->triangles.at(i).getEdge(1)->isPart(this->points.at(pointA)) && this->triangles.at(i).getEdge(2)->isPart(this->points.at(pointA))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(1)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd(), pointA));
		}

		//pointB
		if (this->triangles.at(i).getEdge(0)->isPart(this->points.at(pointB)) && this->triangles.at(i).getEdge(1)->isPart(this->points.at(pointB))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(1)->getOdd(), pointB));
		}
		else if (this->triangles.at(i).getEdge(0)->isPart(this->points.at(pointB)) && this->triangles.at(i).getEdge(2)->isPart(this->points.at(pointB))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd(), pointB));
		}
		else if (this->triangles.at(i).getEdge(1)->isPart(this->points.at(pointB)) && this->triangles.at(i).getEdge(2)->isPart(this->points.at(pointB))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(1)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd(), pointB));
		}

		//pointC
		if (this->triangles.at(i).getEdge(0)->isPart(this->points.at(pointC)) && this->triangles.at(i).getEdge(1)->isPart(this->points.at(pointC))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(1)->getOdd(), pointC));
		}
		else if (this->triangles.at(i).getEdge(0)->isPart(this->points.at(pointC)) && this->triangles.at(i).getEdge(2)->isPart(this->points.at(pointC))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd(), pointC));
		}
		else if (this->triangles.at(i).getEdge(1)->isPart(this->points.at(pointC)) && this->triangles.at(i).getEdge(2)->isPart(this->points.at(pointC))) {
			newFaces.push_back(vect(this->triangles.at(i).getEdge(1)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd(), pointC));
		}

		//create one last triangle using all three odd points
		newFaces.push_back(vect(this->triangles.at(i).getEdge(0)->getOdd(), this->triangles.at(i).getEdge(1)->getOdd(), this->triangles.at(i).getEdge(2)->getOdd()));
	}

	//replace the old values with the new, and generate new data structures
	this->points.swap(newPoints);
	this->faces.swap(newFaces);
	this->storeData();
}

void obj::subdivide_ug(float lambda) {
	unsigned int i = 0;
	unsigned int j = 0;

	std::vector<vect> newPoints;

	for (i = 0; i < this->points.size(); i++) { //even vertices
		float temp[3] = { 0 };
		for (j = 0; j < this->pointConns.at(i).size(); j++) {
			temp[0] = temp[0] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[0] - this->points.at(i).getArr()[0]) / this->pointConns.at(j).size();
			temp[1] = temp[1] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[1] - this->points.at(i).getArr()[1]) / this->pointConns.at(j).size();
			temp[2] = temp[2] + (this->points.at(this->pointConns.at(i).at(j)).getArr()[2] - this->points.at(i).getArr()[2]) / this->pointConns.at(j).size();
		}
		temp[0] = temp[0] * lambda + this->points.at(i).getArr()[0];
		temp[1] = temp[1] * lambda + this->points.at(i).getArr()[1];
		temp[2] = temp[2] * lambda + this->points.at(i).getArr()[2];

		newPoints.push_back(vect(temp[0], temp[1], temp[2]));
	}
	this->points.swap(newPoints);
	this->storeData();
}

void obj::setAmbient(vect am) {
	this->ambient.setColor(am);
}

void obj::setDiffuse(vect dif) {
	this->diffuse.setColor(dif);
}

void obj::setSpecular(vect spec) {
	this->specular.setColor(spec);
}

void obj::setPhong(float phong) {
	this->phong = phong;
}

void obj::pointPopulate() {
	unsigned int i, j, k = 0;
	vect temp = vect(0, 0, 0);
	for (i = 0; i < points.size(); i++) {
		for (j = 0; j < triangles.size(); j++) { //for every triangle, if current point is part of triangle, find triangle norm
			if (triangles.at(j).getPoint(0).comp(points.at(i)) || triangles.at(j).getPoint(1).comp(points.at(i)) || triangles.at(j).getPoint(2).comp(points.at(i))) {
				vect t = vect(points.at(faces.at(j).getArr()[1]).getArr()[0] - points.at(faces.at(j).getArr()[0]).getArr()[0], points.at(faces.at(j).getArr()[1]).getArr()[1] - points.at(faces.at(j).getArr()[0]).getArr()[1], points.at(faces.at(j).getArr()[1]).getArr()[2] - points.at(faces.at(j).getArr()[0]).getArr()[2]);
				vect t1 = vect(points.at(faces.at(j).getArr()[2]).getArr()[0] - points.at(faces.at(j).getArr()[1]).getArr()[0], points.at(faces.at(j).getArr()[2]).getArr()[1] - points.at(faces.at(j).getArr()[1]).getArr()[1], points.at(faces.at(j).getArr()[2]).getArr()[2] - points.at(faces.at(j).getArr()[1]).getArr()[2]);
				vect t2 = t.crossProduct(&t1);
				//cross product of b-a and c-b gives you triangle normal; use points.at(faces) to preserve original order points were read in in.
				triangles.at(j).populateNorm(t2); //store the normal as part of the triangle
				temp.add(&t2);
				k++;
			}
		}
		temp.multConst(1.0 / k); //average out all of the normals for the point
		pointNorms.push_back(vect4(temp, 0.0));
		temp = vect(0, 0, 0);
		k = 0;
	}
}

color obj::getAmbient() {
	return ambient;
}

color obj::getDiffuse() {
	return diffuse;
}

color obj::getSpecular() {
	return specular;
}

float obj::getPhong() {
	return phong;
}

std::vector<vect>* obj::getPoints() {
	return &this->points;
}

std::vector<triangle> obj::getTriangles() {
	return this->triangles;
}

vect4 obj::getPointNorm(int loc) {
	return pointNorms.at(loc);
}
