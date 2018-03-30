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

void scene::acquireData(std::string name){

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
  if(!(input.is_open())){
    std::cout << "File not found; try excluding the filename extension" << std::endl;
    exit(EXIT_FAILURE);
  }
  while(!input.eof()){
	  if (getline(input, parse)) { //if file line is empty, skip it
		  if (parse[0] >= 65 && parse[0] <= 122) { //if line starts with char, figure out what data on that line
			  follow = false;
		  }
		  else follow = true; //if it doesn't start with a char, it's adding to the last line (or two lines ago)
		  std::istringstream iss(parse); //convert the line to a string stream to parse data from it
		  if (!follow) iss >> type;
		  switch (type) { //based on the type of data being read
			  case 'e': //eye
				  iss >> temp[0];
				  iss >> temp[1];
				  iss >> temp[2];
				  eye = vect(temp[0], temp[1], temp[2]);
			  break;
			  case 'l': //lookat
				  iss >> temp[0];
				  iss >> temp[1];
				  iss >> temp[2];
				  lookat = vect(temp[0], temp[1], temp[2]);
			  break;
			  case 'u': //up
				  iss >> temp[0];
				  iss >> temp[1];
				  iss >> temp[2];
				  up = vect(temp[0], temp[1], temp[2]);
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
				  }
				  else {
					  lTemp.setCol(vect(temp[0], temp[1], temp[2])); //next line is color, so read in that second
					  lights.push_back(lTemp);
				  }
			  break;
			  case 'd': //depth
				  iss >> nearDepth;
				  iss >> farDepth;
			  break;
			  case 'M': //object mesh
				  iss >> objName;
				  oTemp.readData(objName);
				  objects.push_back(oTemp);
				  //colors for the object
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