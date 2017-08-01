#ifndef OBJECT_H
#define OBJECT_H

#define ACCELERATION 3
#define MAX_TEMP 660 //in F 

class Object
{
protected:
	int x;
	int y;
	int width;
	int height;

	int weight;
	int acc;
	int temperature;
	bool exist;

public:
	Object();
	Object(int x_coord, int y_coord, int w, int h, int lb);

	//recieve
	int getTemp();
	int getCoord(char c);
	int getSize(char c);

	//set
	void setCoord(int x_coord, int y_coord);
	void setTemp(int t);

	//asthetics
	void draw();
	void update();
};

#endif