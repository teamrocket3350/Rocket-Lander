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

	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glVertex2f(cx-160, cy+300);
		glVertex2f(cx+160, cy+300);
		glVertex2f(cx+160, cy-295);
		glVertex2f(cx-160, cy-295);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	
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
	//clean up all images in master folder
	system("rm ./images/*.ppm");
	
	//convert images to ppm
	system("mogrify -format ppm ./images/*.png");
	system("mogrify -format ppm ./images/*.jpg");
}

void imageClean()
{
	//clean up all images in master folder
	system("rm *.ppm");
	return;
}

int getData(int count, int flag)
{
	if (flag == 0) {
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
	else if (flag == 1){
		int level;
		ifstream load;
		load.open ("./level_1.txt");
		if (load.is_open()) {
			if (count == 1) {
				cout << "\nLevel 1 has been loaded successfully.\n\n";
			}
			for (int i = 0; i < count; i++) {
				load >> level;
			}
		}
		else {
			cout << "\nCould not open save file\n";
			level = 1;
			cout << "Default level - 1\n";
		}
		load.close();
		return (level);
	}
	else if (flag == 2){
		int level;
		ifstream load;
		load.open ("./level_2.txt");
		if (load.is_open()) {
			if (count == 1) {
				cout << "\nLevel 2 has been loaded successfully.\n\n";
			}
			for (int i = 0; i < count; i++) {
				load >> level;
			}
		}
		else {
			cout << "\nCould not open save file\n";
			level = 1;
			cout << "Default level - 1\n";
		}
		load.close();
		return (level);
	}
	else if (flag == 3){
		int level;
		ifstream load;
		load.open ("./level_3.txt");
		if (load.is_open()) {
			if (count == 1) {
				cout << "\nLevel 3 has been loaded successfully.\n\n";
			}
			for (int i = 0; i < count; i++) {
				load >> level;
			}
		}
		else {
			cout << "\nCould not open save file\n";
			level = 1;
			cout << "Default level - 1\n";
		}
		load.close();
		return (level);
	}
	else {
		level = 1;
		cout << "Error\n";
		return (level);
	}
}

void putData(int data, int flag)
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
	int parCount = 1;

	cout << "\nLoading the Save File. Do not touch the Memory Card in Slot A or the Power Button...\n";

	data.levelNumber = getData(parCount,0);
	cout << "Level - " << data.levelNumber << endl;
	parCount++;

	data.score = getData(parCount,0);
	cout << "Score - " << data.score << endl;
	parCount++;

	data.rocket.type = getData(parCount,0);
	cout << "Rocket - " << data.rocket.type << endl;
	parCount++;

	data.rocket.x = getData(parCount,0);
	cout << "Rocket x - " << data.rocket.x << endl;
	parCount++;

	data.rocket.y = getData(parCount,0);
	cout << "Rocket y - " << data.rocket.y << endl;
	parCount++;

	data.asteroidCount = getData(parCount,0);
	cout << "Asteroids - " << data.asteroidCount << endl;
	parCount++;

	data.enemyCount = getData(parCount,0);
	cout << "Enemies - " << data.enemyCount << endl;
	parCount++;

	data.gravity = getData(parCount,0);
	cout << "Gravity - " << data.gravity << endl;
	parCount++;

	data.time = getData(parCount,0);
	cout << "time - " << data.time << endl << endl;
	parCount++;

	data.platformCount = getData(parCount,0);
	cout << "Platforms - " << data.platformCount << endl;
	parCount++;

	for (int i = 0; i < data.platformCount; i++) {
		data.platform.x[i] = getData(parCount,0);
		cout << "Platform" << i+1 << " x - " << data.platform.x[i] << endl;
		parCount++;

		data.platform.y[i] = getData(parCount,0);
		cout << "Platform" << i+1 << " y - " << data.platform.y[i] << endl;
		parCount++;
	}

	return data;
}

void saveGame(saveData data)
{
	cout << "Saving... Do not touch the Memory Card in Slot A or the Power Button.\n";
	putData(data.levelNumber,1);
	putData(data.score,0);
	putData(data.rocket.type,0);
	putData(data.rocket.x,0);
	putData(data.rocket.y,0);
	putData(data.asteroidCount,0);
	putData(data.enemyCount,0);
	putData(data.gravity,0);
	putData(data.time,0);
	putData(data.platformCount,0);
	for (int i = 0; i < data.platformCount; i++) {
		putData(data.platform.x[i],0);
		putData(data.platform.y[i],0);
	}
	cout << "\nYour progress was saved.\n\n";
}

levelData loadLevel(levelData level, int levelNumber)
{
	int parCount = 1;

	cout << "\nLoading the Level. Do not touch the Memory Card in Slot A or the Power Button...\n";

	level.levelNumber = getData(parCount, levelNumber);
	cout << "Level - " << level.levelNumber << endl;
	parCount++;

	level.rocket.x = getData(parCount, levelNumber);
	cout << "Rocket x - " << level.rocket.x << endl;
	parCount++;

	level.rocket.y = getData(parCount, levelNumber);
	cout << "Rocket y - " << level.rocket.y << endl;
	parCount++;

	level.asteroidCount = getData(parCount, levelNumber);
	cout << "Asteroids - " << level.asteroidCount << endl;
	parCount++;

	level.enemyCount = getData(parCount, levelNumber);
	cout << "Enemies - " << level.enemyCount << endl;
	parCount++;

	level.gravity = getData(parCount, levelNumber);
	cout << "Gravity - " << level.gravity << endl;
	parCount++;

	level.time = getData(parCount, levelNumber);
	cout << "time - " << level.time << endl << endl;
	parCount++;

	level.platformCount = getData(parCount, levelNumber);
	cout << "Platforms - " << level.platformCount << endl;
	parCount++;

	for (int i = 0; i < level.platformCount; i++) {
		level.platform.x[i] = getData(parCount, levelNumber);
		cout << "Platform" << i+1 << " x - " << level.platform.x[i] << endl;
		parCount++;

		level.platform.y[i] = getData(parCount, levelNumber);
		cout << "Platform" << i+1 << " y - " << level.platform.y[i] << endl;
		parCount++;
	}
	return level;
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
