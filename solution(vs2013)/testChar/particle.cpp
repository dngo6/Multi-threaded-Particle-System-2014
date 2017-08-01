#include <glut.h>
#include "particle.h"

Particle::Particle()
{
	x = 30;
	y = 30;
	width = 15;
	height = 15;
	timeOut = 30; //seconds
	exist = false;
	green = 1;
	drop = false;
	weight = 2;
	acc = ACCELERATION;
}

Particle::Particle(int x_coord, int y_coord)
{
	x = x_coord;
	y = y_coord;
}

int Particle::getExist()
{
	return exist;
}

void Particle::setExist(bool b)
{
	exist = b;
}

void Particle::setDrop(bool b)
{
	drop = b;
}

void Particle::draw()
{
	if(exist)
	{
		glColor3f(1,green,0);

		// bottom left corner
		glVertex2f(x, y);
					
		// bottom right corner
		glVertex2f(x+width, y);

		// top right corner
		glVertex2f(x+width, y+height);
					
		// top left corner
		glVertex2f(x, y+height);
	}
}

void Particle::update()
{
	if (timeOut > 0 && exist && !drop)
	{
		y -= 8; //move up the window by 8 pixels each 1000/MAX_FPS of a second

		//give the particle the little swirly path thingy
		if (timeOut%2 == 1)
			x += 2; //to the right 2 pixels
		else
			x -= 2; //to the left to the left~

		//shrink in width and height
		width -= .5;
		height -= .5;

		//count down
		timeOut -= 1;

		/*
		now this is REALLY cool!
		We had the RGB channels to be (1, green, 0) so that when green starts at 1 (1, 1, 0)...
		the color of the particle is yellow. However each time this update function loops, the particle...
		gradually turns red!
		*/
		green -= 0.1;
	}

	if (drop)
	{
		if (y+height < 720)
		{
			y += (weight + acc);
			acc += 1;
		}
		else
			acc = ACCELERATION;
	}

	if (timeOut <= 0)
	{
		timeOut = 15;
		width = 15;
		height = 15;
		green = 1;
		exist = false;
	}
}