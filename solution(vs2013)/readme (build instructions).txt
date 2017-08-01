The source codes are located in the testChar folder.
However if you want them all in one place, then we created the Source Code folder just for you. :)

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

Within the main.cpp, you can change the number of threads and particles you wish to use.
Just look for:
#define THREADS
#define PARTICLES

Make sure to just debug it, because there has been problems with creating a release executable.

This is all assuming that you are using Visual Studio 2010.

/*********************************************
Controls:
W - Increase Temperature
S - Decrease Temperature
T - Toggle Visibility of Threads
	Current thread displayed will be indicated on left of the screen.
	Value 00 indicates all threads.

D - Press once for funsies. Hold to drop the bass.

Left Click - Drop the box object from the coordinates of the mouse.

**********************************************/