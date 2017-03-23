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

struct Global {
    ALuint alBufferLaser, alBufferBooster;
    ALuint alSourceLaser, alSourceBooster;
} g;




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

// add extern void startUpSound to main
void startUpSound()
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
    g.alBufferLaser = alutCreateBufferFromFile("test.wav");
    //g.alBufferLaser = alutCreateBufferFromFile("./sounds/laser.wav");
    g.alBufferBooster = alutCreateBufferFromFile("test.wav");
    //g.alBufferLaser = alutCreateBufferFromFile("./sounds/booster.wav");

    //Generate source and store into buffer Laser
    alGenSources(1, &g.alSourceLaser);
    alSourcei(g.alSourceLaser, AL_BUFFER, g.alBufferLaser);
    //Set volume and pitch to normal, no looping
    alSourcef(g.alSourceLaser, AL_GAIN, 1);
    alSourcef(g.alSourceLaser, AL_PITCH,1);
    alSourcei(g.alSourceLaser, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERror: laser sound\n");
    }
    //Generate source and store into buffer Booster
    alGenSources(1, &g.alSourceBooster);
    alSourcei(g.alSourceBooster, AL_BUFFER, g.alBufferBooster);
    //Set volume and pitch to normal, no looping
    alSourcef(g.alSourceBooster, AL_GAIN, 5);
    alSourcef(g.alSourceBooster, AL_PITCH,5);
    alSourcei(g.alSourceBooster, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERror: booster sound\n");
    }
}

void playSound(ALuint source)
{
    alSourcePlay(source);
}
/*
//when space is hit, shoot bullet when < MAX_BULLLETS
void LaserSound(ALuint alSource)
{
//Buffer holds sound info
ALuint alBuffer;
alBuffer = alutCreateBufferFromFile("test.wav");
//alBuffer = alutCreateBufferFromFile("./sounds/laser.wav");

//Source refers to sound
//ALuint alSource;

//Generate a source
alGenSources(1, &alSource);
alSourcei(alSource, AL_BUFFER, alBuffer);

//Set volume and pitch to normal, no looping
alSourcef(alSource, AL_GAIN, 1);
alSourcef(alSource, AL_PITCH,1);
alSourcei(alSource, AL_LOOPING, AL_FALSE);
if (alGetError() != AL_NO_ERROR) {
printf("ERror: laser sound\n");
alDeleteSources(1,&alSource);
alDeleteBuffers(1,&alBuffer);
alSourceRewind(alSource);
}
else { 
alSourcePlay(alSource);
}
}

void BoosterSound(ALuint alSource)
{
//Buffer holds sound info
ALuint alBuffer;
alBuffer = alutCreateBufferFromFile("test.wav");  
//alBuffer = alutCreateBufferFromFile("./sounds/booster.wav");

//Source refers to sound
//ALuint alSource;

//Generate a source
alGenSources(1, &alSource);
alSourcei(alSource, AL_BUFFER, alBuffer);

//Set volume and pitch to normal, no looping
alSourcef(alSource, AL_GAIN, 3);
alSourcef(alSource, AL_PITCH, 3);
alSourcei(alSource, AL_LOOPING, AL_FALSE);
if (alGetError() != AL_NO_ERROR) {
printf("Error: booster sound\n");
alDeleteSources(1,&alSource);
alDeleteBuffers(1,&alBuffer);
alGetError();
}
alSourcePlay(alSource);
}
*/
// add extern void cleanSound to main
void cleanSound()
{
    //Cleanup
    //delete the source
    alDeleteSources(1,&g.alSourceLaser);
    alDeleteSources(1,&g.alSourceBooster);
    //delete buffer
    alDeleteBuffers(1,&g.alBufferLaser);   
    alDeleteBuffers(1,&g.alBufferBooster);   
    //close out openal
    //get active context
    ALCcontext *Context = alcGetCurrentContext();
    //get device for active contex
    ALCdevice *Device = alcGetContextsDevice(Context);
    //disable contex
    alcMakeContextCurrent(NULL);
    //release context(s)
    alcDestroyContext(Context);
    //close device
    alcCloseDevice(Device);
}

