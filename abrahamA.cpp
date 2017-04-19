// Abraham Aldana
// teamrocket3350
// Rocket-Lander
// abrahamA.cpp

// load game data from file and save back to it
// game credits
// convert images to ppm
// clean images

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
	glColor3f(0.1,0.1,0.1);
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
	ggprint40(&r,16, 0xffffff, "Game Credits");

	r.bot = cy + 200;
	r.left = cx;
	r.center = 1;
	ggprint16(&r,16, 0xffffff, "Abraham Aldana");
	
	r.bot = cy + 150;
	r.left = cx;
	r.center = 1;
	ggprint16(&r,16, 0xffffff, "Nicholas P");
	
	r.bot = cy + 100;
	r.left = cx;
	r.center = 1;
	ggprint16(&r,16, 0xffffff, "Patrick E");
	
	r.bot = cy + 50;
	r.left = cx;
	r.center = 1;
	ggprint16(&r,16, 0xffffff, "Ramon R");
}

void imageConvert()
{
	//copy images to main
	system("cp ./images/*.png .");
	system("cp ./images/*.jpg .");
	
	//convert images to ppm
	system("mogrify -format ppm *.png");
	system("mogrify -format ppm *.jpg");
}

void imageClean()
{
	//clean up all images in master folder
	system("rm *.ppm");
	system("rm *.png");
	system("rm *.jpg");
	return;
}

int getData(int count)
{
	int data;

	ifstream load;
	load.open ("./saveFile.txt");
	if (load.is_open()) {
		if (count == 1) {
			cout << "\nThe Save File has been loaded successfully.\n\n";
		}
		for (int i = 0; i < count; i++) {
			load >> data;
		}
	}
	else {
		cout << "\nCould not open save file\n";
		data = 1;
		cout << "Default data - 1\n";
	}
	load.close();
	return (data);
}

void putData(float data, int flag)
{
	ofstream save;
	if (flag == 1) {
		save.open ("./saveFile.txt");
		save << data << "\n";
	}
	else {
		save.open ("./saveFile.txt", fstream::out | fstream::app);
		save << data << "\n";
	}
	save.close();
}

saveData loadGame(saveData data)
{
	cout << "\nLoading the Save File. Do not touch the Memory Card in Slot A or the Power Button...\n";
    	data.levelNumber = getData(1);
	cout << "Level - " << data.levelNumber << endl;
	data.score = getData(2);
	cout << "Score - " << data.score << endl;
	data.rocket = getData(3);
	cout << "Rocket - " << data.rocket << endl;
	data.platformCount = getData(4);
	cout << "Platforms - " << data.platformCount << endl;
	data.asteroidCount = getData(5);
	cout << "Asteroids - " << data.asteroidCount << endl;
	data.enemyCount = getData(6);
	cout << "Enemies - " << data.enemyCount << endl;
	data.gravity = getData(7);
	cout << "Gravity - " << data.gravity << endl;
	data.time = getData(8);
	cout << "time - " << data.time << endl << endl;

	return data;
}

void saveGame(saveData data)
{
	cout << "Saving... Do not touch the Memory Card in Slot A or the Power Button.\n";
	putData(data.levelNumber,1);
	putData(data.score,0);
	putData(data.rocket,0);
	putData(data.platformCount,0);
	putData(data.asteroidCount,0);
	putData(data.enemyCount,0);
	putData(data.gravity,0);
	putData(data.time,0);
	cout << "\nYour progress was saved.\n\n";
}


/*

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
