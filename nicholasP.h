#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"

//typedef float Vec[3];

//struct Shape {
//    float width, height;
//    float radius;
//    Vec center;
//};

//class Platform
//{
//    public:
//	void draw();
//};

void drawNicholasMenu(int xres, int yres, Rect r);
