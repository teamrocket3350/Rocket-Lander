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
#define PI 3.141592653589793
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
int pat_menu = 1;	// change to 1 to turn on Main Menu
int ramon_menu = 0;

Ppmimage * backgroundImage=NULL;
Ppmimage *bg_image=NULL;
Ppmimage *bgc_image=NULL;
Ppmimage *platform_image=NULL;
GLuint backgroundTexture;
GLuint bg_texture;
GLuint bgc_texture;
GLuint platform_texture;

struct Game {
    int curLevel;
    levelData level; //Abraham's level loading
    Ship ship; // Nick's ship class
    Platform ground;
    Platform plats[100]; // Nick's platform class
    Goal goal;
    Fueler fueler;
    Game() {
        curLevel = 1;
        level = loadLevel(curLevel); //Abraham's level loading

        ship.enableBooster2();
        ship.setPosX(level.rocket.x);
        ship.setPosY(level.rocket.y);
        //	ship.setPosX(53);
        //	ship.setPosY(32);

        ground.setPosX(0);
        ground.setPosY(0);
        ground.setWidth(1250);
        ground.setHeight(32);

        for (int i = 0; i < level.platformCount; i++) { 
            printf("Plat %d x: %d\n", i+1, level.platform.x[i]);
            printf("Plat %d y: %d\n", i+1, level.platform.y[i]);
            plats[i].setPosX(level.platform.x[i]);
            plats[i].setPosY(level.platform.y[i]);
            plats[i].setWidth(100);
            plats[i].setHeight(32);
        }

        //	plats[0].setPosX(100);
        //	plats[0].setPosY(600);
        //	plats[0].setWidth(100);
        //	plats[0].setHeight(32);

        //	plats[1].setPosX(550);
        //	plats[1].setPosY(440);
        //	plats[1].setWidth(100);
        //	plats[1].setHeight(32);

        goal.setPosX(level.goalX);
        goal.setPosY(level.goalY);
        goal.setWidth(100);
        goal.setHeight(27);

        //	goal.setPosX(950);
        //	goal.setPosY(740);
        //	goal.setWidth(100);
        //	goal.setHeight(27);

        fueler.setPosX(850);
        fueler.setPosY(340);
        fueler.setWidth(100);
        fueler.setHeight(27);
    }
};

int keys[65536];

//function prototypes
void initXWindows(void);
void init_opengl(Game *g);
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
void changeLevel(Game *g, int level);

int main(void)
{
    logOpen();
    initXWindows();
    imageConvert();
#ifdef USE_OPENAL_SOUND
    startUpSound();		// from Patrick to add sound
    init_sounds();
#endif //end openal sound
    Game game;
    init_opengl(&game);
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

void init_opengl(Game *g)
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

    // Initialize Hitter image
    inHitters(); // Initialize hitter image

    // Initialize rocket image
    init_alpha_image((char *)"./images/RocketFinal.ppm",
            g->ship.image, &g->ship.texture, &g->ship.silhouette);

    // Initialize platform images
    init_image((char *)"./images/ground.ppm",
            g->ground.image, &g->ground.texture);
    for (int i = 0; i < g->level.platformCount; i++)
        init_image((char *)"./images/platform.ppm",
                g->plats[i].image, &g->plats[i].texture);

    // Initialize goal image
    init_alpha_image((char *)"./images/goal.ppm",
            g->goal.image, &g->goal.texture, &g->goal.silhouette);

    // Initialize refuel image
    init_alpha_image((char *)"./images/refuel.ppm",
            g->fueler.image, &g->fueler.texture, &g->fueler.silhouette);

    // Initialize background image
    init_image((char *)"./images/background.ppm",
            backgroundImage, &backgroundTexture);

    platform_image = ppm6GetImage("./images/platform.ppm");	
    glGenTextures(1, &platform_texture);
    glBindTexture(GL_TEXTURE_2D, platform_texture);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, platform_image->width, platform_image->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, platform_image->data);

    bg_image = ppm6GetImage("./images/background2.ppm");	
    glGenTextures(1, &bg_texture);
    glBindTexture(GL_TEXTURE_2D, bg_texture);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bg_image->width, bg_image->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, bg_image->data);

    bgc_image = ppm6GetImage("./images/background3.ppm");	
    glGenTextures(1, &bgc_texture);
    glBindTexture(GL_TEXTURE_2D, bgc_texture);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bgc_image->width, bgc_image->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, bgc_image->data);


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

void changeLevel(Game *g, int level) {
    if (level > 5) {
#ifdef USE_OPENAL_SOUND
        stopSound(p.alSourceAstroid);
        playSound(p.alSourceVictory);
        sleep(5);
#endif
        pat_menu = 1;
        credits = 1;
        level = 0;
        g->curLevel = 0;
    } else {
        g->level = loadLevel(level); //Abraham's level loading
#ifdef USE_OPENAL_SOUND
        stopSound(p.alSourceAstroid);	//stops bgm
        playSound(p.alSourceVictory);	//Victory
        sleep(5);
        playSound(p.alSourceAstroid);	//bgm
#endif //end openal sound

        g->ship.enableBooster2();
        g->ship.setPosX(g->level.rocket.x);
        g->ship.setPosY(g->level.rocket.y);
        g->ship.resetFuel();

        for (int i = 0; i < g->level.platformCount; i++) { 
            g->plats[i].setPosX(g->level.platform.x[i]);
            g->plats[i].setPosY(g->level.platform.y[i]);
            g->plats[i].setWidth(100);
            g->plats[i].setHeight(32);
        }

        g->goal.setPosX(g->level.goalX);
        g->goal.setPosY(g->level.goalY);
        g->goal.setWidth(100);
        g->goal.setHeight(27);

        g->fueler.setPosX(850);
        g->fueler.setPosY(340);
        g->fueler.setWidth(100);
        g->fueler.setHeight(27);

        init_opengl(g);
    }
}

void physics(Game *g)
{
    g->ship.addGravity(GRAVITY);
    g->ship.move();

    // Check for collision with platforms
    g->ship.collidesWith(g->ground);
    for (int i=0; i<g->level.platformCount; i++) {
        g->ship.collidesWith(g->plats[i]);
    }

    // Check for collision with goal platform
    g->ship.collidesWith(g->goal);
    if (g->ship.goalTriggered(g->goal))
        changeLevel(g, ++g->curLevel);

    // Check for collision with fueler platform
    g->ship.collidesWith(g->fueler);
    // If fueler is triggered and it has fuel left
    if (g->ship.fuelerTriggered(g->fueler) && g->fueler.getFuelLeft() > 1) {
        // Do not overfill the ship
        if (!(g->ship.getFuelLeft() > g->ship.getFuelMax()-1)) {
            g->fueler.removeFuel();
            g->ship.addFuel();
        }
    }

    if (g->ship.getPosX() < 0.0) {
        g->ship.setPosX(g->ship.getPosX() + (float)xres);
    }
    else if (g->ship.getPosX() > (float)xres) {
        g->ship.setPosX(g->ship.getPosX() - (float)xres);
    }
    //---------------------------------------------------
    //check keys pressed now
    if (keys[XK_Left]) {
        g->ship.rotateLeft();

    }
    if (keys[XK_Right]) {
        g->ship.rotateRight();
    }
    if (keys[XK_Up]) {
#ifdef USE_OPENAL_SOUND
        playSound(p.alSourceBooster);	//Booster
#endif //end openal sound
        g->ship.accelerate();
    }

    if (g->ship.shipExploded()) {
        g->ship.setPosX(g->level.rocket.x);
        g->ship.setPosY(g->level.rocket.y);
        g->ship.setVelX(0);
        g->ship.setVelY(0);
        g->ship.setRot(0);
        printf("Ship exploded!\n");
#ifdef USE_OPENAL_SOUND
        playSound(p.alSourceCollide);	//Collide
#endif //end openal sound
        g->ship.reset();
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
    g->ship.draw();

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
    drawFuelGauge(g->ship.getFuelLeft(), g->ship.getFuelMax(), xres*.5, 850);
    ggprint8b(&fuelBar, 16, 0x00ff0000, "Fuel");

    g->ground.draw();
    for (int i = 0; i < g->level.platformCount; i++)
        g->plats[i].draw();

    g->goal.draw();
    g->fueler.draw();

    menus(r);
}
