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



void UpgradeMenu(int xres,int yres,Rect r)
{
    glColor3f(1,0,0);
    int cx = xres/2;
    int cy = yres/2;
    glBegin(GL_QUADS);
    glVertex2i(cx-400,cy+200);
    glVertex2i(cx+400,cy+200);
    glVertex2i(cx+100,cy-200);
    glVertex2i(cx-100,cy-200);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot=cy+20;
    r.left=cx;
    r.center=1;
    ggprint8b(&r,16,0,"Upgrades");    
}
