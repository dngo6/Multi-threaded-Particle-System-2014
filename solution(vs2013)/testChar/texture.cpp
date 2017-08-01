/*	
	File:   texture.cpp 
	PROGRAMMER:  Dan Cliburn  (dcliburn@pacific.edu)
	PURPOSE:	Defines the methods for the texture class.
*/

#include "texture.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <gl/GL.h>
#include <gl/GLU.h>
using namespace std;

Texture::Texture(char filename[], int id, TexType t, bool displayTexData)
{
	if (!(loadTexBMP(filename, id, t, displayTexData)))
		isLOADED = false;
}

void Texture::readHeaderData(ifstream &in, int &offset, int &bitsPerPixel, bool displayTexData)
{
	char bitData[4];

	in.seekg(10);  //move to the beginning of the header data we want to read
	in.read(bitData, 4);  //read in offset of pixel array
	offset = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;
	if (displayTexData) cout << " offset for pixel data " << offset << endl;
	  
	in.read(bitData, 4);  //read in number of bytes in header from here
	int remaining = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;  
	if (displayTexData) cout << " number of bytes remaining in header " << remaining << endl;
	
	in.read(bitData, 4);  //read in width
	imageWidth = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256 + (unsigned char)bitData[2] * 65536;
	if (displayTexData) cout << " width " << imageWidth << endl;
	
	in.read(bitData, 4);  //read in height
	imageHeight = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256 + (unsigned char)bitData[2] * 65536; 
	if (displayTexData) cout << " height " << imageHeight << endl;
	
	in.read(bitData, 2);  //read in number of color planes
	int colorPlanes = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;
	if (displayTexData) cout << " color planes " << colorPlanes << endl;

	in.read(bitData, 2);  //read in bits per pixel
	bitsPerPixel = (unsigned char)bitData[0] * 1 + (unsigned char)bitData[1] * 256;  
	if (displayTexData) cout << " bitsPerPixel " << bitsPerPixel << endl;

	//There is more header data but we aren't bothering to read it.  We have what we need:
	// offset to the pixel data, bitsPerPixel, image width, and image height
}

int Texture::loadTexBMP(char fileName[], int id, TexType t, bool displayTexData)
{
	unsigned char temp;
	int offset, bitsPerPixel;

	ifstream in(fileName, ios::binary);

	if (!in)
	{
		cout << "Could not load texture!";
		return 0;
	}
	
	if (displayTexData) cout << "Data for " << fileName << endl;
	readHeaderData(in, offset, bitsPerPixel, displayTexData);

	//Make sure image is of a supported type
	if (bitsPerPixel != 24 || imageHeight < 32 || imageWidth < 32 || imageWidth %4 != 0)
	{																
		in.close();
		cout << fileName << " error. Bitmaps must be 24 bits per pixel,\n"
				"at least 32 pixels for row and height,\n"
				"and width must be a multiple of 4." << endl;
		return 0;
	}
	isLOADED = true;
	
	long size = imageWidth * imageHeight;
	char *imageData = new char[size*3]; //3 bytes per pixel
	in.seekg(offset, ios::beg);  //move past the header data in the file
	in.read(imageData, size*3);
	in.close();

	//Reds and blues are transposed, swap them
	for (int i = 0; i < size; i++)
	{
		temp = imageData[i*3];
		imageData[i*3] = imageData[i*3 + 2];
		imageData[i*3 + 2] = temp;
	}

	glBindTexture (GL_TEXTURE_2D, id);
	if (t == addAlpha)  //Add an alpha value - used with billboards
	{
		char *imageDataAlpha;
		imageDataAlpha = new char[size*(3 + 1)];

		// Loop through the image data.  If the color is black, set alpha to zero.
		// Otherwise, set alpha to 1.
		int newIndex = 0;
		for (int i = 0; i < size*3; i +=3)
		{
			// Copy the pixel data into the new array
			imageDataAlpha[newIndex] = imageData[i];
			imageDataAlpha[newIndex+1] = imageData[i+1];
			imageDataAlpha[newIndex+2] = imageData[i+2];

			// If the pixel is black, set the alpha to 0. Otherwise, set it to 255.
			// We know pixel is black if RGB are all 0.
			if (imageData[i] == 0 && imageData[i+1] == 0 && imageData[i+2] == 0)
			  imageDataAlpha[newIndex+3] = 0;
			else
			  imageDataAlpha[newIndex+3] = 0xFF;  //This is binary 11111111, or 255
												  // the highest alpha possible
			newIndex += 4;
		}

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, imageWidth, imageHeight, 
				GL_RGBA, GL_UNSIGNED_BYTE, imageDataAlpha);

		delete [] imageDataAlpha;
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, 
				GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}

	setGLTexParams();

	// delete image data array since it has already been loaded into OpenGL
	delete [] imageData;

	return 1;
}

void Texture::setGLTexParams()
{
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}