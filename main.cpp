/*! \file main.cpp: main function for complex number calculator */
#include <Image.h>
#include <Tools.h>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
#include <fstream>

int main(int argc, char* argv[])
{
	bool debug = false;
	Image img;
	Tools tools;

	if(argc == 1) {
		cerr << "Improper number of args, should be 3 but is " << argc << "!" << endl;
		return -1;
	}

	string input(argv[1]);
	if(input.compare("print") == 0) {
		//This means we are using pa2 to print an image
		img.parseFile(argv[2]);	
		img.printImage();
		return -1;	
	}
	//using pa2 to compare if two images are equal
	else if(input.compare("compare") == 0) {
		//This means we are using pa2 to print an image
		img.parseFile(argv[2]);	
		Image img2;
		img2.parseFile(argv[3]);
		bool compare = tools.compareImage(img, img2);
		if(compare) cout << "These are the Same Image!" << endl;
		else cout << "These images are Not the same!" << endl;
		return -1;	
	}
	else if(input.compare("type") == 0) {
		//This means we are using pa2 to print an image
		img.parseFile(argv[2]);	
		bool compare = img.isBinary;
		if(compare) cout << "This is a Binary Image!" << endl;
		else cout << "This is an ASCII image!" << endl;
		return -1;	
	}

	if(argc > 3 || argc < 3) {
		cerr << "Improper number of args, should be 3 but is " << argc << "!" << endl;
		return -1;
	}
	


	int imgResult = img.parseFile(argv[1]);	
	if(imgResult < 0) {
		
		if(debug) cerr << "image Read failed, returned code " << imgResult << endl;
		return -1;
	}

	//Img should be safely read at this point
	
	Image scaledImage = tools.scaleImage(img);
	
	//Force a binary save for PA3
	scaledImage.isBinary = true;
	
	input = argv[2];
	//Here we will check if the original file had .pgma extension
	//If it does, write file in ASCII form, else in Binary form
	if(input.size() > 4) {
		input = input.substr(input.size()-5, input.size());
		if(debug) cout << " Output file ending in: " << input << endl;
		if(input.compare(".pgma") == 0) {
			//if input file ends in .pgma you want to write in ascii
			scaledImage.isBinary = false;
		}
	}

	

	int toolResult = tools.writeImageToFile(scaledImage, argv[2]);

	//img.printImage();
	//cout << "returning " << result;
	if(scaledImage.goodImage == false || toolResult < 0) {
		return -1;
	}
	//Succes case
	return 0;
}


