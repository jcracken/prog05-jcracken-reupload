#include "ppm.h"

ppm::ppm(){ //constructor
  this->data = NULL;
  this->width = 0;
  this->height = 0;
  this->maxVal = 255;
}
//loads in image data from filename argument
void ppm::readData(std::string name){
  std::ifstream input(name, std::ifstream::in | std::ifstream::binary);
  std::string temp;
  int i = 1;
  //verifies file existence
  if(!(input.is_open())){
    std::cout << "File not found; try excluding the filename extension" << std::endl;
    exit(EXIT_FAILURE);
  }
  //verifies file is of right format
  getline(input, temp);
  if(temp.compare("P6") != 0){
    std::cout << "File not correct format" << std::endl;
    exit(EXIT_FAILURE);
  }
  //parse header
  while(i < 3){
    getline(input, temp);
    //skip commented out lines
    if(temp.at(0) == '#');
    else if(i == 1){
      std::istringstream iss(temp);
      iss >> this->width;
      iss >> this->height;
      i = 2;
    }
    else if(i == 2){
      std::istringstream iss(temp);
      iss >> this->maxVal;
      i = 3;
    }
  }
  //remove any remaining whitespace
  input >> std::ws;
  //clear array used for loaded data, just in case
  this->data = new unsigned char[3*this->height*this->width];
  for(i = 0; i < 3*this->height*this->width; i++){
    this->data[i] = 0;
  }
  //load in data and close file
  input.read((char*)this->data, 3*this->height*this->width);
  input.close();
  //used for debugging purposes
  //writeData("temp");
}
//returns pixel data
unsigned char* ppm::returnData(){
  return this->data;
}
//returns image width
int ppm::returnWidth(){
  return this->width;
}
//returns image height
int ppm::returnHeight(){
  return this->height;
}
//returns max val variable
int ppm::returnMaxVal(){
  return this->maxVal;
}
//used for debugging
void ppm::writeData(std::string name){
  std::ofstream out(name, std::ofstream::out | std::ofstream::binary);
  //write header
  out << "P6\n" << this->width << " " << this->height << "\n" << this->maxVal << "\n";
  //write pixel data
  out.write((char*)this->data, this->width * this->height * 3);
  //close file
  out.close();
}

void ppm::setData(unsigned char* data){
  this->data = data;
}

void ppm::setData(float** data, int height, int width){ //if float array is passed in, convert to unsigned char first
  int i, j, k = 0;
  float min = -5000.0;
  unsigned char** tempData = new unsigned char*[height];
  for (i = 0; i < height; i++) {
	  tempData[i] = new unsigned char[width];
  }
  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
      if(data[i][j] < min) min = data[i][j];
    }
  }
  if(min < 0.0) min = 0.0;
  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
      tempData[i][j] = data[i][j] * 255;
      if(tempData[i][j] < 0) tempData[i][j] = 0;
      if(tempData[i][j] > 255) tempData[i][j] = 255;
      this->data[k] = tempData[i][j];
      k++;
    }
  }
}

void ppm::setWidth(int width){ //set width
  this->width = width;
}

void ppm::setHeight(int height){ //set height
  this->height = height;
}
