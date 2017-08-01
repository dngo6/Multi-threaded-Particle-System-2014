#include "object.h"
#include <glut.h>

Object::Object(){
}
	
Object::Object(int x_coord, int y_coord, int w, int h, int lb){
	x = x_coord;
	y = y_coord;
	width = w;
	height = h;
	weight = lb;
	acc = ACCELERATION;
}

int Object::getTemp()
{
	return temperature;
}

int Object::getCoord(char c)
{
	switch (c)
	{
	case 'x':
		return x;
		break;
	case 'y':
		return y;
		break;
	}
}

int Object::getSize(char c)
{
	switch (c)
	{
	case 'w':
		return width;
		break;
	case 'h':
		return height;
		break;
	}
}

void Object::setCoord(int x_coord, int y_coord)
{
	x = x_coord;
	y = y_coord;
}

void Object::setTemp(int t)
{
	temperature = t;
}

void Object::draw()
{
	glColor3f(0.75,0.75,0.75);
	// bottom left corner
	glVertex2f(x, y);
					
	// bottom right corner
	glVertex2f(x+width, y);

	// top right corner
	glVertex2f(x+width, y+height);
					
	// top left corner
	glVertex2f(x, y+height);
}

void Object::update()
{
	//just giving this object some gravity
	if (y+height < 720)
	{
		y += (weight + acc);
		acc += 1;
	}
	else
		acc = ACCELERATION;
	//------------------------------------

	//if (temperature < MAX_TEMP)
		//temperature += 10;

}