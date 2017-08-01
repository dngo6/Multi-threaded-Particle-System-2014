#ifndef PARTICLE_H
#define PARTICLE_H

#include "object.h"

class Particle: public Object
{
private:
	int id;
	int timeOut;
	float green;
	bool drop;

public:
	Particle();
	Particle(int x_coord, int y_coord);

	//get 
	int getExist();

	//set
	void setExist(bool b);
	void setDrop(bool b);

	//asthetics
	void draw();
	void update();
};

#endif