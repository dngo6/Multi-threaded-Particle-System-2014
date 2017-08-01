/*********************************************
Fire Particle Simulator
David - Framework and Engine
Shandip - Parallelization
Daniel and Joseph - Physics Designers

Controls:
W - Increase Temperature
S - Decrease Temperature
T - Toggle Visibility of Threads
	Current thread displayed will be indicated on left of the screen.
	Value 00 indicates all threads.

D - Press once for funsies. Hold to drop the bass.

Left Click - Drop the box object from the coordinates of the mouse.

Just a warning that before you compile the program through the compiler, you have to change the directories of the dependancies.
That is because the directories say assume you are using my computer.
All of the folders you need are located within the Release folder.

Includes:
Parallel Code\Release\glut-3.7.6-bin
Parallel Code\Release\SDL-1.2.15\include
Parallel Code\Release\pthreads\Pre-built.2\include

Libraries:
Parallel Code\Release\glut-3.7.6-bin
Parallel Code\Release\SDL-1.2.15\lib\x86
Parallel Code\Release\pthreads\Pre-built.2\x86

**********************************************/
#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <glut.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "particle.h"
#include "object.h"

//define constants
#define THREADS 8 //change to however many you like
#define PARTICLES 2048 //change to however many you like
#define MAX_FPS 60

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

using std::cout;
using std::endl;

//Function Prototypes
void keyboard(unsigned char key, int x, int y);
void mouse(int click, int state, int x, int y);
void render();
void mainLoop(int val);
void* thread_func(void* arg);
void printw (float x, float y, char* format, ...);
void calculateFPS();

//Initiate Global Functions/Variables
Object source(100,100,800,100, 3); //draw the heatsource as a box
Particle spark[PARTICLES]; 

int frame;
int display = 0;
float currentTime;
float previousTime;
float fps = 0;
bool game_end = false;
bool drawing = false;
bool drop = false;

//Set the Barrier
bool ready[THREADS];
//-------------

float num_particles = 0;

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
pthread_t* thread_handles;
pthread_mutex_t mutex;

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		source.setTemp(source.getTemp()+50);
		break;
	case 's':
		source.setTemp(source.getTemp()-50);
		break;
	case 'd':
		if(drop)
			drop = false;
		else
			drop = true;

		for (int i = 0; i < PARTICLES; i++)
			spark[i].setDrop(drop);
		break;
	case 't':
		if (display >= THREADS)
			display = 0;
		else
			display++;
		break;
	default:
		return;
	}
}

void mouse(int click, int state, int x, int y)
{
	switch (click)
	{
	case GLUT_RIGHT_BUTTON:
		exit(0);
		break;
	case GLUT_LEFT_BUTTON:
		source.setCoord(x,y);
		break;
	default:
		break;
	}
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WIN_WIDTH, WIN_HEIGHT, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int start = 0;
	int end = PARTICLES;

	if (display > 0)
	{
		start = display*(PARTICLES/THREADS) - (PARTICLES/THREADS);
		end = start+(PARTICLES/THREADS);
	}

	//draw Heat Source
	glPushMatrix();
		glBegin(GL_QUADS);

		source.draw();

		glEnd();
	glPopMatrix();

	drawing = true;
	//draw particle
	for (int i = start; i < end; i++)
	{
		glPushMatrix();

			glBegin(GL_QUADS);

			spark[i].draw();

			glEnd();
		glPopMatrix();
	}

	calculateFPS();
	//  Load the identity matrix so that FPS string being drawn
    //  won't get animates
	glLoadIdentity ();

	glColor3f(1,1,1);
	//  Print the FPS to the window
	printw(0, 20, "FPS: %4.2f", fps);
	printw(0, 40, "Particles: %4.2f", num_particles);
	printw(0, 60, "Source Temp: %4.2d", source.getTemp());
	printw(0, 80, "Thread: %4.2d", display);

	drawing = false;

	for(int i=0;i<THREADS;i++) //signal all the threads that they can start updating.
	{
		ready[i]=true;
	}

	glutSwapBuffers();
}

void mainLoop(int val)
{
	source.update();

	//Count the number of particles.
	for (int i = 0; i < PARTICLES; i++)
	{
		if (spark[i].getExist())
		{
				num_particles++;
		}
	}

	render();

	num_particles = 0; //reset the counter 

	glutTimerFunc(1000/MAX_FPS, mainLoop, val);
}

void* thread_func(void* arg)
{
	int rank = (int) arg;
	int start = (PARTICLES/THREADS)*rank; //start index for this thread
	int end = start+(PARTICLES/THREADS); // end index for this thread

	while(!game_end) //Keep going until the game ends.
	{
		if(ready[rank]) //Wait for the render function to be ready for new frame to be drawn
		{
			if (source.getTemp() > 300 && !drop)
			{
				for (int i = start; i < end; i++) // from the first index to the last
				{
					if (!spark[i].getExist()) //if the particle of i index is not being used yet.
					{
						if (rand()%3 == 2) //just to add to the randomness
						{
							//Create the particle
							spark[i].setCoord((rand()*(rank+1))%800+(source.getCoord('x')-5), (rand()*(rank+1))%50 + source.getCoord('y'));
							spark[i].setExist(true);
						}
					}
				}
			}

			for (int i = start; i < end; i++)
			{
				spark[i].update(); // update each particle
			}
			ready[rank]=false; //set barrier to false to wait for the render functions go

		} //if ready end
	}
	return NULL;
}

void calculateFPS()
{
	//  Increase frame count
    frame++;

    //  Get the number of milliseconds since glutInit called 
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frame / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frame = 0;
    }
}

void printw (float x, float y, char* format, ...)
{
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1; 

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list 
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos2i (x, y);

	//  Draw the characters one by one
    for (i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	thread_handles = new pthread_t[THREADS];

	pthread_mutex_init(&mutex, NULL);

	glutInit(&argc, argv);
	glutInitWindowSize(WIN_WIDTH,WIN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	for(int i=0;i<THREADS;i++)
	{
		ready[i]=true;
	}

	glEnable(GL_TEXTURE_2D);

	glutCreateWindow("Flame Particle Simulator");

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	/*********************
	Threads will continuously run throughout the whole runtime of the program to 
	reduce overhead time of each game loop.
	**********************/
	for(int i=0;i<THREADS;i++)
	{
		ready[i]=true; //initiate the "ready" marker for each thread.
		pthread_create(&(thread_handles[i]), NULL, thread_func, (void*)i); //create thread to start calculating the frames to be rendered
	}

	glutDisplayFunc(render); //start the render function
	glutTimerFunc(1000/MAX_FPS, mainLoop, 0); //timer function loops the main loop the MAX_FPS times per second or 1000/MAX_FPS
	glutMainLoop();

	//Bring it aaaaaaalllll together.
	for (int i = 0; i < THREADS; i++)
		pthread_join(thread_handles[i], NULL);

	/* delete the local dynamically allocated arrays */
    if (thread_handles != NULL) 
		delete [] thread_handles;

	return 0;
}