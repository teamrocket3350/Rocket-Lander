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

//image stuff
#include "ppm.h"

// sound stuff
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //use openal sound

extern int xres,yres;

#ifdef USE_OPENAL_SOUND
struct Global {
    ALuint alBufferLaser, alBufferBooster, alBufferAstroid, alBufferVictory, 
	   alBufferCollide;
    ALuint alSourceLaser, alSourceBooster, alSourceAstroid, alSourceVictory, 
	   alSourceCollide;
} p;
#endif // close use_openal_sound

// Playing with background image
void startMenu(void)
{
    Ppmimage *bgImage= NULL;
    GLuint bgTexture;

    //OpenGL initialization
    glViewport(0, 0, xres, yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, xres, 0, yres, -1, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    //Clear the screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    bgImage = ppm6GetImage("./images/background.ppm");

    //create opengl texture elements
    glGenTextures(1, &bgTexture);

    glBindTexture(GL_TEXTURE_2D, bgTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    bgImage->width, bgImage->height, 
	    0, GL_RGB, GL_UNSIGNED_BYTE, bgImage->data);
}

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
#ifdef USE_OPENAL_SOUND
void startUpSound()
{
    //Start up
    alutInit(0,NULL);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERROR: alutIni()\n");
    }
    alGetError();
    //Setup listener
    float vec[6] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
    alListener3f(AL_POSITION, 0,0,0);
    alListenerfv(AL_ORIENTATION,vec);
    alListenerf(AL_GAIN, 1);
    p.alBufferLaser = alutCreateBufferFromFile("test.wav");
    //p.alBufferLaser = alutCreateBufferFromFile("./sounds/laser.wav");
    //p.alBufferBooster = alutCreateBufferFromFile("test.wav");
    p.alBufferBooster = alutCreateBufferFromFile("./sounds/booster.wav");
    //p.alBufferAstroid = alutCreateBufferFromFile("test.wav");
    p.alBufferAstroid = alutCreateBufferFromFile("./sounds/astroid.wav");
    //p.alBufferVictory = alutCreateBufferFromFile("test.wav");
    p.alBufferVictory = alutCreateBufferFromFile("./sounds/victory.wav");
    //p.alBufferCollide = alutCreateBufferFromFile("test.wav");
    p.alBufferCollide = alutCreateBufferFromFile("./sounds/collide.wav");

    //Generate source and store into buffer Laser
    alGenSources(1, &p.alSourceLaser);
    alSourcei(p.alSourceLaser, AL_BUFFER, p.alBufferLaser);
    //Set volume and pitch to normal, no looping
    alSourcef(p.alSourceLaser, AL_GAIN, 1.0f);
    alSourcef(p.alSourceLaser, AL_PITCH,1.0f);
    alSourcei(p.alSourceLaser, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("Error: laser sound\n");
    }

    //Generate source and store into buffer Booster
    alGenSources(1, &p.alSourceBooster);
    alSourcei(p.alSourceBooster, AL_BUFFER, p.alBufferBooster);
    //Set volume and pitch to normal, no looping
    alSourcef(p.alSourceBooster, AL_GAIN, 1.0f);
    alSourcef(p.alSourceBooster, AL_PITCH,1.0f);
    alSourcei(p.alSourceBooster, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("Error: booster sound\n");
    }

    //Generate source and store into buffer Astroid
    alGenSources(1, &p.alSourceAstroid);
    alSourcei(p.alSourceAstroid, AL_BUFFER, p.alBufferAstroid);
    //Set volume and pitch to normal, no looping
    alSourcef(p.alSourceAstroid, AL_GAIN, 1.0f);
    alSourcef(p.alSourceAstroid, AL_PITCH,1.0f);
    alSourcei(p.alSourceAstroid, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("Error: astroid destruction sound\n");
    }

    //Generate source and store into buffer Victory
    alGenSources(1, &p.alSourceVictory);
    alSourcei(p.alSourceVictory, AL_BUFFER, p.alBufferVictory);
    //Set volume and pitch to normal, no looping
    alSourcef(p.alSourceVictory, AL_GAIN, 1.0f);
    alSourcef(p.alSourceVictory, AL_PITCH,1.0f);
    alSourcei(p.alSourceVictory, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("Error: victory sound\n");
    }

    //Generate source and store into buffer Collide
    alGenSources(1, &p.alSourceCollide);
    alSourcei(p.alSourceCollide, AL_BUFFER, p.alBufferCollide);
    //Set volume and pitch to normal, no looping
    alSourcef(p.alSourceCollide, AL_GAIN, 1.0f);
    alSourcef(p.alSourceCollide, AL_PITCH,1.0f);
    alSourcei(p.alSourceCollide, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("Error: Collide sound\n");
    }
}
#endif // close use_openal_sound

#ifdef USE_OPENAL_SOUND
void playSound(ALuint source)
{
    alSourcePlay(source);
}
#endif // close use_openal_sound

#ifdef USE_OPENAL_SOUND
void cleanSound()
{
    //Cleanup
    //delete the source
    alDeleteSources(1,&p.alSourceLaser);
    alDeleteSources(1,&p.alSourceBooster);
    alDeleteSources(1,&p.alSourceAstroid);
    alDeleteSources(1,&p.alSourceVictory);
    alDeleteSources(1,&p.alSourceCollide);
    //delete buffer
    alDeleteBuffers(1,&p.alBufferLaser);   
    alDeleteBuffers(1,&p.alBufferBooster);   
    alDeleteBuffers(1,&p.alBufferAstroid);   
    alDeleteBuffers(1,&p.alBufferVictory);
    alDeleteBuffers(1,&p.alBufferCollide);
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
#endif // close use_openal_sound

