#include "main.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
using namespace MenuFuncs;


// Define the minimum pixel value for reference (0)
const unsigned char pixelMin = 0;
// Define the maximum pixel value for reference (255)
const unsigned char pixelMax = 255;

// Function to read an image from a binary file located at path.
// The function reads the image header and pixel data, and returns an Image object.
Image MenuFuncs::readDataFile(string path) {
	// Open the file in binary mode using ifstream.
	ifstream inputFile(path, ios_base::binary);
	// Check if the file was successfully opened.
	if (inputFile.is_open()) {

		// Create a Header object to store the image header data.
		Image::Header hD;
		// Read the image header values from the file one by one.
		inputFile.read(reinterpret_cast<char*>(&hD.idLength), sizeof(hD.idLength));
    	inputFile.read(reinterpret_cast<char*>(&hD.colorMapType), sizeof(hD.colorMapType));
    	inputFile.read(reinterpret_cast<char*>(&hD.dataTypeCode), sizeof(hD.dataTypeCode));
    	inputFile.read(reinterpret_cast<char*>(&hD.colorMapOrigin), sizeof(hD.colorMapOrigin));
    	inputFile.read(reinterpret_cast<char*>(&hD.colorMapLength), sizeof(hD.colorMapLength));
    	inputFile.read(reinterpret_cast<char*>(&hD.colorMapDepth), sizeof(hD.colorMapDepth));
    	inputFile.read(reinterpret_cast<char*>(&hD.xOrigin), sizeof(hD.xOrigin));
    	inputFile.read(reinterpret_cast<char*>(&hD.yOrigin), sizeof(hD.yOrigin));
    	inputFile.read(reinterpret_cast<char*>(&hD.width), sizeof(hD.width));
    	inputFile.read(reinterpret_cast<char*>(&hD.height), sizeof(hD.height));
    	inputFile.read(reinterpret_cast<char*>(&hD.bitsPerPixel), sizeof(hD.bitsPerPixel));
    	inputFile.read(reinterpret_cast<char*>(&hD.imageDescriptor), sizeof(hD.imageDescriptor));

		// Create an Image object using the read header data.
		Image image = Image(hD);

		// Loop to read pixel data. The number of pixels equals the product of the image's width and height.
		for (int i = 0; i < (hD.height * hD.width); ++i) {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			// Read the blue, green, and red components in that order (BGR).
			inputFile.read((char*)&b, sizeof(b));
			inputFile.read((char*)&g, sizeof(g));
			inputFile.read((char*)&r, sizeof(r));
			
			Pixel p = Pixel(r, g, b);
			
			image.addPixels(p);
		}

		
		inputFile.close();
		return image;
	}
	// If the file couldn't be opened, print an error message and return an empty Image object.
	cout << "Can't load the input file '" << path << " Please try again!" << endl; 
	return Image();
}


bool MenuFuncs::writeDataFile(string path, Image& image) {
	// Open the file for binary writing at the given path.
	ofstream outputFile(path, ios_base::binary);
	// Check if the file is open successfully.
	if (outputFile.is_open()) {

		// Retrieve the image header from the image object.
		Image::Header hD = image.getTheHeader();
		// Write each component of the image header to the output file.
		// The header contains metadata about the image
		outputFile.write(&hD.idLength, sizeof(hD.idLength));
		outputFile.write(&hD.colorMapType, sizeof(hD.colorMapType));
		outputFile.write(&hD.dataTypeCode, sizeof(hD.dataTypeCode));
		outputFile.write((char*)&hD.colorMapOrigin, sizeof(hD.colorMapOrigin));
		outputFile.write((char*)&hD.colorMapLength, sizeof(hD.colorMapLength));
		outputFile.write((char*)&hD.colorMapDepth, sizeof(hD.colorMapDepth));
		outputFile.write((char*)&hD.xOrigin, sizeof(hD.xOrigin));
		outputFile.write((char*)&hD.yOrigin, sizeof(hD.yOrigin));
		outputFile.write((char*)&hD.width, sizeof(hD.width));
		outputFile.write((char*)&hD.height, sizeof(hD.height));
		outputFile.write(&hD.bitsPerPixel, sizeof(hD.bitsPerPixel));
		outputFile.write(&hD.imageDescriptor, sizeof(hD.imageDescriptor));

		// Write the pixel data to the file.
		// The loop iterates over all the pixels in the image.
		for (int i = 0; i < image.getPixCount(); ++i) {
			// Array to store the RGB color values of the pixel.
			unsigned char rgb[3];
			// Get the pixel at index i
			Pixel p = image.getPix(i);
			// Retrieve the RGB values for the pixel and store them in the RGB array.
			for (int j = 0; j < 3; ++j)
				rgb[j] = p.getRGBColor(j);
			// Write the blue, green, and red color values to the file in that order.
			// This is because many image formats (like TGA) store pixels in BGR order.
			outputFile.write((char*)&rgb[2], sizeof(rgb[2]));
			outputFile.write((char*)&rgb[1], sizeof(rgb[1]));
			outputFile.write((char*)&rgb[0], sizeof(rgb[0]));
		}

		
		outputFile.close();
		return true;
	}

	
	return false;
}

// Compares two images by reading them from file paths 'first' and 'second'.
bool MenuFuncs::compareImage(string first, string second) {
	// Read the two images from their respective files.
	Image Image1 = readDataFile(first);
	Image Image2 = readDataFile(second);
	
	int headErr = 0;
	int pixelErr = 0;
   // Output the names of the images being compared and their pixel counts.
	cout << "Comparing '" << first << "' and '" << second << "'." << endl;
	cout << "Pixels: " << Image1.getPixCount() << " vs. " << Image2.getPixCount() << endl;

	// Compare the headers of both images field by field.
	if (Image1.getTheHeader().idLength != Image2.getTheHeader().idLength)
		headErr++;
	if (Image1.getTheHeader().colorMapType != Image2.getTheHeader().colorMapType)
		headErr++;
	if (Image1.getTheHeader().dataTypeCode != Image2.getTheHeader().dataTypeCode)
		headErr++;
	if (Image1.getTheHeader().colorMapOrigin != Image2.getTheHeader().colorMapOrigin)
		headErr++;
	if (Image1.getTheHeader().colorMapLength != Image2.getTheHeader().colorMapLength)
		headErr++;
	if (Image1.getTheHeader().colorMapDepth != Image2.getTheHeader().colorMapDepth)
		headErr++;
	if (Image1.getTheHeader().xOrigin != Image2.getTheHeader().xOrigin)
		headErr++;
	if (Image1.getTheHeader().yOrigin != Image2.getTheHeader().yOrigin)
		headErr++;
	if (Image1.getTheHeader().width != Image2.getTheHeader().width)
		headErr++;
	if (Image1.getTheHeader().height != Image2.getTheHeader().height)
		headErr++;
	if (Image1.getTheHeader().bitsPerPixel != Image2.getTheHeader().bitsPerPixel)
		headErr++;
	if (Image1.getTheHeader().imageDescriptor != Image2.getTheHeader().imageDescriptor)
		headErr++;

	// Compare the pixels of both images.
	for (int i = 0; i < Image1.getPixCount(); ++i) {
		// Loop through each RGB component of each pixel.
		for (int j = 0; j < 3; ++j) {
			// If the color component is different, increment pixelErr.
			if (Image1.getPix(i).getRGBColor(j) != Image2.getPix(i).getRGBColor(j))
				pixelErr++;
		}
	}
    // If there are no errors in header or pixel data, images are identical.
	if (pixelErr == 0 && headErr == 0) {
		cout << "No difference between the images!" << endl;
		return true;
	}
	else {
		// Output the number of header and pixel errors.
		cout << "The header errors are: " << headErr << " The pixel errors are: " << pixelErr << endl;
		return false;
	}
}

// Function to print the image's header.
void MenuFuncs::printTheImage(Image& image) {
	image.printTheHeader();
}

// Function to normalize a color component 'c' with respect to the top and bottom range.
// This scales 'c' to a value between 0 and 1.
float MenuFuncs::normImage(unsigned char c, unsigned char top, unsigned char bottom) {
	float temp = ((float)c / (float)(top - bottom));
	return temp;
}

// Function to multiply two images pixel by pixel.
Image MenuFuncs::multiplyImage(const Image& top, const Image& bottom) {
	// Create a new image with the same header as 'top'.
	Image image = Image(top.getTheHeader());

	// Loop through each pixel of the top image.
	for (int i = 0; i < top.getPixCount(); ++i) {
		Pixel pixT = top.getPix(i);
		Pixel pixB = bottom.getPix(i);
		float rgb[6];
		unsigned char RGBTemp[3];

		// Normalize the RGB values of the top and bottom pixels.
		for (int j = 0; j < 6; ++j) {
			if (j < 3)
				rgb[j] = normImage(pixT.getRGBColor(j), pixelMax, pixelMin);
			else
				rgb[j] = normImage(pixB.getRGBColor(j - 3), pixelMax, pixelMin);
		}

		// Multiply corresponding RGB values and clamp to the range [0, 255].
		for (int j = 0; j < 3; ++j) {
			if (rgb[j] * rgb[j + 3] > 1)
				RGBTemp[j] = pixelMax;
			else
				RGBTemp[j] = (unsigned char)((rgb[j] * rgb[j + 3]) * 255 + 0.5f);
		}

		// Create a new pixel with the multiplied RGB values and add it to the new image.
		Pixel p = Pixel(RGBTemp[0], RGBTemp[1], RGBTemp[2]);
		image.addPixels(p);
	}

	return image;
}



Image MenuFuncs::subtractImage(const Image& top, const Image& bottom) {
	// Create a new Image object using the header from the 'top' image.
	Image image = Image(top.getTheHeader());

	// Loop through each pixel in the 'top' image. The loop assumes that both images have the same number of pixels.
	for (int i = 0; i < top.getPixCount(); ++i) {
		// Get the current pixel from both the 'top' and 'bottom' images.
		Pixel pixT = top.getPix(i);
		Pixel pixB = bottom.getPix(i);
		// Array to store the RGB values of both 'top' and 'bottom' pixels.
		unsigned char rgb[6];
		unsigned char RGBTemp[3];

		// Populate the 'RGB' array with the RGB values of 'top' and 'bottom' pixels.
		for (int j = 0; j < 6; ++j) {
			if (j < 3)
			// Store RGB values of 'top' pixel in the first three positions (0: red, 1: green, 2: blue).
				rgb[j] = pixT.getRGBColor(j);
			else
			// Store RGB values of 'bottom' pixel in the next three positions (3: red, 4: green, 5: blue).
				rgb[j] = pixB.getRGBColor(j - 3);
		}

		// Perform pixel subtraction. For each color component (R, G, B):
		for (int j = 0; j < 3; ++j) {
			if (rgb[j+3] - rgb[j] < 0)
				RGBTemp[j] = pixelMin;
			else
				RGBTemp[j] = (rgb[j+3] - rgb[j]);
		}

		// Create a new Pixel with the resulting subtracted RGB values and add it to the new image.
		Pixel p = Pixel(RGBTemp[0], RGBTemp[1], RGBTemp[2]);
		image.addPixels(p);
	}

	return image;
}

Image MenuFuncs::screenImage(const Image& top, const Image& bottom, const Image& screen) {
	// First, create a temporary image by multiplying the top and bottom images pixel by pixel.
	Image temp = multiplyImage(top, bottom);
	// Create a new Image object 'image' using the header information from the temporary image.
	Image image = Image(temp.getTheHeader());

	// Loop through all the pixels in the screen image.
	for (int i = 0; i < screen.getPixCount(); ++i) {
		// Get the corresponding pixels from the screen and the temporary multiplied image.
		Pixel pixT = screen.getPix(i);
		Pixel pixB = temp.getPix(i);
		// Arrays to store normalized RGB values from the screen and multiplied images.
		float rgb[6];
		unsigned char RGBTemp[3];
		// Normalize the RGB values from both the screen and multiplied image pixels.
		// Normalization scales the color values to a range between 0 and 1 for blending calculations.
		for (int j = 0; j < 6; ++j) {
			if (j < 3)
			// Normalize the color component from the screen pixel.
				rgb[j] = normImage(pixT.getRGBColor(j), pixelMax, pixelMin);
			else
			// Normalize the color component from the multiplied image pixel.
				rgb[j] = normImage(pixB.getRGBColor(j - 3), pixelMax, pixelMin);
		}

		// Apply the screen blend formula for each color component (R, G, B).
		for (int j = 0; j < 3; ++j) {
			float a = 1 - (1 - rgb[j]) * (1 - rgb[j + 3]);
			if (a > 1)
				a = 1;
			else if (a < 0)
				a = 0;
			// Convert the result back to the 0-255 range and store it in RGBTemp.
			RGBTemp[j] = (unsigned char)(a * 255 + 0.5f);
		}

		// Create a new pixel with the blended RGB values.
		Pixel p = Pixel(RGBTemp[0], RGBTemp[1], RGBTemp[2]);
		// Add the new pixel to the final image.
		image.addPixels(p);
	}

	return image;
}

Image MenuFuncs::overlayImage(const Image& top, const Image& bottom) {
	// Create a new Image object with the same header as the 'top' image.
	Image image = Image(top.getTheHeader());

	// Loop through each pixel in the 'top' image.
	for (int i = 0; i < top.getPixCount(); ++i) {
		Pixel pixT = top.getPix(i);
		Pixel pixB = bottom.getPix(i);
		float rgb[6];
		unsigned char RGBTemp[3];

		// Normalize the RGB values of both the 'top' and 'bottom' pixels.
		// First 3 values (RGB[0], RGB[1], RGB[2]) are for the 'top' pixel.
		// Last 3 values (RGB[3], RGB[4], RGB[5]) are for the 'bottom' pixel.
		for (int j = 0; j < 6; ++j) {
			if (j < 3)
				rgb[j] = normImage(pixT.getRGBColor(j), pixelMax, pixelMin);
			else
				rgb[j] = normImage(pixB.getRGBColor(j - 3), pixelMax, pixelMin);
		}

		// Apply the overlay operation for each color component (R, G, B).
		for (int j = 0; j < 3; ++j) {
			if (rgb[j + 3] <= 0.5) {
				float a = 2 * rgb[j] * rgb[j + 3];
				if (a > 1)
					a = 1;
				RGBTemp[j] = (unsigned char)(a * 255 + 0.5f);
			}
			// If the bottom pixel's normalized value is greater than 0.5, use this formula.
			else {
				float a = 1 - 2 * (1 - rgb[j]) * (1 - rgb[j + 3]);
				if (a > 1)
					a = 1;
				else if (a < 0)
					a = 0;
				RGBTemp[j] = (unsigned char)(a * 255 + 0.5f);
			}
		}

		// Create a new Pixel object using the computed RGB values.
		Pixel p = Pixel(RGBTemp[0], RGBTemp[1], RGBTemp[2]);
		// Add the new pixel to the resulting image.
		image.addPixels(p);
	}

	return image;
}

Image MenuFuncs::addRGBImage(const Image& top, unsigned char red, unsigned char green, unsigned char blue) {
	// Create a new Image object initialized with the header of the "top" image.
	Image image = Image(top.getTheHeader());

	// Iterate through all the pixels in the "top" image.
	for (int i = 0; i < top.getPixCount(); ++i) {
		Pixel pixT = top.getPix(i);
		// Declare an array to store normalized RGB values of the current pixel.
		float rgb[3];
		unsigned char RGBTemp[3];

		// Normalize the red, green, and blue values of the current pixel.
		// The normImage function normalizes the pixel values based on a min-max range (0 to 1).
		for (int j = 0; j < 3; ++j)
			rgb[j] = normImage(pixT.getRGBColor(j), pixelMax, pixelMin);
		
			// Add the normalized "red" parameter to the pixel's normalized red value.
		// Ensure the value does not exceed 1 (the maximum for normalized RGB).
			float a = rgb[0] + normImage(red, pixelMax, pixelMin);
			if (a > 1)
				a = 1;
			// Convert the normalized red value back to an unsigned char and store it in RGBTemp.
			RGBTemp[0] = (unsigned char)(a * 255 + 0.5f);
			// Repeat the same process for the green channel.
			a = rgb[1] + normImage(green, pixelMax, pixelMin);
			if (a > 1)
				a = 1;
			RGBTemp[1] = (unsigned char)(a * 255 + 0.5f);
			// Repeat the same process for the blue channel.
			a = rgb[2] + normImage(blue, pixelMax, pixelMin);
			if (a > 1)
				a = 1;
			RGBTemp[2] = (unsigned char)(a * 255 + 0.5f);

		// Create a new pixel with the adjusted RGB values and add it to the new image.
		Pixel p = Pixel(RGBTemp[0], RGBTemp[1], RGBTemp[2]);
		image.addPixels(p);
	}

	return image;
}

Image MenuFuncs::scaleImage(const Image& top, bool red, float x, bool green, float y, bool blue, float z) {
	// Create a new Image object with the same header as the input image (top).
	Image image = Image(top.getTheHeader());

	// Loop through each pixel in the original image.
	for (int i = 0; i < top.getPixCount(); ++i) {
		Pixel pixT = top.getPix(i);
		unsigned char rgb[3];
		unsigned char RGBTemp[3];

		// Retrieve the RGB values of the current pixel and store them in the RGB array.
		for (int j = 0; j < 3; ++j)
			rgb[j] = pixT.getRGBColor(j);

		// Scale the red component if the 'red' flag is true.
		if (red == 1) {
			if (rgb[0] * x > pixelMax)
				RGBTemp[0] = pixelMax;
			else
				RGBTemp[0] = (unsigned char)(rgb[0] * x);
		}
		else
		// If not scaling red, keep the original red value.
			RGBTemp[0] = rgb[0];
		// Scale the green component if the 'green' flag is true.
		if (green == 1) {
			if (rgb[1] * y > pixelMax)
				RGBTemp[1] = pixelMax;
			else
				RGBTemp[1] = (unsigned char)(rgb[1] * y);
		}
		else
		// If not scaling green, keep the original green value.
			RGBTemp[1] = rgb[1];
			// Scale the blue component if the 'blue' flag is true.
		if (blue == 1) {
			if (rgb[2] * z > pixelMax)
				RGBTemp[2] = pixelMax;
			else
				RGBTemp[2] = (unsigned char)(rgb[2] * z);
		}
		else
		// If not scaling blue, keep the original blue value.
			RGBTemp[2] = rgb[2];

		// Create a new Pixel object with the scaled RGB values and add it to the new image.
		Pixel p = Pixel(RGBTemp[0], RGBTemp[1], RGBTemp[2]);
		image.addPixels(p);
	}

	return image;
}

vector<Image> MenuFuncs::separateRGB(const Image& top) {
	// Create a vector to hold three grayscale images (for red, green, and blue channels).
	vector<Image> temp;
	// Initialize three new Image objects (for Red, Green, and Blue channels) using the header from the 'top' image.
	// This ensures that the separated images will have the same dimensions and metadata as the original image.
	temp.push_back(Image(top.getTheHeader())); 
	temp.push_back(Image(top.getTheHeader())); 
	temp.push_back(Image(top.getTheHeader())); 
	
	// Loop through each pixel in the 'top' image.
	for (int i = 0; i < top.getPixCount(); ++i) {
		// Get the current pixel from the original image.
		Pixel pixT = top.getPix(i);
		// Extract the red, green, and blue color values from the pixel.
		unsigned char red = pixT.getRGBColor(0);
		unsigned char green = pixT.getRGBColor(1);
		unsigned char blue = pixT.getRGBColor(2);
		// Create new grayscale pixel objects where each channel contains only the respective red, green, or blue component.
		// Each grayscale image will represent one color channel in shades of gray.
		Pixel pixRed = Pixel(red, red, red);
		Pixel pixGreen = Pixel(green, green, green);
		Pixel pixBlue = Pixel(blue, blue, blue);

		// Add the grayscale red, green, and blue pixels to their respective images.
		temp.at(0).addPixels(pixRed);
		temp.at(1).addPixels(pixGreen);
		temp.at(2).addPixels(pixBlue);
	}

	return temp;
}

Image MenuFuncs::combineImage(const Image& red, const Image& green, const Image& blue) {
	// Create a new Image object using the header from the 'red' image.
	// Assumes all images ('red', 'green', 'blue') have the same header/dimensions.
	Image image = Image(red.getTheHeader());
	// Loop through each pixel in the red, green, and blue images.
	for (int i = 0; i < red.getPixCount(); ++i) {
		unsigned char rgb[3];
		
		rgb[0] = red.getPix(i).getRGBColor(0);
		rgb[1] = green.getPix(i).getRGBColor(0);
		rgb[2] = blue.getPix(i).getRGBColor(0);

		// Create a new Pixel object using the combined RGB values.
		Pixel p = Pixel(rgb[0], rgb[1], rgb[2]);
		// Add the newly created Pixel to the image.
		image.addPixels(p);
	}

	return image;
}


Image MenuFuncs::rotateImage(const Image& top) {
	// Create a new Image object initialized with the header of the 'top' image
	Image image = Image(top.getTheHeader());

	// Initialize a counter to go through each pixel in reverse order (starting from the last pixel)
	int i = (top.getTheHeight() * top.getTheWidth()) - 1;
	// Loop through each pixel in reverse, adding it to the new image
	for (i; i > -1; --i) {
		Pixel p = top.getPix(i);
		image.addPixels(p);
	}

	return image;
}

// Pixel class constructor to initialize RGB values
Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b) {
	red = r;
	green = g;
	blue = b;
}
// Function to get the RGB color component based on an index 'x'
// 0 for red, 1 for green, 2 for blue
const unsigned char Pixel::getRGBColor(int x) const {
	if (x == 0)
		return red;
	if (x == 1)
		return green;
	if (x == 2)
		return blue;
	return 0;
}

// Constructor for Image class that takes a Header object and initializes the image's data
Image::Image(Header x) {
	head = x;
	width = x.width;
	height = x.height;
}
// Default constructor for the Image class
Image::Image() {
	// Initialize image header with default values
	head.idLength = 0;
	width = 0;
	height = 0;
}
// Function to print the image header information
void Image::printTheHeader() {
	// Print all header fields
	cout << head.idLength << " ";
	cout << head.colorMapType << " ";
	cout << head.dataTypeCode << " ";
	cout << head.colorMapOrigin << " ";
	cout << head.colorMapLength << " ";
	cout << head.colorMapDepth << " ";
	cout << head.xOrigin << " ";
	cout << head.yOrigin << " ";
	cout << head.width << " ";
	cout << head.height << " ";
	cout << head.bitsPerPixel << " ";
	cout << head.imageDescriptor << " ";
	cout << "Number of Pixels: " << getPixCount() << endl;
}
// Function to get the image header (const version to ensure it can't modify class state)
const Image::Header Image::getTheHeader() const {
	return head;
}
// Function to get the image header (non-const version)
Image::Header Image::getTheHeader() {
	return head;
}
// Function to add a pixel to the image
void Image::addPixels(Pixel& p) {
	pixels.push_back(p);
}
// Function to get the number of pixels in the image (const version)
const int Image::getPixCount() const {
	return pixels.size();
}
// Function to get the number of pixels in the image (non-const version)
int Image::getPixCount() {
	return pixels.size();
}
// Function to get a specific pixel at index 'x' (const version)
const Pixel Image::getPix(int x) const {
	return pixels.at(x);
}
// Function to get a specific pixel at index 'x' (non-const version)
Pixel Image::getPix(int x) {
	return pixels.at(x);
}
// Function to get the height of the image (const version)
const short Image::getTheHeight() const {
	return height;
}
// Function to get the width of the image (const version)
const short Image::getTheWidth() const {
	return width;
}



int main()
{
	cout << "Begin image processing: " << endl;
	
	// Task 1: Multiply two images (layer1 and pattern1) and write the result to part1.tga
	Image top1 = readDataFile("input/layer1.tga");
	Image bottom1 = readDataFile("input/pattern1.tga");
	Image testingtask1 = multiplyImage(top1, bottom1);
	writeDataFile("output/part1.tga", testingtask1);
	
	// Task 2: Subtract the car image from layer2 and write the result to part2.tga
	Image top2 = readDataFile("input/layer2.tga");
	Image bottom2 = readDataFile("input/car.tga");
	Image testingtask2 = subtractImage(top2, bottom2);
	writeDataFile("output/part2.tga", testingtask2);
			
	// Task 3: Screen blend layer1, pattern2, and a text image, then write the result to part3.tga
	Image top3 = readDataFile("input/layer1.tga");
	Image bottom3 = readDataFile("input/pattern2.tga");
	Image screen = readDataFile("input/text.tga");
	Image testingtask3 = screenImage(top3, bottom3, screen);
	writeDataFile("output/part3.tga", testingtask3);
			
	// Task 4: Multiply layer2 and circles, then subtract the result from pattern2, and write to part4.tga
	Image top4 = readDataFile("input/layer2.tga");
	Image bottom4 = readDataFile("input/circles.tga");
	Image extra = readDataFile("input/pattern2.tga");
	Image testingtask4Temp = multiplyImage(top4, bottom4);
	Image testingtask4 = subtractImage(extra, testingtask4Temp);
	writeDataFile("output/part4.tga", testingtask4);
			
	// Task 5: Overlay layer1 on pattern1 and write the result to part5.tga
	Image top5 = readDataFile("input/layer1.tga");
	Image bottom5 = readDataFile("input/pattern1.tga");
	Image testingtask5 = overlayImage(top5, bottom5);
	writeDataFile("output/part5.tga", testingtask5);
			
	// Task 6: Add green color (increase the green value by 200) to the car image and save it to part6.tga
	Image top6 = readDataFile("input/car.tga");
	Image testingtask6 = addRGBImage(top6, 0, 200, 0);
	writeDataFile("output/part6.tga", testingtask6);

	// Task 7: Scale the red component of the car image by 4.0 and save the result to part7.tga
	Image top7 = readDataFile("input/car.tga");
	Image testingtask7 = scaleImage(top7, true, 4.0, false, 0.0, true, 0.0);
	writeDataFile("output/part7.tga", testingtask7);
			
	// Task 8: Separate the RGB channels of the car image and save each to part8_r.tga, part8_g.tga, and part8_b.tga
	Image top8 = readDataFile("input/car.tga");
	vector<Image> testingtask8 = separateRGB(top8);
	writeDataFile("output/part8_r.tga", testingtask8.at(0));			
	writeDataFile("output/part8_g.tga", testingtask8.at(1));				
	writeDataFile("output/part8_b.tga", testingtask8.at(2));
			
	// Task 9: Combine three separate RGB channel images into one and save the result to part9.tga
	Image top9red = readDataFile("input/layer_red.tga");
	Image top9green = readDataFile("input/layer_green.tga");
	Image top9blue = readDataFile("input/layer_blue.tga");
	Image testingtask9 = combineImage(top9red, top9green, top9blue);
	writeDataFile("output/part9.tga", testingtask9);

	// Task 10: Rotate the text2 image 90 degrees and save the result to part10.tga
	Image top10 = readDataFile("input/text2.tga");
	Image testingtask10 = rotateImage(top10);
	writeDataFile("output/part10.tga", testingtask10);
	
	// Notify the user that the image processing is complete and files have been written to the output folder.
	cout << "Please check tga files in the output folder and make sure everything works." << endl;

	return 0;
}