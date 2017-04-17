#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "ramonR.h"
#include "ppm.h"


void drawRamRMenu(int xres, int yres, Rect r)
{
    glColor3f(0.5, 0.0, 0.0);
    int cx = xres/2;
    int cy = yres/2;
    glBegin(GL_QUADS);
	glVertex2f(cx-500, cy+500);
	glVertex2f(cx+500, cy+500);
	glVertex2f(cx+500, cy-500);
	glVertex2f(cx-500, cy-500);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot = cy + 20;
    r.left = cx;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ffff00, "This is Ramon's menu");
}
//-------------------------------------------------------------------
// upload images onto the program
// movment just left to right
// reference from rainforest framework section: ./~gordon/3350/code/framework
int done = 0;
int xres = 800, yres = 600;
typedef struct t_hitters {
	Vec pos;
	Vec vel;
} Hitters;
Hitters hitters;
Ppimage *hittersImage = NULL;
// Ppimage *backgroundImage = NULL;
// and other objects
GLunit hittersTexture;
// GLunit backgroundTexture;
// and other objects
int showHitters = 0;
// int objects;
// delcare other objects;
// typedef struct t_otherObjects {
	
// } otherObjects;
unsigned char *buildAlphadata(Ppimage *img) {
	int i, a, b, c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *) img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i = 0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}
void initOpengl(void) {
	glViewport(0, 0, xres, yres);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	
	

