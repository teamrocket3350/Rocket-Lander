// Abraham Aldana
// teamrocket3350
// Rocket-Lander
// abrahamA.cpp

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<ctime>
#include<cmath>
#include<X11/Xlib.h>
#include<X11/keysym.h>
#include<GL/glx.h>
#include"log.h"
#include"fonts.h"
#include"abrahamA.h"


void menuAbraham(int xres, int yres, Rect r)
{
	glColor3f(1.0,1.0,1.0);
	int cx = xres/2;
	int cy = yres/2;
	glBegin(GL_QUADS);
		glVertex2f(cx-100, cy+100);
		glVertex2f(cx+100, cy+100);
		glVertex2f(cx+100, cy-100);
		glVertex2f(cx-100, cy-100);
	glEnd();
	glEnable(GL_TEXTURE_2D);

	r.bot = cy + 20;
	r.left = cx;
	r.center = 1;
	ggprint8b(&r,16, 0x00000000, "Abraham Menu");
    
}
