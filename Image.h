#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE


#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cmath>        



using std::abs;
using std::istream;
using std::ostream;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::cerr;


class Image {
public:
	Image();
	int parseFile(string fileName);
	
	void printImage();
	
	// mainData = all rows of int data, not including the PG
	vector<int> mainData;
	// width and height will correspond to the dimensions of image
	int width, height, maxPixelValue;
	bool debug;

	int max, min;
	double mean;
	//safetyFlag is a
	bool goodImage;
	bool isBinary;
private:

	int charToInt(char c);
	int continueParseInAscii(ifstream& myFile);
	int continueParseInBinary(ifstream& myFile);
	int fromBinaryToInt(char* arr);

	string toGrayScale(unsigned int val);
	bool isInteger(const string & s);
	int nextValidValue(ifstream& openFile);

};


#endif
