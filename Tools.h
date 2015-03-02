#ifndef TOOLS_H_INCLUDE
#define TOOLS_H_INCLUDE

/*! \file Complex.h: Declares the Complex number class 
 */

#include <Image.h>

using std::ofstream;

/*! \brief A numeric class for complex numbers with arithmetic operations.
 *
 * A class for instances of complex numbers, i.e. numbers with both real and
 * imaginary components. The class not only provides a representation for
 * complex numbers, it also defines simple arithmetic operations (+, -, *)
 * and I/O operations (<< and >>).
 */
class Tools {
public:

	Tools();
	int writeImageToFile(Image img, string filename);
	Image scaleImage(Image img);
	int doubleToInt(double val);
	bool compareImage(Image img, Image img2);
	bool debug;
private:
	int scalePixel(int val, int min, int max, int scaleTo);
	string fromIntToCharArray(int val);

};


#endif
