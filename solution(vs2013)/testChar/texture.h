/*	
	File:   texture.h 
	PROGRAMMER:  Dan Cliburn  (dcliburn@pacific.edu)
	PURPOSE:  The following is the description of the Texture class.  To use, simply pass the filename
	of a 24-bit per pixel Bitmap image to the constructor or call the loadTexBMP method. Bitmaps 
	should be at least 32 pixels in width and height and the width must be a multiple of four.
	If your image does not meet these requirements, you can easily modify it using MS Paint.  Just 
	follow these steps:
	 1) Open the image in MS Paint
	 2) Select the "Resize" option from the "Home" menu.  Click on the "Pixels" option and with 
		"Maintain aspect ratio" selected, specify the closest multiple of 4 to your image
		for the "Horizontal" value.  Click "OK".
	 3) Now, choose to "Save as" then "BMP picture".  In the box that pops up, select "24-bit Bitmap"
	    from the "Save as type:" drop down list.  Click "Save".

	Acknowledgment #1: The idea for this class is based on the c functions written 
	by Nate Miller on 5/5/1999 for handling texture mapping using TGA images.
  	(see http://www.essi.fr/~buffa/cours/synthese_image/DOCS/Tutoriaux/Nate/index-6.htm)

	Acknowledgment #2: Later, this class was modified to support transparent tectures,
	i.e. portions of an image are invisible.  This is often used when constructing billboards.
	Passing the parameter "addAlpha" to the load method (or constructor) adds an alpha bit of 0 
	to the pixels where the image is black.  Other colors will be given an alpha of 255.  With 
	appropriate blending parmeters set, the black portions of the image can be made invisible.
	The idea for this comes from the text "OpenGL Game Programming", by Kevin Hawkins 
	and Dave Astle, pages 434-438.

	Acknowledgement #3:  In 2010 a bug was resolved that kept some 24-bit Bitmaps from loading correctly.
	At this time the class was also modified to support printing of some of the header information to the 
	screen. Header info is printed if true is passed as the 4th parameter to loadTexBMP or the 
	constructor.  Thank you to those who provided suggestions (such as Michael Casolary) that helped 
	to resolve this issue.
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <fstream>
using namespace std;

enum TexType {addAlpha, rgb};

class Texture
{
	public:

		Texture() {isLOADED = false;}
		Texture(char filename[], int id, TexType t = rgb, bool displayTexData = false);

		int loadTexBMP(char filename[], int id, TexType t = rgb, bool displayTexData = false);

		int isLoaded() {return isLOADED;}

		static void setGLTexParams();
	
	private:
		
		bool isLOADED;

		//Image parameter information
		int imageWidth;
		int imageHeight;
		
		int id;  //Texture id number

		void readHeaderData(ifstream &in, int &offset, int &bitsPerPixel, bool displayTexData);
};

#endif