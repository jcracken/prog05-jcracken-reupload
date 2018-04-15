#include "scene.h"

scene::scene(){ //constructor
  this->width = 0;
  this->height = 0;
  this->angle = 0.0;
}

float** scene::returnData(int type){ //converts data stored locally into a 2D float array and returns it
	int i, j;
  float** returnData;
  returnData = new float*[this->height];
  for (i = 0; i < height; i++)
    returnData[i] = new float[3 * this->width];
  float* temp;
  for(i = 0; i < this->height; i++){
    for(j = 0; j < this->width; j++){
		if (type == 0) { //flat
			temp = (this->data[this->height - i - 1][j]).getArr();
			returnData[i][3 * j] = temp[0];
			returnData[i][3 * j + 1] = temp[1];
			returnData[i][3 * j + 2] = temp[2];
		} else if (type == 1) { //gourand
			temp = (this->gData[this->height - i - 1][j]).getArr();
			returnData[i][3 * j] = temp[0];
			returnData[i][3 * j + 1] = temp[1];
			returnData[i][3 * j + 2] = temp[2];
		} else { //phong
			temp = (this->pColor[this->height - i - 1][j]).getColor().getArr();
			returnData[i][3 * j] = temp[0];
			returnData[i][3 * j + 1] = temp[1];
			returnData[i][3 * j + 2] = temp[2];
		}
    }
  }
  return returnData;
}

int scene::returnWidth(){ //returns width
  return this->width;
}

int scene::returnHeight(){ //returns height
  return this->height;
}

vect4 scene::getEye(){ //returns eye
  return this->eye;
}

vect4 scene::getLookAt(){ //returns lookat
  return this->lookat;
}

vect4 scene::getUp(){ //returns up
  return this->up;
}

float scene::returnAngle(){ //returns angle
  return this->angle;
}


///This takes in a filename (name), opens it, and populates variables based on that file's contents

void scene::acquireData(std::string name) {

	std::ifstream input(name, std::ifstream::in); //open file
	std::string parse; //used for parsing
	std::string objName;
	bool follow = false; //used for parsing
	char type = '\0'; //type of data being read

	light lTemp = light(); //used for parsing
	obj oTemp = obj();

	float temp[3]; //used for parsing
	int line = 0; //used for parsing
	int i = 0; //used for parsing

	//verifies file existence
	if (!(input.is_open())) {
		std::cout << "File not found; try excluding the filename extension" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (!input.eof()) {
		if (getline(input, parse)) { //if file line is empty, skip it
			std::istringstream iss(parse); //convert the line to a string stream to parse data from it
			if (!follow) iss >> type;
			switch (type) { //based on the type of data being read
				case 'e': //eye
					iss >> temp[0];
					iss >> temp[1];
					iss >> temp[2];
					eye = vect4(temp[0], temp[1], temp[2], 1.0);
				break;
				case 'l': //lookat
					iss >> temp[0];
					iss >> temp[1];
					iss >> temp[2];
					lookat = vect4(temp[0], temp[1], temp[2], 1.0);
				break;
				case 'u': //up
					iss >> temp[0];
					iss >> temp[1];
					iss >> temp[2];
					up = vect4(temp[0], temp[1], temp[2], 0.0);
				break;
				case 'f': //fov
					iss >> angle;
				break;
				case 'i': //image dimensions
					iss >> width;
					iss >> height;
					break;
					case 'L': //light
						iss >> temp[0];
						iss >> temp[1];
						iss >> temp[2];
						if (!follow) {
							lTemp.setLoc(vect(temp[0], temp[1], temp[2])); //first read in location
							follow = true;
						} else {
							lTemp.setCol(vect(temp[0], temp[1], temp[2])); //next line is color, so read in that second
							lights.push_back(lTemp);
							follow = false;
						}
				break;
				case 'd': //depth
					iss >> nearDepth;
					iss >> farDepth;
				break;
				case 'M': //object mesh

					if (!follow) { //read in object data
						line = 1;
						iss >> objName;
						oTemp.readData(objName);
						oTemp.storeData();
						objects.push_back(oTemp);
						follow = true;
					} else {
						iss >> temp[0] >> temp[1] >> temp[2]; //read in color data
						if (line == 1) {
							objects.at(i).setAmbient(vect(temp[0], temp[1], temp[2]));
							line = 2;
						} else if (line == 2) {
							objects.at(i).setDiffuse(vect(temp[0], temp[1], temp[2]));
							line = 3;
						} else if (line == 3) {
							objects.at(i).setSpecular(vect(temp[0], temp[1], temp[2]));
							line = 4;
						} else {
							objects.at(i).setPhong(temp[0]);
							line = 0;
							i++; //i is the current location in objects--it starts at 0 and goes up after we add a new obj
							follow = false;
						}
					}
				break;
				default: //there's a line that doesn't make any sense
					std::cout << "input file malformed" << std::endl;
					exit(EXIT_FAILURE);
				break;
			}
		}
	}
	input.close();
}

void scene::setup() {
	float w[3];
	float* u;
	vect temp;
	matrix mTemp, mT2;
	unsigned int i;

	for (i = 0; i < (unsigned)3; i++) { //create w vector
		w[i] = (this->eye.getArr()[i] - this->lookat.getArr()[i]) / sqrt(powf(this->eye.getArr()[0] - this->lookat.getArr()[0], 2.0) + powf(this->eye.getArr()[1] - this->lookat.getArr()[1], 2.0) + powf(this->eye.getArr()[2] - this->lookat.getArr()[2], 2));
	}
	this->w = vect4(w[0], w[1], w[2], 0.0);

	temp = this->up.crossProduct(&this->w);

	u = temp.getArr(); //create u from w

	for (i = 0; i < (unsigned)3; i++) { //normalize u
		if (u[i] == 0.0) u[i] = 0.0;
		else u[i] = u[i] / sqrt(powf(u[0], 2) + powf(u[1], 2) + powf(u[2], 2));
	}

	this->u = vect4(u[0], u[1], u[2], 0.0);

	vect4 te = vect4(u[0], u[1], u[2], 1.0); //because cmake on hardship complains

	temp = this->w.crossProduct(&te); //create v from u and w
	this->v = vect4(temp, 0.0);

	float imageHeight = std::tan(this->angle * (3.141592653589793238463 / 180.0) / 2.0) * std::abs(this->nearDepth) * 2.0; //calculate image height
	float imageWidth = std::tan((this->angle * (3.141592653589793238463 / 180.0) * (this->width / this->height)) / 2.0) * std::abs(this->nearDepth) * 2.0; //calculate image width

	this->r = imageWidth / 2.0;
	this->l = imageWidth / -2.0;
	this->t = imageHeight / 2.0;
	this->b = imageHeight / -2.0;

	for (i = 0; i < (unsigned)4; i++) { //clearing out matrices before calculations
		for (int j = 0; j < 4; j++) {
			mTemp.setVal(i, j, 0.0);
			mT2.setVal(i, j, 0.0);
			M_per.setVal(i, j, 0.0);
			M_cam.setVal(i, j, 0.0);
			M_vp.setVal(i, j, 0.0);
			M.setVal(i, j, 0.0);
		}
	}

	//M_per
	M_per.setVal(0, 0, ((2.0 * this->nearDepth) / (r - l)));
	M_per.setVal(0, 2, ((l + r) / (l - r)));
	M_per.setVal(1, 1, ((2.0 * this->nearDepth) / (t - b)));
	M_per.setVal(1, 2, ((b + t) / (b - t)));
	M_per.setVal(2, 2, ((this->farDepth + this->nearDepth) / (this->nearDepth - this->farDepth)));
	M_per.setVal(2, 3, ((2.0 * this->nearDepth * this->farDepth) / (this->farDepth - this->nearDepth)));
	M_per.setVal(3, 2, 1.0);

	//M_cam
	for (i = 0; i < 3; i++) {
		mTemp.setVal(0, i, this->u.getArr()[i]);
		mTemp.setVal(1, i, this->v.getArr()[i]);
		mTemp.setVal(2, i, this->w.getArr()[i]);
	}
	mTemp.setVal(3, 3, 1.0);
	for (i = 0; i < 4; i++) {
		mT2.setVal(i, i, 1.0);
	}
	mT2.setVal(0, 3, -1.0 * this->eye.getArr()[0]);
	mT2.setVal(1, 3, -1.0 * this->eye.getArr()[1]);
	mT2.setVal(2, 3, -1.0 * this->eye.getArr()[2]);

	mTemp.mult(&mT2, &M_cam);

	//M_vp
	M_vp.setVal(0, 0, (this->width / 2.0));
	M_vp.setVal(1, 1, (this->height / 2.0));
	M_vp.setVal(2, 2, 1.0);
	M_vp.setVal(3, 3, 1.0);
	M_vp.setVal(0, 3, ((this->width - 1.0) / 2.0));
	M_vp.setVal(1, 3, ((this->height - 1.0) / 2.0));

	mTemp = matrix();

	M_vp.mult(&M_per, &mTemp);
	mTemp.mult(&M_cam, &M);

	this->genPixelData(imageHeight, imageWidth); //basically create and clears out a bunch of 2D arrays
	for (i = 0; i < objects.size(); i++) { //create point and triangle norms
		objects.at(i).storeData();
		objects.at(i).pointPopulate();
	}

	this->flat(); //create data for flat and gourand shading
	this->gourand();
	
	for (i = 0; i < this->objects.size(); i++) { //transform all vertices
		std::vector<vect>* t = this->objects.at(i).getPoints();
		for (unsigned int j = 0; j < t->size(); j++) {
			vect4 t1 = vect4(t->at(j), 1.0);
			M.mult(&t1, &mTemp);
			t->at(j) = vect(mTemp.getVal(0, 0) / mTemp.getVal(3, 0), mTemp.getVal(1, 0) / mTemp.getVal(3, 0), mTemp.getVal(2, 0) / mTemp.getVal(3, 0));
		}
		objects.at(i).storeData();
	}
}

void scene::draw() {
	float zTemp;
	float w0, w1, w2;
	int boundXmax, boundXmin, boundYmax, boundYmin;
	std::vector<triangle> t;
	std::vector<vect>* p;
	vect a, b, c;
	float temp[3];
	float atemp[3];
	bool pA = false;
	bool pB = false;
	bool pC = false;
	for (unsigned int i = 0; i < this->objects.size(); i++) {
		t = this->objects.at(i).getTriangles(); //we copy these over for each object because this shaves off minutes of copying over the entire object to cache over and over
		p = this->objects.at(i).getPoints();
		for (unsigned int j = 0; j < t.size(); j++) {
			boundYmax = ceil(t.at(j).boundYMax()); //find bounding values
			boundYmin = floor(t.at(j).boundYMin());
			for (unsigned int m = boundYmin; m < boundYmax; m++) {
				boundXmax = ceil(t.at(j).boundXMax()); //ditto
				boundXmin = floor(t.at(j).boundXMin());
				for (unsigned int n = boundXmin; n < boundXmax; n++) {
					if (m < height && n < width) { //to make sure we're not trying to draw outside of the box
						if (t.at(j).intersect(pixelLoc[m][n], &zTemp, &w0, &w1, &w2)) {
							if (zTemp > z[m][n]) { //if z is closer
								z[m][n] = zTemp;
								data[m][n] = fColor[i][j].getColor(); //flat data stored
								
								a = t.at(j).getPoint(0);
								b = t.at(j).getPoint(1);
								c = t.at(j).getPoint(2);

								temp[0] = 0.0;
								temp[1] = 0.0;
								temp[2] = 0.0;

								atemp[0] = 0.0;
								atemp[1] = 0.0;
								atemp[2] = 0.0;

								pA = false;
								pB = false;
								pC = false;

								for (unsigned int x = 0; x < p->size() && !(pA && pB && pC); x++) { //interpolate phong and gourand
									if (p->at(x).comp(a)) {
										temp[0] = temp[0] + gColor[i][x].getColor().getArr()[0] * w0;
										temp[1] = temp[1] + gColor[i][x].getColor().getArr()[1] * w0;
										temp[2] = temp[2] + gColor[i][x].getColor().getArr()[2] * w0;
										atemp[0] = atemp[0] + this->objects.at(i).getPointNorm(x).getArr()[0] * w0;
										atemp[1] = atemp[1] + this->objects.at(i).getPointNorm(x).getArr()[1] * w0;
										atemp[2] = atemp[2] + this->objects.at(i).getPointNorm(x).getArr()[2] * w0;
										pA = true;
									}
									if (p->at(x).comp(b)) {
										temp[0] = temp[0] + gColor[i][x].getColor().getArr()[0] * w1;
										temp[1] = temp[1] + gColor[i][x].getColor().getArr()[1] * w1;
										temp[2] = temp[2] + gColor[i][x].getColor().getArr()[2] * w1;
										atemp[0] = atemp[0] + this->objects.at(i).getPointNorm(x).getArr()[0] * w1;
										atemp[1] = atemp[1] + this->objects.at(i).getPointNorm(x).getArr()[1] * w1;
										atemp[2] = atemp[2] + this->objects.at(i).getPointNorm(x).getArr()[2] * w1;
										pB = true;
									}
									if (p->at(x).comp(c)) {
										temp[0] = temp[0] + gColor[i][x].getColor().getArr()[0] * w2;
										temp[1] = temp[1] + gColor[i][x].getColor().getArr()[1] * w2;
										temp[2] = temp[2] + gColor[i][x].getColor().getArr()[2] * w2;
										atemp[0] = atemp[0] + this->objects.at(i).getPointNorm(x).getArr()[0] * w2;
										atemp[1] = atemp[1] + this->objects.at(i).getPointNorm(x).getArr()[1] * w2;
										atemp[2] = atemp[2] + this->objects.at(i).getPointNorm(x).getArr()[2] * w2;
										pC = true;
									}
								}
								gData[m][n] = vect(temp[0], temp[1], temp[2]); //store gourand

								pData[m][n] = vect(atemp[0], atemp[1], atemp[2]); //store this data for phong; will be used after rasterization
								pObj[m][n] = i;
								pLocs[m][n] = vect(m, n, z[m][n]);
							}
						}
					}
				}
			}
		}
	}
	this->phong(); //calculate phong values
}

vect scene::shading(vect n, vect v, vect ambient, vect diffuse, vect specular, float phong) { //blinn-phong calculation
	float light[3] = { (float)ambient.getArr()[0] * 0.5f, (float)ambient.getArr()[1] * 0.5f, (float)ambient.getArr()[2] * 0.5f };
	for (unsigned int i = 0; i < lights.size(); i++) {

		//this is the "correct" way, with l being the direction of the light
		vect t = vect(this->eye.getArr()[0] - v.getArr()[0], this->eye.getArr()[1] - v.getArr()[1], this->eye.getArr()[2] - v.getArr()[2]);
		vect l = vect(lights.at(i).getLoc().getArr()[0] - v.getArr()[0], lights.at(i).getLoc().getArr()[1] - v.getArr()[1], lights.at(i).getLoc().getArr()[2] - v.getArr()[2]);
		t.normalize();
		l.normalize();
		n.normalize();
		v.normalize();
		vect h = half(t, l);
		h.normalize();
		light[0] = light[0] + diffuse.getArr()[0] * lights.at(i).getCol().getColor().getArr()[0] * std::max(0.0f, n.dotProduct(&l)) + specular.getArr()[0] * lights.at(i).getCol().getColor().getArr()[0] * std::powf(std::max(0.0f, n.dotProduct(&h)), phong);
		light[1] = light[1] + diffuse.getArr()[1] * lights.at(i).getCol().getColor().getArr()[1] * std::max(0.0f, n.dotProduct(&l)) + specular.getArr()[1] * lights.at(i).getCol().getColor().getArr()[0] * std::powf(std::max(0.0f, n.dotProduct(&h)), phong);
		light[2] = light[2] + diffuse.getArr()[2] * lights.at(i).getCol().getColor().getArr()[2] * std::max(0.0f, n.dotProduct(&l)) + specular.getArr()[2] * lights.at(i).getCol().getColor().getArr()[0] * std::powf(std::max(0.0f, n.dotProduct(&h)), phong);

		//this is the "wrong" way, with l being the light's location
		/*vect t = vect(this->eye.getArr()[0] - v.getArr()[0], this->eye.getArr()[1] - v.getArr()[1], this->eye.getArr()[2] - v.getArr()[2]);
		vect l = vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2]);
		t.normalize();
		l.normalize();
		n.normalize();
		v.normalize();
		vect h = half(t, l);
		h.normalize();
		light[0] = light[0] + diffuse.getArr()[0] * lights.at(i).getCol().getColor().getArr()[0] * std::max(0.0f, n.dotProduct(&vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2]))) + specular.getArr()[0] * lights.at(i).getCol().getColor().getArr()[0] * std::powf(std::max(0.0f, n.dotProduct(&h)), phong);
		light[1] = light[1] + diffuse.getArr()[1] * lights.at(i).getCol().getColor().getArr()[1] * std::max(0.0f, n.dotProduct(&vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2]))) + specular.getArr()[1] * lights.at(i).getCol().getColor().getArr()[0] * std::powf(std::max(0.0f, n.dotProduct(&h)), phong);
		light[2] = light[2] + diffuse.getArr()[2] * lights.at(i).getCol().getColor().getArr()[2] * std::max(0.0f, n.dotProduct(&vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2]))) + specular.getArr()[2] * lights.at(i).getCol().getColor().getArr()[0] * std::powf(std::max(0.0f, n.dotProduct(&h)), phong);*/
	}
	return vect(light[0], light[1], light[2]);
}

//basically, c throws a hissy fit if you try to declare a 2d array with non-static values, so this was the compromise
void scene::genPixelData(float imageHeight, float imageWidth) {
	data = new vect*[this->height];
	gData = new vect*[this->height];
	pData = new vect*[this->height];
	pLocs = new vect*[this->height];
	pObj = new int*[this->height];
	z = new float*[this->height];
	pixelLoc = new vect*[this->height];
	for (int i = 0; i < this->height; i++) {
		data[i] = new vect[this->width];
		gData[i] = new vect[this->width];
		pData[i] = new vect[this->width];
		pLocs[i] = new vect[this->width];
		pObj[i] = new int[this->width];
		pixelLoc[i] = new vect[this->width];
		z[i] = new float[this->width];
		for (int j = 0; j < this->width; j++) {
			pixelLoc[i][j] = vect(imageWidth / -2.0 + j, imageHeight / -2.0 + i, nearDepth);
			z[i][j] = -INT_MAX;
			pObj[i][j] = INT_MAX;
		}
	}
}

void scene::gourand() { //calculate gourand values
	std::vector<vect>* p;
	vect ambient, diffuse, specular;
	float phong;
	for (unsigned int i = 0; i < objects.size(); i++) {
		gColor.push_back(std::vector<color>());
		p = objects.at(i).getPoints();
		ambient = objects.at(i).getAmbient().getColor(); //copying these over saves us minutes of time
		diffuse = objects.at(i).getDiffuse().getColor();
		specular = objects.at(i).getSpecular().getColor();
		phong = objects.at(i).getPhong();
		for (unsigned int j = 0; j < p->size(); j++) {
			color c = color();
			vect v = vect(objects.at(i).getPointNorm(j).getArr()[0], objects.at(i).getPointNorm(j).getArr()[1], objects.at(i).getPointNorm(j).getArr()[2]);
			c.setColor(shading(v, vect(p->at(j).getArr()[0], p->at(j).getArr()[1], p->at(j).getArr()[2]), ambient, diffuse, specular, phong));
			gColor[i].push_back(c);
		}
	}
}

void scene::flat() { //calculate flat values
	vect ambient, diffuse, specular;
	float phong;
	for (unsigned int i = 0; i < objects.size(); i++) {
		fColor.push_back(std::vector<color>());
		std::vector<triangle> t = objects.at(i).getTriangles(); //copying these over saves us minutes of time
		ambient = objects.at(i).getAmbient().getColor();
		diffuse = objects.at(i).getDiffuse().getColor();
		specular = objects.at(i).getSpecular().getColor();
		phong = objects.at(i).getPhong();
		for (unsigned int j = 0; j < t.size(); j++) {
			color c = color();
			vect v = vect(0.0, 0.0, 0.0);
			for (int k = 0; k < 3; k++) {
				vect b = t.at(j).getPoint(k); //because hardship threw a fit
				v.add(&b);
			}
			v.multConst(1 / 3.0);
			c.setColor(shading(t.at(j).getNorm(), v, ambient, diffuse, specular, phong));
			fColor[i].push_back(c);
		}
	}
}

void scene::phong() { //calculate phong values
	vect ambient, diffuse, specular;
	float phong;
	for (int i = 0; i < this->height; i++) {
		pColor.push_back(std::vector<color>());
		for (int j = 0; j < this->width; j++) {
			color c = color();
			vect v = vect(pData[i][j].getArr()[0], pData[i][j].getArr()[1], pData[i][j].getArr()[2]);
			if (pObj[i][j] == INT_MAX) c.setColor(vect(0, 0, 0)); //only do this if there are actual values for this pixel
			else {
				ambient = objects.at(pObj[i][j]).getAmbient().getColor();
				diffuse = objects.at(pObj[i][j]).getDiffuse().getColor();
				specular = objects.at(pObj[i][j]).getSpecular().getColor();
				phong = objects.at(pObj[i][j]).getPhong();
				c.setColor(shading(v, vect(pLocs[i][j].getArr()[0], pLocs[i][j].getArr()[1], pLocs[i][j].getArr()[2]), ambient, diffuse, specular, phong));
			}
			pColor[i].push_back(c);
		}
	}
}

vect scene::half(vect m, vect n) { //calculates half vector for specular
	float temp[3];
	float* mVal = m.getArr();
	float* nVal = n.getArr();

	temp[0] = (mVal[0] + nVal[0]) / (std::powf(std::powf(mVal[0] + nVal[0], 2) + std::powf(mVal[1] + nVal[1], 2) + std::powf(mVal[2] + nVal[2], 2), 0.5));
	temp[1] = (mVal[1] + nVal[1]) / (std::powf(std::powf(mVal[0] + nVal[0], 2) + std::powf(mVal[1] + nVal[1], 2) + std::powf(mVal[2] + nVal[2], 2), 0.5));
	temp[2] = (mVal[2] + nVal[2]) / (std::powf(std::powf(mVal[0] + nVal[0], 2) + std::powf(mVal[1] + nVal[1], 2) + std::powf(mVal[2] + nVal[2], 2), 0.5));

	return vect(temp[0], temp[1], temp[2]);
}
