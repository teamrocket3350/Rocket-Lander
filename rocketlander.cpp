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
const float GRAVITY = 0.005;

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
extern void drawnicholasMenu(int xres, int yres, Rect r);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

// Patrick's extern-------------------------------------------------
extern void startUpSound();
extern void cleanSound();
extern struct Global {
    ALuint alBufferLaser, alBufferBooster, alBufferAstroid, alBufferVictory,
	   alBufferCollide;
    ALuint alSourceLaser, alSourceBooster, alSourceAstroid, alSourceVictory,
	   alSourceCollide;
}p;
//------------------------------------------------------------------

int xres=1250, yres=900;
int credits = 0;
int nick_menu = 0;
int renderShip = 1;
int pat_menu = 0;
int ramon_menu = 0;

struct Ship {
    Vec dir;
    Vec pos;
    Vec vel;
    // Added shape
    Shape s;

    float angle;
    float color[3];
    Ship() {
	VecZero(dir);
	pos[0] = 60;
	pos[1] = 25;
	pos[2] = 0.0f;
	VecZero(vel);
	angle = 0.0;
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;
	// Init shape size
	s.width = 40;
	s.height = 60;
    }
};

struct Bullet {
    Vec pos;
    Vec vel;
    float color[3];
    struct timespec time;
    Bullet() { }
};

struct Asteroid {
    Vec pos;
    Vec vel;
    int nverts;
    Shape s;
    Vec vert[8];
    float angle;
    float rotate;
    float color[3];
    struct Asteroid *prev;
    struct Asteroid *next;
    Asteroid() {
	prev = NULL;
	next = NULL;
    }
};

struct Game {


    Ship ship;
    Ship2 ship2; // Nick's ship class
    Asteroid *ahead;
    Bullet *barr;
    int nasteroids;
    int nbullets;
    struct timespec bulletTimer;
    struct timespec mouseThrustTimer;
    //Shape platforms[2];
	Platform ground[5];
    Platform plat[2]; // Nick's platform class
    bool mouseThrustOn;
    Game() {
	ahead = NULL;
	barr = new Bullet[MAX_BULLETS];
	nasteroids = 0;
	nbullets = 0;

	ship2.enableBooster2();
	ship2.setPosX(53);
	ship2.setPosY(15);

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

	plat[0].setPosX(100);
	plat[0].setPosY(600);
	plat[0].setWidth(100);
	plat[0].setHeight(10);

	plat[1].setPosX(950);
	plat[1].setPosY(740);
	plat[1].setWidth(100);
	plat[1].setHeight(10);

	mouseThrustOn = false;
    }
    ~Game() {
	delete [] barr;
    }
};

int keys[65536];

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
//void check_mouse(XEvent *e, Game *game);
int check_keys(XEvent *e);
void init(Game *g);
void init_sounds(void);
void physics(Game *game);
void render(Game *game);
void set_mouse_position(int x, int y);
void show_mouse_cursor(const int onoff);

int main(void)
{
    logOpen();
    initXWindows();
    init_opengl();
    startUpSound();		// from Patrick to add sound
    Game game;
    init(&game);
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
	    //check_mouse(&e, &game);
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
    cleanup_fonts();
    cleanSound();		// clean up sounds
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

void init(Game *g)
{
    //build 3 asteroids...
    for (int j=0; j<3; j++) {
	Asteroid *a = new Asteroid;
	a->nverts = 8;
	a->s.radius = rnd()*40.0 + 20.0;
	Flt r2 = a->s.radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)a->nverts;
	for (int i=0; i<a->nverts; i++) {
	    a->vert[i][0] = sin(angle) * (r2 + rnd() * a->s.radius);
	    a->vert[i][1] = cos(angle) * (r2 + rnd() * a->s.radius);
	    angle += inc;
	}
	a->pos[0] = (Flt)(rand() % xres);
	a->pos[1] = (Flt)(rand() % yres);
	a->pos[2] = 0.0f;
	a->angle = 0.0;
	a->rotate = rnd() * 4.0 - 2.0;
	a->color[0] = 0.8;
	a->color[1] = 0.8;
	a->color[2] = 0.7;
	a->vel[0] = (Flt)(rnd()*2.0-1.0);
	a->vel[1] = (Flt)(rnd()*2.0-1.0);
	//std::cout << "asteroid" << std::endl;
	//add to front of linked list
	a->next = g->ahead;
	if (g->ahead != NULL)
	    g->ahead->prev = a;
	g->ahead = a;
	g->nasteroids++;
    }
    clock_gettime(CLOCK_REALTIME, &g->bulletTimer);
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
	    nick_menu = nick_menu ^ 1;
	    break;
	case XK_t:
	    renderShip ^= 1;
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

void deleteAsteroid(Game *g, Asteroid *node)
{
    //remove a node from doubly-linked list
    if (node->prev == NULL) {
	if (node->next == NULL) {
	    //only 1 item in list.
	    g->ahead = NULL;
	} else {
	    //at beginning of list.
	    node->next->prev = NULL;
	    g->ahead = node->next;
	}
    } else {
	if (node->next == NULL) {
	    //at end of list.
	    node->prev->next = NULL;
	} else {
	    //in middle of list.
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    delete node;
    node = NULL;
}

void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
    //build ta from a
    ta->nverts = 8;
    ta->s.radius = a->s.radius / 2.0;
    Flt r2 = ta->s.radius / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)ta->nverts;
    for (int i=0; i<ta->nverts; i++) {
	ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->s.radius);
	ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->s.radius);
	angle += inc;
    }
    ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
    ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
    ta->pos[2] = 0.0f;
    ta->angle = 0.0;
    ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
    ta->color[0] = 0.8;
    ta->color[1] = 0.8;
    ta->color[2] = 0.7;
    ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
    ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
    //std::cout << "frag" << std::endl;
}

void physics(Game *g)
{
    Flt d0,d1,dist;
    //Update ship position
    g->ship.pos[0] += g->ship.vel[0];
    g->ship.pos[1] += g->ship.vel[1];
	// My ship
	g->ship2.addGravity(GRAVITY);
    g->ship2.setPosX(g->ship2.getPosX()+g->ship2.getVelX());
    g->ship2.setPosY(g->ship2.getPosY()+g->ship2.getVelY());

	// Check for collision with platforms
	for (int i=0; i<5; i++) {
		g->ship2.collidesWith(g->ground[i]);
	}
	for (int i=0; i<2; i++) {
		g->ship2.collidesWith(g->plat[i]);
	}
	
    //Check for collision with window edges
    if (g->ship.pos[0] < 0.0) {
	g->ship.pos[0] += (float)xres;
    }
    else if (g->ship.pos[0] > (float)xres) {
	g->ship.pos[0] -= (float)xres;
    }
    else if (g->ship.pos[1] < 0.0) {
	g->ship.pos[1] += (float)yres;
    }
    else if (g->ship.pos[1] > (float)yres) {
	g->ship.pos[1] -= (float)yres;
    }
	// My ship
    if (g->ship2.getPosX() < 0.0) {
		g->ship2.setPosX(g->ship2.getPosX() + (float)xres);
    }
    else if (g->ship2.getPosX() > (float)xres) {
		g->ship2.setPosX(g->ship2.getPosX() - (float)xres);
    }
    else if (g->ship2.getPosY() < 0.0) {
		g->ship2.setPosY(g->ship2.getPosY() + (float)yres);
    }
    else if (g->ship2.getPosY() > (float)yres) {
		g->ship2.setPosY(g->ship2.getPosY() - (float)yres);
    }
    //
    //
    //Update bullet positions
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i=0;
    while (i < g->nbullets) {
	Bullet *b = &g->barr[i];
	//How long has bullet been alive?
	double ts = timeDiff(&b->time, &bt);
	if (ts > 2.5) {
	    //time to delete the bullet.
	    memcpy(&g->barr[i], &g->barr[g->nbullets-1], sizeof(Bullet));
	    g->nbullets--;
	    //do not increment i.
	    continue;
	}
	//move the bullet
	b->pos[0] += b->vel[0];
	b->pos[1] += b->vel[1];
	//Check for collision with window edges
	if (b->pos[0] < 0.0) {
	    b->pos[0] += (float)xres;
	}
	else if (b->pos[0] > (float)xres) {
	    b->pos[0] -= (float)xres;
	}
	else if (b->pos[1] < 0.0) {
	    b->pos[1] += (float)yres;
	}
	else if (b->pos[1] > (float)yres) {
	    b->pos[1] -= (float)yres;
	}
	i++;
    }
    //
    //Update asteroid positions
    Asteroid *a = g->ahead;
    while (a) {
	a->pos[0] += a->vel[0];
	a->pos[1] += a->vel[1];
	//Check for collision with window edges
	if (a->pos[0] < -100.0) {
	    a->pos[0] += (float)xres+200;
	}
	else if (a->pos[0] > (float)xres+100) {
	    a->pos[0] -= (float)xres+200;
	}
	else if (a->pos[1] < 0) {
	    //else if (a->pos[1] < -100.0) {
	    a->pos[1] = 0;
	    a->vel[1] = -a->vel[1];
	    //a->pos[1] += (float)yres+200;
	}
	else if (a->pos[1] > (float)yres) {
	    //else if (a->pos[1] > (float)yres+100) {
	    a->pos[1] = yres;
	    a->vel[1] = -a->vel[1];
	    //a->pos[1] -= (float)yres+200;
	}
	a->angle += a->rotate;
	a = a->next;
	}
	//
	//Asteroid collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break the asteroid into pieces
	//        if asteroid small, delete it
	a = g->ahead;
	while (a) {
	    //is there a bullet within its radius?
	    int i=0;
	    while (i < g->nbullets) {
		Bullet *b = &g->barr[i];
		d0 = b->pos[0] - a->pos[0];
		d1 = b->pos[1] - a->pos[1];
		dist = (d0*d0 + d1*d1);
		if (dist < (a->s.radius*a->s.radius)) {
		    //std::cout << "asteroid hit." << std::endl;
		    //this asteroid is hit.
		    if (a->s.radius > MINIMUM_ASTEROID_SIZE) {
			//break it into pieces.
			Asteroid *ta = a;
			buildAsteroidFragment(ta, a);
			int r = rand()%10+5;
			for (int k=0; k<r; k++) {
			    //get the next asteroid position in the array
			    Asteroid *ta = new Asteroid;
			    buildAsteroidFragment(ta, a);
			    //add to front of asteroid linked list
			    ta->next = g->ahead;
			    if (g->ahead != NULL)
				g->ahead->prev = ta;
			    g->ahead = ta;
			    g->nasteroids++;
			}
		    } else {
			a->color[0] = 1.0;
			a->color[1] = 0.1;
			a->color[2] = 0.1;
			//asteroid is too small to break up
			//delete the asteroid and bullet
			Asteroid *savea = a->next;
			deleteAsteroid(g, a);
			a = savea;
			g->nasteroids--;
		    }
		    //delete the bullet...
		    memcpy(&g->barr[i], &g->barr[g->nbullets-1], sizeof(Bullet));
		    g->nbullets--;
		    if (a == NULL)
			break;
		    //continue;
		}
		i++;
	    }
	    if (a == NULL)
		break;
	    a = a->next;
	}
	//---------------------------------------------------
	//check keys pressed now
	if (keys[XK_Left]) {
	    g->ship.angle += 4.0;
		g->ship2.setRot(g->ship2.getRot()+4.0);
	    if (g->ship.angle >= 360.0f)
		g->ship.angle -= 360.0f;
	}
	if (keys[XK_Right]) {
	    g->ship.angle -= 4.0;
		g->ship2.setRot(g->ship2.getRot()-4.0);
	    if (g->ship.angle < 0.0f)
		g->ship.angle += 360.0f;
	}
	if (keys[XK_Up]) {
	    //apply thrust
	    //BoosterSound();	//BoosterSound
	    playSound(p.alSourceBooster);	//Booster
	    //convert ship angle to radians
	    Flt rad = ((g->ship2.getRot()+90.0) / 360.0f) * PI * 2.0;
	    //Flt rad = ((g->ship.angle+90.0) / 360.0f) * PI * 2.0;
	    //convert angle to a vector
	    Flt xdir = cos(rad);
	    Flt ydir = sin(rad);
		// My ship
		if (g->ship2.enabledBooster1) {
			g->ship2.setVelX(g->ship2.getVelX() + xdir * 0.02f);
			g->ship2.setVelY(g->ship2.getVelY() + ydir * 0.02f);
		} else if (g->ship2.enabledBooster2) {
			g->ship2.setVelX(g->ship2.getVelX() + xdir * 0.025f);
			g->ship2.setVelY(g->ship2.getVelY() + ydir * 0.025f);
		} else if (g->ship2.enabledBooster3) {
			g->ship2.setVelX(g->ship2.getVelX() + xdir * 0.03f);
			g->ship2.setVelY(g->ship2.getVelY() + ydir * 0.03f);
		}
		// Orig ship
	    g->ship.vel[0] += xdir*0.02f; //
	    g->ship.vel[1] += ydir*0.02f; //
	    Flt speed = sqrt(g->ship.vel[0]*g->ship.vel[0]+
		    g->ship.vel[1]*g->ship.vel[1]);
	    if (speed > 10.0f) {
		speed = 10.0f;
		normalize(g->ship.vel);
		g->ship.vel[0] *= speed;
		g->ship.vel[1] *= speed;
	    }
	}
	if (keys[XK_space]) {
	    //a little time between each bullet
	    struct timespec bt;
	    clock_gettime(CLOCK_REALTIME, &bt);
	    double ts = timeDiff(&g->bulletTimer, &bt);
	    if (ts > 0.1) {
		timeCopy(&g->bulletTimer, &bt);
		if (g->nbullets < MAX_BULLETS) {
		    //LaserSound();	// Laser
		    playSound(p.alSourceLaser);	// Laser
		    //shoot a bullet...
		    //Bullet *b = new Bullet;
		    Bullet *b = &g->barr[g->nbullets];
		    timeCopy(&b->time, &bt);
		    b->pos[0] = g->ship.pos[0];
		    b->pos[1] = g->ship.pos[1];
		    b->vel[0] = g->ship.vel[0];
		    b->vel[1] = g->ship.vel[1];
		    //convert ship angle to radians
		    Flt rad = ((g->ship.angle+90.0) / 360.0f) * PI * 2.0;
		    //convert angle to a vector
		    Flt xdir = cos(rad);
		    Flt ydir = sin(rad);
		    b->pos[0] += xdir*20.0f;
		    b->pos[1] += ydir*20.0f;
		    b->vel[0] += xdir*6.0f + rnd()*0.1;
		    b->vel[1] += ydir*6.0f + rnd()*0.1;
		    b->color[0] = 1.0f;
		    b->color[1] = 1.0f;
		    b->color[2] = 1.0f;
		    g->nbullets++;
		}
	    }
	}
	if (g->mouseThrustOn) {
	    //should thrust be turned off
	    struct timespec mtt;
	    clock_gettime(CLOCK_REALTIME, &mtt);
	    double tdif = timeDiff(&mtt, &g->mouseThrustTimer);
	    //std::cout << "tdif: " << tdif << std::endl;
	    if (tdif < -0.3)
		g->mouseThrustOn = false;
	}
	}

	void render(Game *g)
	{
	    Rect r;
	    glClear(GL_COLOR_BUFFER_BIT);
	    //
	    r.bot = yres - 20;
	    r.left = 10;
	    r.center = 0;
	    ggprint8b(&r, 16, 0x00ff0000, "Rocket Lander");
	    //	    ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g->nbullets);
	    //	    ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g->nasteroids);
	    //-------------------------------------------------------------------------

	    g->ship2.draw();

		glColor3ub(51,102,0);
		for (int i=0;i<5;i++) {
			g->ground[i].draw();
		}

		glColor3ub(255,165,0);
	    g->plat[0].draw();
	    g->plat[1].draw();

		if (renderShip) {
			//Draw the ship
			glColor3fv(g->ship.color);
			glPushMatrix();
			glTranslatef(g->ship.pos[0], g->ship.pos[1], g->ship.pos[2]);
			//float angle = atan2(ship.dir[1], ship.dir[0]);
			glRotatef(g->ship.angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex2f(-10.0f, -10.0f);
			glVertex2f(  0.0f, 20.0f);
			glVertex2f( 10.0f, -10.0f);
			glVertex2f(-12.0f, -10.0f);
			glVertex2f(  0.0f, 20.0f);
			glVertex2f(  0.0f, -6.0f);
			glVertex2f(  0.0f, -6.0f);
			glVertex2f(  0.0f, 20.0f);
			glVertex2f( 12.0f, -10.0f);
			glEnd();
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(0.0f, 0.0f);
			glEnd();
			glPopMatrix();
			if (keys[XK_Up] || g->mouseThrustOn) {
			int i;
			//draw thrust
			Flt rad = ((g->ship.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			Flt xs,ys,xe,ye,r;
			glBegin(GL_LINES);
			for (i=0; i<16; i++) {
				xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
				ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
				r = rnd()*40.0+40.0;
				xe = -xdir * r + rnd() * 18.0 - 9.0;
				ye = -ydir * r + rnd() * 18.0 - 9.0;
				glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
				glVertex2f(g->ship.pos[0]+xs,g->ship.pos[1]+ys);
				glVertex2f(g->ship.pos[0]+xe,g->ship.pos[1]+ye);
			}
			glEnd();
			}
		}
	    //-------------------------------------------------------------------------
	    //Draw the asteroids
	    {
		Asteroid *a = g->ahead;
		while (a) {
		    //Log("draw asteroid...\n");
		    glColor3fv(a->color);
		    glPushMatrix();
		    glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
		    glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
		    glBegin(GL_LINE_LOOP);
		    //Log("%i verts\n",a->nverts);
		    for (int j=0; j<a->nverts; j++) {
			glVertex2f(a->vert[j][0], a->vert[j][1]);
		    }
		    glEnd();
		    //glBegin(GL_LINES);
		    //	glVertex2f(0,   0);
		    //	glVertex2f(a->s.radius, 0);
		    //glEnd();
		    glPopMatrix();
		    glColor3f(1.0f, 0.0f, 0.0f);
		    glBegin(GL_POINTS);
		    glVertex2f(a->pos[0], a->pos[1]);
		    glEnd();
		    a = a->next;
		}
	    }
	    //-------------------------------------------------------------------------
	    //Draw the bullets
	    for (int i=0; i<g->nbullets; i++) {
		Bullet *b = &g->barr[i];
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
	    }

	    //draw abrahams menu
	    if (credits == 1) {
		showCredits(xres,yres,r);
	    }
	    // Draw Nick's Menu
	    if (nick_menu == 1) {
		drawNicholasMenu(xres, yres, r);
	    }
	    // Draw Pat's Menu
	    if (pat_menu == 1) {
		UpgradeMenu(xres, yres, r);
	    }
	    // Draw Ramon's Menu
	    if (ramon_menu == 1) {
		drawRamRMenu(xres, yres, r);
	    }
	}
