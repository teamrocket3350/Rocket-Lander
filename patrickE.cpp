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
#include "abrahamA.h"

//image stuff
#include "ppm.h"

// sound stuff
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //use openal sound

#ifdef USE_OPENAL_SOUND
struct Global {
    ALuint alBufferLaser, alBufferBooster, alBufferAstroid, alBufferVictory, 
	   alBufferCollide;
    ALuint alSourceLaser, alSourceBooster, alSourceAstroid, alSourceVictory, 
	   alSourceCollide;
} p;
#endif // close use_openal_sound

extern Ppmimage *bg_image;
extern GLuint bg_texture;
extern Ppmimage *backgroundImage;
extern GLuint backgroundTexture;
extern int xres,yres;
extern int pat_menu;
extern int credits ;
int upgrade_menu;
int fire_menu ;
int boost_menu ;


// Playing with background image
void startMenu(Rect r)
{
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0, yres);
    glVertex2i(xres, yres);
    glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, bg_texture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1); glVertex2i(0, 0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0, yres);
    glTexCoord2f(1, 0.0); glVertex2i(xres, yres);
    glTexCoord2f(1, 1); glVertex2i(xres, 0);
    glEnd();

    int cx = xres/2;
    int cy = yres/2;
    glEnable(GL_TEXTURE_2D);
    r.bot=cy;
    r.left=cx;
    r.center=1;
    ggprint16(&r,160,0xffcc11,"Rocket Lander");    
    ggprint13(&r,20,0xffcc11,"Press P to Play"); 
    ggprint13(&r,20,0xffcc11,"Press C for Credits"); 
}

//void checkBooster()
//{
//    if (g->ship2.enabledBooster1()) enableBooster2();
//    else if (g->ship2.enabledBooster2()) enableBooster3();
//    else if (g->ship2.enabledBooster3()) enableBooster1();
//}

void UpgradeMenu(Rect r)
{
    glColor3f(.13,.49,.2);
    int cx = xres/2;
    int cy = yres/2;
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex2i(cx-100,cy+100);
    glVertex2i(cx+100,cy+100);
    glVertex2i(cx+100,cy-100);
    glVertex2i(cx-100,cy-100);
    glEnd();
    r.bot= cy+60;
    r.left=cx;
    r.center=1;
    ggprint16(&r,40,0,"Upgrades");    
    ggprint13(&r,20,0,"Booster Uprades(press b)");    
    ggprint13(&r,20,0,"Laser Upgrades(press f)"); 
}

void BoosterMenu(Rect r)
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

void LaserMenu(Rect r)
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
    ggprint13(&r,20,0,"Laser Fire Speed 3");    
}

void menus(Rect r)
{
    //draw Pat's Menu
    if (pat_menu == 1) {
	upgrade_menu = 0;
	startMenu(r);
    } 
    //draw abrahams menu
    if (pat_menu == 1 &&credits == 1) {
	showCredits(xres,yres,r);
    }
    else credits =0;
}

void menus2(Rect r)
{
    if (pat_menu == 0) {
	if (boost_menu == 1 && fire_menu == 0) {
	    BoosterMenu(r);
	}
	else if (boost_menu == 0 && fire_menu == 1) {
	    LaserMenu(r);
	}
	else boost_menu = 0, fire_menu = 0;    
    }
}


/**********  SOUNDS  **********/
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
    p.alBufferAstroid = alutCreateBufferFromFile("./sounds/bg.wav");
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
    //Set volume and pitch to normal, looping
    alSourcef(p.alSourceAstroid, AL_GAIN, 1.0f);
    alSourcef(p.alSourceAstroid, AL_PITCH,1.0f);
    alSourcei(p.alSourceAstroid, AL_LOOPING, AL_TRUE);
    if (alGetError() != AL_NO_ERROR) {
	printf("Error: astroid music\n");
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
void init_sounds() {
    playSound(p.alSourceAstroid);       //bgm
}
#endif //end openal sound

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
