#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


// The Pixel class represents a single pixel with RGB color components.
class Pixel {
public:
// Constructor initializes the pixel with the given red, green, and blue values.
	Pixel(unsigned char r, unsigned char g, unsigned char b);
	// Method to get the specific color component (R, G, or B) based on an index 'x'.
	// x = 0 returns red, x = 1 returns green, x = 2 returns blue.
	const unsigned char getRGBColor(int x) const;
private:
// Stores the red, green, and blue values of the pixel.
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

// The Image class represents a collection of pixels and stores header information for image metadata.
class Image {
public:
    // Structure that defines the header of the image file. This holds metadata about the image.
	struct Header {
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;
	};
	// Default constructor initializes an empty Image object.
	Image();
	// Constructor initializes the Image with a specific header.
	Image(Header x);

	// Prints the image header information.
	void printTheHeader();
	// Accessor methods to get the image's header and individual pixel.
	const Header getTheHeader() const;
	Header getTheHeader();
	const Pixel getPix(int x) const;
	Pixel getPix(int x);
	// Methods to get the number of pixels and dimensions of the image.
	const int getPixCount() const;
	int getPixCount();
	const short getTheHeight() const;
	const short getTheWidth() const;

	// Adds a pixel to the image.
	void addPixels(Pixel& p);
	
private:
// Stores the header of the image.
	Header head;
	// Stores the width and height of the image (from the header).
	short width;
	short height;
	// Vector to hold all the pixels in the image.
	vector<Pixel> pixels;
};

// The MenuFuncs namespace provides a set of algorithm functions to perform various operations on images.
namespace MenuFuncs {
	// Reads an image from a file at the specified path.
	Image readDataFile(string path);
	// Writes an image to a file at the specified path.
	bool writeDataFile(string path, Image& image);
	// Compares two images by reading them from their file paths.
	bool compareImage(string first, string second);
	// Prints out the image's header and pixel information.
	void printTheImage(Image& image);
	// Normalizes the color component 'c' of an image based on a given range (top and bottom).
	float normImage(unsigned char c, unsigned char top, unsigned char bottom);
	// Multiplies two images pixel by pixel.
	Image multiplyImage(const Image& top, const Image& bottom);
	// Subtracts one image from another pixel by pixel.
	Image subtractImage(const Image& top, const Image& bottom);
	// Screens (blends) two images together using a third image for the screen effect.
	Image screenImage(const Image& top, const Image& bottom, const Image& screen);
	// Applies an overlay blend between two images.
	Image overlayImage(const Image& top, const Image& bottom);
	// Adds specific RGB values to the entire image.
	Image addRGBImage(const Image& top, unsigned char red, unsigned char green, unsigned char blue);
	// Scales the image by modifying individual color components (R, G, or B) by a given factor.
	Image scaleImage(const Image& top, bool red, float x, bool green, float y, bool blue, float z);
	// Separates the image into three grayscale images corresponding to the red, green, and blue channels.
	vector<Image> separateRGB(const Image& top);
	// Combines three grayscale images into one color image by using the separate R, G, and B channels.
	Image combineImage(const Image& red, const Image& green, const Image& blue);
	// Rotates the image by 90 degrees.
	Image rotateImage(const Image& top);
}