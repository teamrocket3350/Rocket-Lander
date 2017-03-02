// Abraham Aldana
// teamrocket3350
// Rocket-Lander
// abrahamA.cpp

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<ctime>
#include<cmath>
#include<X11/Xlib.h>
#include<X11/keysym.h>
#include<GL/glx.h>
#include"log.h"
#include"fonts.h"

void menuAbraham(int xres, int yres, Rect r);

int loadLevel();

int loadShip();

void save(int level, int ship);
