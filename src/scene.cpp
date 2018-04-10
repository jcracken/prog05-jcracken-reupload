#include "scene.h"

scene::scene(){ //constructor
  this->width = 0;
  this->height = 0;
  this->angle = 0.0;
}

float** scene::returnData(int type){ //converts data stored locally into a 2D float array and returns it
  int i, j, a = 0, b = 0;
  float** returnData;
  returnData = new float*[this->height];
  for (i = 0; i < height; i++)
    returnData[i] = new float[3 * this->width];
  float* temp;
  for(i = 0; i < this->height; i++){
    for(j = 0; j < this->width; j++){
		if (type == 0) { //flat
			temp = (this->data[i][j]).getArr();
			returnData[i][3 * j] = temp[0];
			returnData[i][3 * j + 1] = temp[1];
			returnData[i][3 * j + 2] = temp[2];
		} else if (type == 1) { //gourand
			temp = (this->data[i][j]).getArr();
			returnData[i][3 * j] = temp[0];
			returnData[i][3 * j + 1] = temp[1];
			returnData[i][3 * j + 2] = temp[2];
		} else { //phong
			temp = (this->data[i][j]).getArr();
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

					if (!follow) {
						line = 1;
						iss >> objName;
						oTemp.readData(objName);
						oTemp.storeData();
						objects.push_back(oTemp);
						follow = true;
					} else {
						iss >> temp[0] >> temp[1] >> temp[2];
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

	for (i = 0; i < (unsigned)3; i++) {
		w[i] = (this->eye.getArr()[i] - this->lookat.getArr()[i]) / sqrt(powf(this->eye.getArr()[0] - this->lookat.getArr()[0], 2.0) + powf(this->eye.getArr()[1] - this->lookat.getArr()[1], 2.0) + powf(this->eye.getArr()[2] - this->lookat.getArr()[2], 2));
	}
	this->w = vect4(w[0], w[1], w[2], 0.0);

	temp = this->up.crossProduct(&this->w);

	u = temp.getArr();

	for (i = 0; i < (unsigned)3; i++) {
		if (u[i] == 0.0) u[i] = 0.0;
		else u[i] = u[i] / sqrt(powf(u[0], 2) + powf(u[1], 2) + powf(u[2], 2));
	}

	this->u = vect4(u[0], u[1], u[2], 0.0);

	temp = this->w.crossProduct(&vect4(u[0], u[1], u[2], 1.0));
	this->v = vect4(temp, 0.0);

	float dist = powf((powf(this->eye.getArr()[0] - this->lookat.getArr()[0], 2.0)) + (powf(this->eye.getArr()[1] - this->lookat.getArr()[1], 2.0)) + (powf(this->eye.getArr()[2] - this->lookat.getArr()[2], 2.0)), 0.5);
	float imageHeight = std::tan(this->angle * (3.141592653589793238463 / 180.0) / 2.0) * std::abs(this->nearDepth) * 2.0; //calculate image height
	float imageWidth = std::tan((this->angle * (3.141592653589793238463 / 180.0) * (this->width / this->height)) / 2.0) * std::abs(this->nearDepth) * 2.0; //calculate image width

	this->r = imageWidth / 2.0;
	this->l = imageWidth / -2.0;
	this->t = imageHeight / 2.0;
	this->b = imageHeight / -2.0;

	for (i = 0; i < (unsigned)4; i++) {
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
	
	for (i = 0; i < this->objects.size(); i++) {
		std::vector<vect>* t = this->objects.at(i).getPoints();
		for (unsigned int j = 0; j < t->size(); j++) {
			vect4 t1 = vect4(t->at(j), 1.0);
			M.mult(&t1, &mTemp);
			t->at(j) = vect(mTemp.getVal(0, 0), mTemp.getVal(1, 0), mTemp.getVal(2, 0));
		}
		objects.at(i).storeData();
		objects.at(i).pointPopulate();
	}
	this->genPixelData(imageHeight, imageWidth);
	this->flat();
	//this->gourand();
	//this->phong();
}

void scene::draw() {
	float zTemp;
	float w0, w1, w2;
	int boundXmax, boundXmin, boundYmax, boundYmin;
	std::vector<triangle> t;
	for (unsigned int i = 0; i < this->objects.size(); i++) {
		t = this->objects.at(i).getTriangles();
		for (unsigned int j = 0; j < t.size(); j++) {
			boundXmax = ceil(t.at(j).boundXMax());
			if (boundXmax > this->height) boundXmax = height;
			if (boundXmax < 0) continue;
			boundXmin = floor(t.at(j).boundXMin());
			if (boundXmin < 0) boundXmin = 0;
			if (boundXmin > 800) continue;
			for (unsigned int m = boundXmin; m < boundXmax; m++) {
				boundYmax = ceil(t.at(j).boundYMax());
				if (boundYmax > this->height) boundYmax = height;
				if (boundYmax < 0) continue;
				boundYmin = floor(t.at(j).boundYMin());
				if (boundYmin < 0) boundYmin = 0;
				if (boundYmin > 800) continue;
				for (unsigned int n = boundYmin; n < boundYmax; n++) {
					if (m < height && n < width) {
						if (t.at(j).intersect(pixelLoc[m][n], &zTemp, &w0, &w1, &w2)) {
							if (zTemp < z[m][n]) {
								z[m][n] = zTemp;
								data[m][n] = fColor[i][j].getColor();
							}
						}
					}
				}
			}
		}
	}
}

vect scene::shading(vect n, obj o) { //blinn-phong
	float light[3] = { 0.0 };
	for (unsigned int i = 0; i < lights.size(); i++) {
		light[0] = light[0] + o.getDiffuse().getColor().getArr()[0] * lights.at(i).getCol().getColor().getArr()[0] * std::max(0.0f, n.dotProduct(&vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2])));
		light[1] = light[1] + o.getDiffuse().getColor().getArr()[1] * lights.at(i).getCol().getColor().getArr()[1] * std::max(0.0f, n.dotProduct(&vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2])));
		light[2] = light[2] + o.getDiffuse().getColor().getArr()[2] * lights.at(i).getCol().getColor().getArr()[2] * std::max(0.0f, n.dotProduct(&vect(lights.at(i).getLoc().getArr()[0], lights.at(i).getLoc().getArr()[1], lights.at(i).getLoc().getArr()[2])));
	}
	return vect(light[0], light[1], light[2]);
}

void scene::genPixelData(float imageHeight, float imageWidth) {
	data = new vect*[this->height];
	for (int i = 0; i < this->height; i++) {
		data[i] = new vect[this->width];
	}
	gData = new vect*[this->height];
	for (int i = 0; i < this->height; i++) {
		gData[i] = new vect[this->width];
	}
	pData = new vect*[this->height];
	for (int i = 0; i < this->height; i++) {
		pData[i] = new vect[this->width];
	}
	pixelLoc = new vect*[this->height];
	for (int i = 0; i < this->height; i++) {
		pixelLoc[i] = new vect[this->width];
	}
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			pixelLoc[i][j] = vect(imageWidth / -2.0 + j, imageHeight / -2.0 + i, nearDepth);
		}
	}
	z = new float*[this->height];
	for (int i = 0; i < this->height; i++) {
		z[i] = new float[this->width];
	}
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			z[i][j] = INT_MAX;
		}
	}
}

void scene::gourand() {
	for (unsigned int i = 0; i < objects.size(); i++) {
		gColor.push_back(std::vector<color>());
		for (unsigned int j = 0; j < objects.at(i).getPoints()->size(); j++) {
			color c = color();
			vect v = vect(objects.at(i).getPointNorm(j).getArr()[0], objects.at(i).getPointNorm(j).getArr()[1], objects.at(i).getPointNorm(j).getArr()[2]);
			c.setColor(shading(v, objects.at(i)));
			gColor[i].push_back(c);
		}
	}
}

void scene::flat() {
	for (unsigned int i = 0; i < objects.size(); i++) {
		fColor.push_back(std::vector<color>());
		for (unsigned int j = 0; j < objects.at(i).getTriangles().size(); j++) {
			color c = color();
			c.setColor(shading(objects.at(i).getTriangles().at(j).getNorm(), objects.at(i)));
			fColor[i].push_back(c);
		}
	}
}

void scene::phong() {
	//interpolate normals
}
