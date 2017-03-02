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
#include"abrahamA.h"

void menuAbraham(int xres, int yres, Rect r)
{
	glColor3f(1.0,1.0,1.0);
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
	ggprint8b(&r,16, 0x00000000, "Abraham Menu");
    
}

int loadLevel()
{
    	int level;
	ifstream load;
	load.open ("./save_file.txt");
		if (load.is_open()) {
			cout << "\nSave file loaded successfully\n";
			load >> level;
			cout << "Level - " << level << "\n";
		}
		else {
			cout << "\nCould not open save file\n";
			level = 1;
			cout << "Level - 1\n";	
		}
	load.close();
	return (level);	
}

int loadShip()
{
    	int ship;
	char buffer[3];
	ifstream load;
	load.open ("./save_file.txt");
		if (load.is_open()) {
			cout << "Loaded successfully\n";
			load.getline(buffer,3);
			load >> ship;	  
			cout << "Ship - " << ship << "\n\n";
		}
		else {
			cout << "Could not open save file\n";
			ship = 1;
			cout << "Ship - 1\n\n";	
		}

	load.close();
	return (ship);	
}

void save(int level, int ship)
{
	ofstream save;
	save.open ("./save_file.txt");
		save << level << "\n"; 
		save << ship;
	save.close();
}
/*
int main() {
    int level, ship;
    level = loadLevel();
    ship = loadShip();
    return 0;
}
*/
