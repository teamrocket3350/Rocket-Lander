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

void drawRamRMenu(int xres, int yres, Rect r)
{
    glColor3f(0.5, 0.0, 0.0);
    int cx = xres/2;
    int cy = yres/2;
    glBegin(GL_QUADS);
	glVertex2f(cx-100, cy+100);
	glVertex2f(cx+100, cy+100);
	glVertex2f(cx+100, cy-100);
	glVertex2f(cx-100, cy-100);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot = cy + 20;
    r.left = cx;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ffff00, "This is Ramon's menu");
}


