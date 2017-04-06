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

using namespace std;

void showCredits(int xres, int yres, Rect r)
{
	glColor3f(0.5,0.5,0.5);
	int cx = xres/2;
	int cy = yres/2;
	glBegin(GL_QUADS);
		glVertex2f(cx-620, cy+420);
		glVertex2f(cx+620, cy+420);
		glVertex2f(cx+620, cy-420);
		glVertex2f(cx-620, cy-420);
	glEnd();
	glEnable(GL_TEXTURE_2D);

	r.bot = cy + 300;
	r.left = cx;
	r.center = 1;
	ggprint8b(&r,16, 0xffffff, "Game Credits");

	r.bot = cy + 200;
	r.left = cx;
	r.center = 1;
	ggprint8b(&r,16, 0xffffff, "Abraham Aldana");
	
	r.bot = cy + 150;
	r.left = cx;
	r.center = 1;
	ggprint8b(&r,16, 0xffffff, "Nicholas P");
	
	r.bot = cy + 100;
	r.left = cx;
	r.center = 1;
	ggprint8b(&r,16, 0xffffff, "Patrick E");
	
	r.bot = cy + 50;
	r.left = cx;
	r.center = 1;
	ggprint8b(&r,16, 0xffffff, "Ramon R");
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

Level::Level(char levelFile[])
{
        score = 0;
        loadLevel(levelFile);

        // Keep track of number of enemies/asteroids (use in add/remove funcs)
        enemyCount = 0; 
        asteroidCount = 0;
        platformCount = 0;
}

void Level::loadLevel(char levelFile[]) // Reads file and contains switch case to load objects
{
    printf("%s\n", levelFile);
}

void Level::moveObjects()  // Calls move() for all MovableObject (ship/enemies/asteroids)
{

}

void Level::addChaser(float posx, float posy, int health)
{

}

void Level::addCircler(float posx, float posy, int health)
{

}

void Level::removeEnemy(int index)
{

}

void Level::addAsteroid(float posx, float posy, float velx, float vely, float radius)
{

}

void Level::removeAsteroid(int index)
{

}

void Level::addPlatform(float posx, float posy, float width, float height)
{

}

void Level::addGoalPlatform(float posx, float posy, float width, float height)
{

}

void Level::addRefuelPlatform(float posx, float posy, float width, float height)
{

}

int Level::calculateFinalScore()
{
    return 0;
} // ?

bool Level::objectiveComplete()  // Check if objective has been complete
{
    return false;
}

void Level::init_level()  // Run any initialization code
{

}

void Level::runCycle()  // Run a single cycle of the game
{

}

*/
