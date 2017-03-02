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
#include "patrickE.h"

// sound stuff
#include <fcntl.h>
#include <sys/stat.h>
#include </usr/include/AL/alut.h>


//#include "ppm.h"

/*
Ppmimage *forestImage=NULL;
GLuint forestTexture;
int forest=1;
forestImage	= ppm6GetImage("./images/forest.ppm");
glGenTextures(1, &forestTexture);

glBindTexture(GL_TEXTURE_2D, forestTexture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0,3, forestImage-> width, forestImage->height,
	0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);
*/

/**********UPGRADE MENUS**********/
void UpgradeMenu(int xres,int yres,Rect r)
{
    glColor3f(.13,.49,.2);
    int cx = xres/2;
    int cy = yres/2;
    glBegin(GL_QUADS);
    glVertex2i(cx-100,cy+100);
    glVertex2i(cx+100,cy+100);
    glVertex2i(cx+100,cy-100);
    glVertex2i(cx-100,cy-100);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot=cy+60;
    r.left=cx;
    r.center=1;
    ggprint16(&r,40,0,"Upgrades");    
    ggprint13(&r,20,0,"Booster Uprades");    
    ggprint13(&r,20,0,"Laser Upgrades");    
}

void BoosterMenu(int xres,int yres,Rect r)
{
    glColor3f(.13,.49,.2);
    int cx = xres/2;
    int cy = yres/2;
    glBegin(GL_QUADS);
    glVertex2i(cx-100,cy+100);
    glVertex2i(cx+100,cy+100);
    glVertex2i(cx+100,cy-100);
    glVertex2i(cx-100,cy-100);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot=cy+60;
    r.left=cx;
    r.center=1;
    ggprint16(&r,40,0,"Booster Upgrades");    
    ggprint13(&r,20,0,"Booster Level 1");    
    ggprint13(&r,20,0,"Booster Level 2");    
    ggprint13(&r,20,0,"Booster Level 3");    
}

void LaserMenu(int xres,int yres,Rect r)
{
    glColor3f(.13,.49,.2);
    int cx = xres/2;
    int cy = yres/2;
    glBegin(GL_QUADS);
    glVertex2i(cx-100,cy+100);
    glVertex2i(cx+100,cy+100);
    glVertex2i(cx+100,cy-100);
    glVertex2i(cx-100,cy-100);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot=cy+60;
    r.left=cx;
    r.center=1;
    ggprint16(&r,40,0,"Laser Upgrades");    
    ggprint13(&r,20,0,"Laser Fire Speed 1");    
    ggprint13(&r,20,0,"Laser Fire Speed 2");    
    ggprint13(&r,20,0,"Dual Laser Fire");    
    ggprint13(&r,20,0,"Triple Laser Fire");    
}

/**********SOUNDS**********/
// testing out gordon's sound file first
void LaserSound()
{
    //Start up
    alutInit(0,NULL);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERROR: alutIni()\n");
    }
    alGetError();
    //Setup listener
    float vec[6] = {0,0,1,0,1,0};
    alListener3f(AL_POSITION, 0,0,0);
    alListenerfv(AL_ORIENTATION,vec);
    alListenerf(AL_GAIN, 1);

    //Buffer holds sound info
    ALuint alBuffer;
    alBuffer = alutCreateBufferFromFile("/sounds/test.wav");

    //Source refers to sound
    ALuint alSource;

    //Generate a source
    alGenSources(1, &alSource);
    alSourcei(alSource, AL_BUFFER, alBuffer);

    //Set volume and pitch to normal, no looping
    alSourcef(alSource, AL_GAIN, 1);
    alSourcef(alSource, AL_PITCH,1);
    alSourcei(alSource, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERror: sound\n");
    }
}
