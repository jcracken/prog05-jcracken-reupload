#include "scene.h"

scene::scene(){ //constructor
  this->width = 0;
  this->height = 0;
  this->angle = 0.0;
}

float** scene::returnData(){ //converts data stored locally into a 2D float array and returns it
  int i, j, a = 0, b = 0;
  float** returnData;
  returnData = new float*[this->height];
  for (i = 0; i < height; i++)
    returnData[i] = new float[3 * this->width];
  float* temp;
  for(i = 0; i < this->height; i++){
    for(j = 0; j < this->width; j++){
      //temp = (this->data[i][j]).getColor();
      returnData[a][b] = temp[0];
      returnData[a][b + 1] = temp[1];
      returnData[a][b + 2] = temp[3];
      a++;
      b = b + 3;
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

vect scene::getEye(){ //returns eye
  return this->eye;
}

vect scene::getLookAt(){ //returns lookat
  return this->lookat;
}

vect scene::getUp(){ //returns up
  return this->up;
}

float scene::returnAngle(){ //returns angle
  return this->angle;
}

///This takes in a filename (name), opens it, and populates variables based on that file's contents

/*void scene::acquireData(std::string name){

  std::ifstream input(name, std::ifstream::in); //open file
  std::string parse; //used for parsing
  bool follow = false; //used for parsing
  char type = '\0'; //type of data being read

  light lTemp = light(); //used for parsing

  float temp[3]; //used for parsing
  int line = 0; //used for parsing
  int i = 0; //used for parsing

  //verifies file existence
  if(!(input.is_open())){
    std::cout << "File not found; try excluding the filename extension" << std::endl;
    exit(EXIT_FAILURE);
  }
  while(!input.eof()){
	  if (getline(input, parse)) { //if file line is empty, skip it
		  if (parse.at(0) >= 65 && parse.at(0) <= 122) { //if line starts with char, figure out what data on that line
			  type = parse.at(0);
			  follow = false;
		  }
		  else follow = true; //if it doesn't start with a char, it's adding to the last line (or two lines ago)
		  std::istringstream iss(parse); //convert the line to a string stream to parse data from it
		  switch (type) { //based on the type of data being read
			  case 'e': //eye
				  iss >> eye[0];
				  iss >> eye[1];
				  iss >> eye[2];
			  break;
			  case 'l': //lookat
				  iss >> lookat[0];
				  iss >> lookat[1];
				  iss >> lookat[2];
			  break;
			  case 'u': //up
				  iss >> up[0];
				  iss >> up[1];
				  iss >> up[2];
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
					  lTemp.setLoc(temp); //first read in location
				  }
				  else {
					  lTemp.setCol(temp); //next line is color, so read in that second
					  lights.push_back(lTemp);
				  }
			  break;
			  case 'P': //plane
				  iss >> temp[0];
				  if (line != 4) { //last line only has one value to read in
					  iss >> temp[1];
					  iss >> temp[2];
				  }
				  if (!follow) {
					  surf.push_back(new plane()); //add new plane to list of surfaces
					  surf.at(i)->setPos(temp); //set position to what was read in
					  iss >> temp[0];
					  iss >> temp[1];
					  iss >> temp[2];
					  surf.at(i)->setNormal(temp); //read in normal and save it
					  line = 1;
				  }
				  else {
					  if (line == 1) {
						  surf.at(i)->setAmbient(temp); //store ambient
						  line = 2;
					  }
					  else if (line == 2) {
						  surf.at(i)->setDiffuse(temp); //store diffuse
						  line = 3;
					  }
					  else if (line == 3) {
						  surf.at(i)->setSpecular(temp); //store specular
						  line = 4;
					  }
					  else {
						  surf.at(i)->setPhong(temp[0]); //store phong
						  line = 0;
						  i++;
					  } //line denotes how many lines of the current plane data has been read in.
				  }
			  break;
			  case 'S': //this is sphere; very similar to the last case, but with a sphere instead
				  iss >> temp[0];
				  if (line != 4) {
					  iss >> temp[1];
					  iss >> temp[2];
				  }
				  if (!follow) {
					  surf.push_back(new sphere());
					  surf.at(i)->setPos(temp);
					  iss >> temp[0];
					  surf.at(i)->setRadius(temp[0]); //instead of normal, there's a radius
					  line = 1;
				  }
				  else {
					  if (line == 1) {
						  surf.at(i)->setAmbient(temp);
						  line = 2;
					  }
					  else if (line == 2) {
						  surf.at(i)->setDiffuse(temp);
						  line = 3;
					  }
					  else if (line == 3) {
						  surf.at(i)->setSpecular(temp);
						  line = 4;
					  }
					  else {
						  surf.at(i)->setPhong(temp[0]);
						  line = 0;
						  surf.at(i)->setType();
						  i++; //i is the current location in surf--it starts at 0 and goes up after we add a new surf
					  }
				  }
				  break;
			  case 's': //read in samples for anti-aliasing
				  iss >> this->samples;
				  break;
			  case 'A': //area light
				  iss >> temp[0];
				  iss >> temp[1];
				  iss >> temp[2];
				  if (!follow) {
					  lTemp.setLoc(temp);
					  iss >> temp[0];
					  iss >> temp[1];
					  iss >> temp[2];
					  lTemp.setA(temp); //A vector
					  iss >> temp[0];
					  iss >> temp[1];
					  iss >> temp[2];
					  lTemp.setB(temp); //B vector
				  }
				  else {
					  lTemp.setCol(temp); //color
					  lights.push_back(lTemp);
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
}*/