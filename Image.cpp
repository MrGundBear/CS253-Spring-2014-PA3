#include <Image.h>

//The Image class has two main functions
// To store all data about an Image's pixels, including width, height, mean, etc
// To contain the original parseFile function to read images from the disk

Image::Image(void) {
	debug = false;
	if(debug) cout << "Image is being created" << endl;
}

//This will return the int representation of a binary ascii array
//Char array must be of size 4
int Image::fromBinaryToInt(char* arr) {
	
	//chars whos value is 255 will be shown as -1,
	//if value is negative, add 256 and continue
	int value = 0;
	int limit = 1;
	for(int i =3; i>=0; i--) {
		int cVal = static_cast<int>(arr[i]);
		if(cVal < 0) cVal += 256;
		value += cVal * limit;
		if(limit == 1) limit = 256;
		else if(limit == 256) limit = 65536;
		else limit = 16777216;
		//cout << " adding " << arr[i] << " total now: " << value << endl;

	}
	return value;
}


//This function continues to parse binary image and sets up mainData, width, height, maxPixelValue
int Image::continueParseInBinary(ifstream& myFile) {

	//So here we expect a myFile with no P5 in it
	//We will read in 4 bytes for the width
	//next 4 bytes are height
	//next 1 byte is maxPixelValue
	//next width*height bytes is all pixels that will occupy mainData.
	
	if(debug) cout << "This is a Binary image!" << endl;
	
	width = 0;
	height = 0;
	while(width == 0 || height == 0) {
		//data will hold 4 bytes of data
		char data[4];
	
		myFile.read(data, 4);
		//This function will go from 0,0,0,24 -> 24
		int value = fromBinaryToInt(data);

		if(debug) cout << "I just extracted '" << data << "' (int val: " 
			<< value << ") from my file!" << endl;
		
		if(width == 0) width = value;
		else height = value;
		
	}

	//NOTE FIND SOME ERROR CASES TO CHECK HERE FOR

	//Now width and height are loaded correctly
	//Now we are loading in maxPixelValue
	char c;
	char data[4];
	myFile.read(data, 4);
	int value = fromBinaryToInt(data);

	//keep in mind now maxPixelValue is 4 bytes
	maxPixelValue = value;
	

	if(debug) cout << "maxPixelValue set to " << maxPixelValue << endl;

	//Now we will be looping through the rest of the file
	//We should be seeing one byte (0-255) that represents a pixel
	while(true) {
		myFile.get(c);
		int pixelVal = charToInt(c);
		if(myFile.eof()) {
			break;
		}
		if(pixelVal > maxPixelValue) {
			cerr << "found pixel with value: " << pixelVal
				<< " that is greater than the maxPixelValue: " 
				<< maxPixelValue << endl;
			return -1;
		}
		mainData.push_back(pixelVal);
		//cout << "reading " << pixelVal << " (" << char(pixelVal) << ")" << endl;
	}
	

	return 0;
}


//This function continues to parse ascii image and sets up mainData, width, height, maxPixelValue
int Image::continueParseInAscii(ifstream& myFile) {
	
	if(debug) cout << "This is an Ascii image!" << endl;

	//Here is where we will read in and set the width, then height
	//The width and height
	//  - must be positive
	//  - must be integers
	int tempWidth, tempHeight, tempMaxPixelValue;

	tempWidth = nextValidValue(myFile);
	if(debug) cout << "tempWidth: " << tempWidth << endl;
	tempHeight = nextValidValue(myFile);
	if(debug) cout << "tempHeight: " << tempHeight << endl;
	if(tempHeight > 0 && tempWidth > 0) {
		width = tempWidth;
		height = tempHeight;
	}
	else {
		cerr << "Error - height and width must be a positive integer!, found width: " << tempWidth << ", found height: " << tempHeight;
		return -3;
	}
	
	//Here we are going to look for the maxPixelValue
	//   - an integer between 0 - 256 (inclusive)
	tempMaxPixelValue = nextValidValue(myFile);
	if(debug) cout << "tempMaxPixelValue: " << tempMaxPixelValue << endl;
	if(tempMaxPixelValue >= 0 && tempMaxPixelValue <= 256) {
		maxPixelValue = tempMaxPixelValue;
	}
	else {
		cerr << "Error - MaxPixelValue is invalid, found value: " << tempMaxPixelValue;
		return -4;
	}
	
	//currentLine is going to hold a list of all pixel values
	//Each pixel value must be:
	//   - a non-negative (0 - maxPixVal) (inclusive) integer
	//   - pixel values are seperated by white space
	//   - There must be width * height pixels, more or less is an error!
	vector<int> tempData;
	while (true) {
		// Here val is set to the read in pixel value			
		int val = nextValidValue(myFile);
		//Here I am doing a manual check to see if the next value is eof
		if(myFile.eof()) {
			break;
		}
		// Checking if pixel value is integer and within range
		if(val < 0 || val > maxPixelValue) {
			cerr << "Pixel value at position " 
				<< tempData.size()+1 << " is invalid with value: " 
					<< val << endl;
			return -5;
		}
		else {
			tempData.push_back(val);
			if(debug && 1 > 2) {
				cout << val << "\t";
				if(tempData.size() % width == 0) {
					cout << endl;
				}
			}

		}
 	}

	//All pixels loaded into Image's mainData vector
	mainData = tempData;
	
	return 0;
}

// parseFile will read in the image file and
// 1. Parse through the header and get the height, width, maxValue
// 2. Catch any errors, return -1 if errors otherwise return 0
int Image::parseFile(string fileName) {
	//KEY for failure returns
	//  0 : PASSING
	// -1 : generic / unknown failure
	// -2 : First element is NOT P2!
	// -3 : Height / Width are not Positive Integers!
	// -4 : MaxPixelValue found is not valid!
	// -5 : one of the pixel values found is INVALID!
	// -6 : max == min
	// -7 : file permissions issues, cannot write to file!

	ifstream myFile;	
	myFile.open(fileName.c_str());
	string output = "";	
	if (myFile.is_open()) {
		//First let us check that the header begins with "P2"
		
		//letter will hold P2 values and compare them
		char letter;

		//After losing points in PA1 for not checking that the first two chars
		// are specifically P2, losing points for cases [ P2] and [P2#Comment]
		//Remedied below by reading from file one char at a time
		
		myFile.get(letter);
		output = letter;

		myFile.get(letter);
		output += letter;


		if(output.compare("P2") == 0) {
			if(debug) cout << "first element is P2!" << endl;
			//Here we know it is an ASCII pgm file, continueParse will finish this
			int success = continueParseInAscii(myFile);
			//set the img isBinary bool to false
			isBinary = false;
			if(success < 0) return success;
		}
		else if(output.compare("P5") == 0) {
			if(debug) cout << "first element is P5!" << endl;
			int success = continueParseInBinary(myFile);
			//set the img isBinary bool to true
			isBinary = true;
			if(success < 0) return success;
		}
		else {
			cerr << "Error - first element is NOT P2 or P5, found value: " << output;
			return -2;
		}
		
		//Here let us calculate max, min, mean, and check that pixel number is correct
		
		// Now we check that tempData is holding a vector of size width * height
		unsigned int expectedSize = width * height;
		if(mainData.size() != expectedSize) {
			cerr << "Invalid number of pixels provided, found: "
				<< mainData.size() << ", am looking for "
				 << width*height << " pixels" << endl;
			return -8;
		}

		min = maxPixelValue, max = 0;
		mean = 0;
		for(unsigned int i = 0; i<mainData.size(); i++) {
			if(mainData.at(i) < min) {
				min = mainData.at(i);
			}
			if(mainData.at(i) > max) {
				max = mainData.at(i);
			}
			mean += mainData.at(i);
		}
		mean /= mainData.size();
	
		if(debug) printImage();
	
		//Here we can do final checks for errors
		// max == min is an error
		if(max == min) {
			cerr << "Error! Max = Min" << endl;
			return -6;	
		}

		//Main PRINT statement - commented out for PA2
		if(debug) cout << min << " " << mean << " " << max << endl;
	

	}
	//else if myFile isnt open (something went wrong, could be permissions with file)
	else {
		cerr << "File permissions issues! Cannot open file!" << endl;
		return -7;
	}
	
	myFile.close();
	//goodImage will be true when image is known to be properly formatted and free of errors
	goodImage = true;
	return 0;
}

bool Image::isInteger(const string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}



//This function will return an int representation of what the next value is
//Importantly, skip all values including COMMENTS
int Image::nextValidValue(ifstream& myFile)
{	
	// output is filled using the >> command, s is a string representation of output
	char output[100];

	if(myFile.is_open()) {
		myFile >> output;
	}

	string s(output);
	string num = "";
	// Here I am checking to make sure no # character has been found,
	// the find function returns string::npos if no # char was found
	// If there is a # found, first check to see if there is a number proceeding '#'
	// This could be a valid value such as 32#Comment, which should return 32 pixel value
	// and still parse for the comment
	if(s.find('#') !=  string::npos) {
		//This case gets run if the comment does not begin with a #
		// Could be 32# which is valid		
		
		string n = s.substr(0, s.find('#'));
		if(output[0] != '#') {
			if(debug) cout << "number found before Comment!" << n << endl;
			s = s.substr(s.find('#'));
			// Now we have the number before the comment
			// Now let us skip through the rest of the comment until we hit newline
			
		}		
		//This code is the same whether we see such as 32# or #comment
	
		string fullComment = s;
		getline(myFile, s);
		//This is not a newline so keep going

		fullComment += s;
		
		if(debug) cout << "found full comment: " << fullComment << endl;	
		if(n.size() > 0) {
			return atoi(n.c_str());
		}
		else {
			return nextValidValue(myFile);
		}
		
		//Now we are at the point where the comment has finished
		// Here we will return the number 
		//myFile >> output;
		
	}
	if(isInteger(output) == false) {

		return -7531;
	}
	return atoi(output);
}	

//This function will return a 0 - 255 representation of a char
int Image::charToInt(char c) {
	int val = (int) c;
	if(val < 0) return val + 256;
	return val;

}

void Image::printImage()
{
	for(unsigned int i = 0; i<mainData.size(); i+=1) {
		//Then I will print the darkest value

		if(mainData.at(i) < max * 0.05) {
			cout << "â–ˆâ–ˆâ–ˆ" ;
			
		}
		else if(mainData.at(i) < max * 0.1) {
			cout << "â–ˆâ–“â–ˆ" ;	
		}
		else if(mainData.at(i) < max * 0.15) {
			cout << "â–“â–ˆâ–“" ;	
		}
		else if(mainData.at(i) < max * 0.2) {
			cout << "â–“â–“â–“" ;	
		}
		else if(mainData.at(i) < max * 0.25) {
			cout << "â–“â–’â–“" ;	
		}
		else if(mainData.at(i) < max * 0.3) {
			cout << "â–’â–“â–’" ;	
		}
		else if(mainData.at(i) < max * 0.35) {
			cout << "â–’â–’â–’" ;	
		}
		else if(mainData.at(i) < max * 0.4) {
			cout << "â–’â–‘â–’" ;	
		}
		else if(mainData.at(i) < max * 0.45) {
			cout << "â–‘â–’â–‘" ;	
		}
		else if(mainData.at(i) < max * 0.5) {
			cout << "â–‘â–‘â–‘" ;	
		}
		else if(mainData.at(i) < max * 0.55) {
			cout << "â–‘|â–‘" ;	
		}
		else if(mainData.at(i) < max * 0.6) {
			cout << "|â–‘|" ;	
		}
		else if(mainData.at(i) < max * 0.65) {
			cout << "|||" ;	
		}
		else if(mainData.at(i) < max * 0.7) {
			cout << "|.|" ;	
		}
		else if(mainData.at(i) < max * 0.75) {
			cout << ".|." ;	
		}
		else if(mainData.at(i) < max * 0.775) {
			cout << "..." ;	
		}
		else if(mainData.at(i) < max * 0.85) {
			cout << ". ." ;	
		}
		else if(mainData.at(i) < max * 0.875) {
			cout << " . " ;	
		}
		else{
			cout << "   " ;	
		}

		if((i+1) % width == 0 && i > 0) {
			cout << endl;
		}
	}
	cout << endl;
}
