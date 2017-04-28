//3350 Spring 2017
//
//program: rocketlander.cpp
//author: Team Rocket 
//date:    2017
// 
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "ppm.h"

#include "nicholasP.h"
#include "abrahamA.h"
#include "patrickE.h"
#include "ramonR.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
const float GRAVITY = 0.006;

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

// Patrick's extern-------------------------------------------------
#ifdef USE_OPENAL_SOUND
extern void startUpSound();
extern void cleanSound();
extern struct Global {
    ALuint alBufferLaser, alBufferBooster, alBufferAstroid, alBufferVictory,
	   alBufferCollide;
    ALuint alSourceLaser, alSourceBooster, alSourceAstroid, alSourceVictory,
	   alSourceCollide;

}p;
#endif //end openal sound
//------------------------------------------------------------------

int xres=1250, yres=900;
int credits = 0;
int renderShip = 0;
int pat_menu = 0;
int ramon_menu = 0;

Ppmimage * backgroundImage=NULL;
Ppmimage *bg_image=NULL;
GLuint backgroundTexture;
GLuint bg_texture;

struct Game {


    Ship2 ship2; // Nick's ship class
    Platform ground[5];
    Platform plats[2]; // Nick's platform class
    Goal goal;
    Fueler fueler;
    Game() {
	ship2.enableBooster2();
	ship2.setPosX(53);
	ship2.setPosY(50);

	ground[0].setPosX(0);
	ground[0].setPosY(0);
	ground[0].setWidth(365);
	ground[0].setHeight(50);

	ground[1].setPosX(365);
	ground[1].setPosY(0);
	ground[1].setWidth(420);
	ground[1].setHeight(30);

	ground[2].setPosX(785);
	ground[2].setPosY(0);
	ground[2].setWidth(240);
	ground[2].setHeight(50);

	ground[3].setPosX(1025);
	ground[3].setPosY(0);
	ground[3].setWidth(100);
	ground[3].setHeight(75);

	ground[4].setPosX(1125);
	ground[4].setPosY(0);
	ground[4].setWidth(125);
	ground[4].setHeight(100);

	plats[0].setPosX(100);
	plats[0].setPosY(600);
	plats[0].setWidth(100);
	plats[0].setHeight(10);

	plats[1].setPosX(550);
	plats[1].setPosY(440);
	plats[1].setWidth(100);
	plats[1].setHeight(10);

	goal.setPosX(950);
	goal.setPosY(740);
	goal.setWidth(100);
	goal.setHeight(10);


	fueler.setPosX(850);
	fueler.setPosY(340);
	fueler.setWidth(100);
	fueler.setHeight(10);

    }
};

int keys[65536];

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
int check_keys(XEvent *e);
void init();
#ifdef USE_OPENAL_SOUND
void init_sounds(void);
#endif //end openal sound
void physics(Game *game);
void render(Game *game);
void set_mouse_position(int x, int y);
void show_mouse_cursor(const int onoff);


#ifdef USE_OPENAL_SOUND
void init_sounds() {
    playSound(p.alSourceAstroid);	//bgm
}
#endif //end openal sound

int main(void)
{
    logOpen();
    initXWindows();
    imageConvert();
    init_opengl();
#ifdef USE_OPENAL_SOUND
    startUpSound();		// from Patrick to add sound
    init_sounds();
#endif //end openal sound
    Game game;
    init();
    srand(time(NULL));
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    set_mouse_position(100,100);
    int done=0;
    while (!done) {
	while (XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_resize(&e);
	    done = check_keys(&e);
	}
	clock_gettime(CLOCK_REALTIME, &timeCurrent);
	timeSpan = timeDiff(&timeStart, &timeCurrent);
	timeCopy(&timeStart, &timeCurrent);
	physicsCountdown += timeSpan;
	while (physicsCountdown >= physicsRate) {
	    physics(&game);
	    physicsCountdown -= physicsRate;
	}
	render(&game);
	glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    imageClean();
    cleanup_fonts();
#ifdef USE_OPENAL_SOUND
    cleanSound();		// clean up sounds
#endif //end openal sound
    logClose();
    return 0;
}

void cleanupXWindows(void)
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Rocket Lander");
}

void setup_screen_res(const int w, const int h)
{
    xres = w;
    yres = h;
}

void initXWindows(void)
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XSetWindowAttributes swa;
    setup_screen_res(xres, yres);
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	std::cout << "\n\tcannot connect to X server" << std::endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	std::cout << "\n\tno appropriate visual found\n" << std::endl;
	exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
	PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
	    vi->depth, InputOutput, vi->visual,
	    CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    show_mouse_cursor(0);
}

void reshape_window(int width, int height)
{
    //window has been resized.
    setup_screen_res(width, height);
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, xres, 0, yres, -1, 1);
    set_title();
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, xres, yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, xres, 0, yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    //// load image files into a ppm structure
    backgroundImage	= ppm6GetImage("./images/background.ppm");

    // Initialize group member images
    inHitters(); // Initialize hitter image
    init_ship_image();

    //create opengl texture elements
    glGenTextures(1, &backgroundTexture);

    //background
    glBindTexture(GL_TEXTURE_2D,backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    backgroundImage->width, backgroundImage->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data);	

    bg_image = ppm6GetImage("./images/background2.ppm");	
    glGenTextures(1, &bg_texture);
    glBindTexture(GL_TEXTURE_2D, bg_texture);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bg_image->width, bg_image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, bg_image->data);

}

void check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != xres || xce.height != yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}

void init()
{
    memset(keys, 0, 65536);
}

void normalize(Vec v)
{
    Flt len = v[0]*v[0] + v[1]*v[1];
    if (len == 0.0f) {
	v[0] = 1.0;
	v[1] = 0.0;
	return;
    }
    len = 1.0f / sqrt(len);
    v[0] *= len;
    v[1] *= len;
}

void set_mouse_position(int x, int y)
{
    XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
}

void show_mouse_cursor(const int onoff)
{
    if (onoff) {
	//this removes our own blank cursor.
	XUndefineCursor(dpy, win);
	return;
    }
    //vars to make blank cursor
    Pixmap blank;
    XColor dummy;
    char data[1] = {0};
    Cursor cursor;
    //make a blank cursor
    blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
    if (blank == None)
	std::cout << "error: out of memory." << std::endl;
    cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(dpy, blank);
    //this makes you the cursor. then set it using this function
    XDefineCursor(dpy, win, cursor);
    //after you do not need the cursor anymore use this function.
    //it will undo the last change done by XDefineCursor
    //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
}

int check_keys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    //Log("key: %i\n", key);
    if (e->type == KeyRelease) {
	keys[key]=0;
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift=0;
	return 0;
    }
    if (e->type == KeyPress) {
	//std::cout << "press" << std::endl;
	keys[key]=1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
	    shift=1;
	    return 0;
	}
    } else {
	return 0;
    }
    if (shift){}
    switch (key) {
	case XK_Escape:
	    return 1;
	case XK_f:
	    break;
	case XK_n:
	    break;
	case XK_t:
	    break;
	case XK_Down:
	    break;
	case XK_equal:
	    break;
	case XK_minus:
	    break;
	case XK_c:
	    credits = credits ^ 1;
	    break;
	case XK_r:
	    ramon_menu = ramon_menu ^ 1;
	    break;
	case XK_p:
	    pat_menu = pat_menu ^ 1;
	    break;
    }
    return 0;
}

void physics(Game *g)
{
    g->ship2.addGravity(GRAVITY);
    g->ship2.move();

    // Check for collision with platforms
    for (int i=0; i<5; i++) {
	g->ship2.collidesWith(g->ground[i]);
    }
    for (int i=0; i<2; i++) {
	g->ship2.collidesWith(g->plats[i]);
    }

    // Check for collision with goal platform
    g->ship2.collidesWith(g->goal);
    g->ship2.goalTriggered(g->goal);

    // Check for collision with fueler platform
    g->ship2.collidesWith(g->fueler);
    // If fueler is triggered and it has fuel left
    if (g->ship2.fuelerTriggered(g->fueler) && g->fueler.getFuelLeft() > 1) {
	// Do not overfill the ship
	if (!(g->ship2.getFuelLeft() > g->ship2.getFuelMax()-1)) {
	    g->fueler.removeFuel();
	    g->ship2.addFuel();
	}
    }

    if (g->ship2.getPosX() < 0.0) {
	g->ship2.setPosX(g->ship2.getPosX() + (float)xres);
    }
    else if (g->ship2.getPosX() > (float)xres) {
	g->ship2.setPosX(g->ship2.getPosX() - (float)xres);
    }
    //---------------------------------------------------
    //check keys pressed now
    if (keys[XK_Left]) {
	g->ship2.rotateLeft();

    }
    if (keys[XK_Right]) {
	g->ship2.rotateRight();
    }
    if (keys[XK_Up]) {
#ifdef USE_OPENAL_SOUND
	playSound(p.alSourceBooster);	//Booster
#endif //end openal sound
	g->ship2.accelerate();
    }
}

void render(Game *g)
{

    glClear(GL_COLOR_BUFFER_BIT);

    // Background Image
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);

    glEnd();
    glPopMatrix();

    renderAstro();
    g->ship2.draw();

    Rect r;
    
    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Rocket Lander");


    // Draw fuel gauge
    Rect fuelBar;
    fuelBar.bot = yres - 43;
    fuelBar.left = xres*.5-10;
    fuelBar.center = 0;
    drawFuelGauge(g->ship2.getFuelLeft(), g->ship2.getFuelMax(), xres*.5, 850);
    ggprint8b(&fuelBar, 16, 0x00ff0000, "Fuel");

    glColor3ub(51,102,0);
    for (int i=0;i<5;i++) {
	g->ground[i].draw();
    }

    glColor3ub(255,165,0);
    g->plats[0].draw();
    g->plats[1].draw();

    g->goal.draw();
    g->fueler.draw();

    //draw abrahams menu
    if (credits == 1) {
	showCredits(xres,yres,r);
    }
    // Draw Pat's Menu
    if (pat_menu == 1) {
	startMenu(r);
	//UpgradeMenu(xres, yres, r);
    }
    //if (pat_menu== 0) {
    //    clearMenu();
    //}
    // Draw Ramon's Menu
    if (ramon_menu == 1) {
	//drawRamRMenu(xres, yres, r);
    }
}
