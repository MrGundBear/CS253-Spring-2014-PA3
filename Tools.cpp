#include <Tools.h>


//This class' role is to modify and return images with its functions,
//It can also write images using it's 

Tools::Tools() {
	debug = false;
	if(debug) cout << "Tools are being created" << endl;
}

//This function will take in an img (making a copy of it) and replace all of its
// pixel values with scaled values, so that all pixels have range of 0-255
Image Tools::scaleImage(Image img) {
	int scaleTo = 255;
	//Walk through each pixel in img vector and replace oldValue with newValue
	// newValue = (255 - (oldValue - img.min)) / (img.max - img.min)
	for(unsigned int i = 0; i < img.mainData.size(); i++) {

		
		int intNewValue = scalePixel(img.mainData.at(i), img.min, img.max, scaleTo);
		/*if(debug) cout << "Scaling origPixel(" << img.mainData.at(i) 
				<< ") -> (" << scaleTo << " * (" << img.mainData.at(i)
				<< " - " << img.min << ")) / (" << img.max << " - " 
				<< img.min << ") = " << intNewValue << endl;*/
		
		img.mainData.at(i) = intNewValue;	
		
	}
	//Here we will want to change the maxPixelValue to be 255
	img.maxPixelValue = scaleTo;
	//also lets change the max / min to be 255 / 0 resepctivley
	img.max = scaleTo;
	img.min = 0;
	return img;

}

bool Tools::compareImage(Image img, Image img2) {
	if(img.min != img2.min || img.max != img2.max || img.mean != img2.mean) {
		return false;
	}
	for(unsigned int i = 0; i < img.mainData.size(); i++) {
		if(img.mainData.at(i) != img2.mainData.at(i)) {
			return false;
		}
	}
	return true;
}

//This function will be used by scaleImage to convert to scaled 0-scaleTo (default 255) pixel
int Tools::scalePixel(int val, int min, int max, int scaleTo) {
	//This double value will hold newPixel value	
	double newValue = (scaleTo * (double(val) - min)) / (double(max) - min);
	return doubleToInt(newValue);
}

int Tools::writeImageToFile(Image img, string filename)
{

	//Creating myFile and checking that it is open correctly
	ofstream myFile;
	myFile.open(filename.c_str());
	if(myFile.is_open()) {
		//if the image is an ASCII image, not binary
		if(img.isBinary == false) {
			myFile << "P2 ";
			myFile << img.width << " ";
			myFile << img.height << " ";
			myFile << img.maxPixelValue << endl;

			for(unsigned int i = 0; i < img.mainData.size(); i++) {
				myFile << img.mainData.at(i) << " ";
				//After adding pixelValue to file lets add spaces between values
				if(img.mainData.at(i) < 10) {
					myFile << "  ";
				}
				else if(img.mainData.at(i) < 100) {
					myFile << " ";
				}
			
			
				if((i+1) % img.width == 0 && i>0) {
					myFile << endl;
				}
			}
			myFile.close();
		}
		//else we are printing to a binary file
		else {
			if(debug) cout << "Am Writing in Binary PGM format" << endl;
			//First write P5 to file
			myFile << "P5";
			//You will need to write 4 8-bit characters 
			//char representations of ints
			string byteForm = fromIntToCharArray(img.width);
			//Here we will write our 4 byte width number to the file
			myFile << byteForm;
			
			byteForm = fromIntToCharArray(img.height);
			//Here we will write our 4 byte height number to the file
			myFile << byteForm;

			//Here we will write our 1 byte maxPixelValue number to the file
			byteForm = fromIntToCharArray(img.maxPixelValue);

			myFile << byteForm;
			
			//now we loop through our mainData and write the 1 byte
			// char representation to myFile
			for(unsigned int i = 0; i < img.mainData.size(); i++) {
				myFile << char(img.mainData.at(i));
			}
			myFile.close();
		}
		
	}
	else {
		cerr << "Error writing to file named '" << filename << "'!" << endl;
		return -1;
	}
	return 0;
}

//this function will return a string representation 
// of an int, where the int is a 4 byte int

string Tools::fromIntToCharArray(int val) {
	//if(debug == true) cout << "Starting with number: " << val;
	string result = "";
	if(debug) cout << "From " << val;
	int limit = 16777216; 
	for(int i = 0; i<4; i++) {
		int count = 0;	
		while (val >= limit) {
			count++;
			val -= limit;
			//cout << val+limit << " -> " << val << ", count: " << count <<endl;
		}
		result+=  count;
		//c = (char) count;
		//cout << "int value = " << (int) c << " * " << limit;
		//cout << "adding char " << (char)count << " int val: " + count << endl;
		//cout << endl << "addigng" << count<< " ";
		//result[i] = c;

		//if(debug) cout << " Adding c: '" << c << "'" << endl;
		if(limit == 16777216) limit = 65536;
		else if(limit == 65536) limit = 256;
		else limit = 1;
	}

	//if(debug) cout << " ended up as String '" << result << "'" << endl;
	if(debug) cout << " to " << result[0] << result[1] << result[2] << result[3]  
				<< "   [" << (int)result[0] << "][" << (int)result[1] << "]["
				<< (int)result[2] << "] [" << (int)result[3] << "]"
				<< "  size: " << result.size() << endl;

	return result;
}
 
//This function's sole purpose is to make sure all file names written 
//are in correct Unix form, can only include the following Chars
//[A-Z],[a-z],[0-9],[.],[_],[,]
//bool Tools::checkFileFormat

int Tools::doubleToInt(double val) {
	//By adding 0.5 to my static cast it will round correctly
	int result = static_cast<int>(val + 0.5);	
	//if(debug && double(result) != val) 
			//cout << "Rounding difference! from " << val 
				//<< " to " << result << endl;
	return result;
}
