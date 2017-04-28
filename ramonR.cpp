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
#define ALPHA 1
unsigned char *buildAlphaData(Ppmimage*);

Ppmimage * hitterImage;
GLuint silhouetteTexture;
GLuint hitterTexture;

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

void inHitters(void) {
	hitterImage = ppm6GetImage("./images/hitters.ppm");
	glGenTextures(1, &silhouetteTexture);
	glGenTextures(1, &hitterTexture);

//Hitters
//init. of the enemy image
	glBindTexture(GL_TEXTURE_2D,hitterTexture);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    hitterImage->width, hitterImage->height,
0, GL_RGB, GL_UNSIGNED_BYTE, hitterImage->data);

//Hitter alpha
//init. hitters
	glBindTexture(GL_TEXTURE_2D,silhouetteTexture);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    // must bild a new set of data...
    	unsigned char *silhouetteData = buildAlphaData(hitterImage);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	    hitterImage->width, hitterImage->height,
	    0, GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);	
	free(silhouetteData); 
	
}




//-------------------------------------------------------------------
// upload images onto the program
// movment just left to right
// reference from rainforest framework section: ./~gordon/3350/code/framework
/*
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
GLunit silhouetteTexture;
// GLunit backgroundTexture;
// and other objects
int showHitters = 0;
int silhouette = 1;
// int objects;
// delcare other objects;
// typedef struct t_otherObjects {
	
// } otherObjects;
void setupScreenRes(const int w, const int h) {
	xres = w;
	yres = h;
}
*/
unsigned char *buildAlphaData(Ppmimage *img) {
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
/*
void initOpengl(void) {
	glViewport(0, 0, xres, yres);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	hittersImage = ppm6GetImage("./Images/hitters.ppm");
	// other objects add-ons
	glGenTextures(1, &HittersTexture);
	// other objects add-ons
	// Visual enemies/aliens
	int w = hittersimage->width;
	int h = hittersImage->height;
	glBindTexture(GL_TEXTURE_2D, hittersTexture);
	glTextParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0 GL_RGB, GL_UNSIGNED_BYTE, hittersImage->data);
	// For background layout
	// look at rainforest.cpp for background upload
	// also must add a transparent background so other objects can be visual
	// add code below for background
}
void moveHitters() {
	int addgrav = 1;
	//update position
	hitters.pos[0] += hitters.vel[0];
	hitters.pos[1] += hitters.vel[1];
	//Check for collision with window edges
	if ((hitters.pos[0] < -140.0 && hitters.vel[0] < 0.0 || 
	     		(hitters.pos[0] >= (float)xres + 140.0 && hitters.vel[0] > 0.0)) {
		hitters.vel[0] = -hitters.vel[0];
		addgrav = 0;
	}
	if ((hitters.pos[1] < 150.0 && king.vel[1] < 0.0 || 
	     		(hitters.pos[1] >= (float)yres && hitters.vel[1] > 0.0)) {
		hitters.vel[1] = -hitters.vel[1];
		addgrav = 0;
	}
	// addgrav = gravity
	if (addgrav)
	    hitters.vel[1] -= 0.75;
	    }
void physics(void) {
	if(showHitters)
		moveHitters();
}
*/
void renderAstro(void) {
//clear screen
//draw a quad with texture
//render the background
// Hitter Image
	glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
        glPushMatrix();
	//glTranslated(x, y, 0); // Update this every frame to move asteroid
        glTranslated(0, 0, 0);
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f); //Alpha
        glBegin(GL_QUADS);
        float w = 1;
        float h = 1;
        glTexCoord2f(0.0f, h); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, hitterImage->height);
        glTexCoord2f(w, 0.0f); glVertex2i(hitterImage->width, hitterImage->height);
        glTexCoord2f(w, h); glVertex2i(hitterImage->width, 0);

        glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}	
	    
	    
	    
		
