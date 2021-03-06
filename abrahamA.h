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
#include"nicholasP.h"

struct aRocket {
	int type;
	int x;
	int y;
};

struct aPlatform {
	int x[100];
	int y[100];
};

struct saveData {
	int levelNumber;
	int score;
	aRocket rocket;
	int asteroidCount;
	int enemyCount;
	float gravity;
	float time;
	int platformCount;
	aPlatform platform;
};

struct levelData {
	int levelNumber;
	aRocket rocket;
	int asteroidCount;
	int enemyCount;
	float gravity;
	float time;
	int goalX;
	int goalY;
	int platformCount;
	aPlatform platform;
};

void showCredits(int xres, int yres, Rect r);

void imageConvert();

void imageClean();

int getData(int, int);

void putData(int, int);

saveData loadGame(saveData);

void saveGame(saveData);

levelData loadLevel(int);


class Level {
        private:
                float gravity;
                float time; // Use whatever type is needed
                int score;
                Ship ship;
                Enemy enemies[5]; // This will be changed when we add enemy types
                Asteroid2 asteroids[10]; // handle enemies like bullets in asteroid framework
                Platform platforms[100]; // This can be a normal array
                int enemyCount;
                int asteroidCount;
                int platformCount;
                //Objective objective; // Don't know how to handle this yet

                void loadLevel(char[]);
                void moveObjects();
                void addChaser(float, float, int); // might not use health
                void addCircler(float, float, int); // might not use health
                void removeEnemy(int);
                void addAsteroid(float, float, float, float, float);
                void removeAsteroid(int);
                void addPlatform(float, float, float, float);
                void addGoalPlatform(float, float, float, float); // sets isGoal to true
                void addRefuelPlatform(float, float, float, float); // sets isRefuel to true
                int calculateFinalScore();
                bool objectiveComplete();

        public:
                void init_level();
                void runCycle();
                Level(char[]);
};
