#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "nicholasP.h"

//// Get this from rocketlander.cpp
//typedef float Vec[3];
//
////extern struct Shape;
//struct Shape {
//    float width, height;
//    float radius;
//    Vec center;
//};

//// Objects such as: platforms
//class Object
//{
////    private:
////	Vec pos;
////	Shape shape;
//
//    public:
//	Vec pos;
//	Shape shape;
//
//	Object()
//	{
//	    pos[0] = 0;
//	    pos[1] = 0;
//	    shape.width = 0.0;
//	    shape.height = 0.0;
//	    shape.radius = 0.0;
//	    shape.center[0] = 0.0;
//	    shape.center[1] = 0.0;
//	}
//
//	~Object()
//	{
//
//	}
//
//	virtual void draw() = 0;
//};

Object::Object()
{
    pos[0] = 0;
    pos[1] = 0;
    shape.width = 0.0;
    shape.height = 0.0;
    shape.radius = 0.0;
    shape.center[0] = 0.0;
    shape.center[1] = 0.0;
}

Object::~Object()
{

}


//// Objects such as: player, enemies, asteroids
//class MovableObject : public Object
//{
//    private:
//	Vec vel;
//
//    public:
//	MovableObject()
//	{
//	    vel[0] = 0;
//	    vel[1] = 0;
//	}
//
//	virtual void move() = 0;
//};

MovableObject::MovableObject()
{
    vel[0] = 0;
    vel[1] = 0;
}


//class Ship : public MovableObject {
//
//    private:
//	// Choose between the 3
//	bool haveBooster1;
//	bool enabledBooster1;
//	bool haveBooster2;
//	bool enabledBooster2;
//	bool haveBooster3;
//	bool enabledBooster3;
//
//	bool haveLeftStrafe;
//	bool haveRightStrafe;
//	bool haveDoubleShot;
//	bool haveTripleShot;
//
//    public:
//	void move() override {
//	    printf("The ship is moving!\n");
//	}
//
//	bool collidesWith() {
//	    printf("The ship is colliding!\n");
//	    return true;
//	}
//
//	void draw() override {
//	    printf("Drawing the ship!\n");
//	}
//
//	// Funcs to interface with Patrick's menus
//	void buyBooster1()
//	{
//	    haveBooster1 = true;
//	}	    
//	void buyBooster2()
//	{
//	    haveBooster2 = true;
//	}	    
//	void buyBooster3()
//	{
//	    haveBooster3 = true;
//	}	    
//	void buyLeftStrafe()
//	{
//	    haveLeftStrafe = true;
//	}	    
//	void buyRightStrafe()
//	{
//	    haveRightStrafe = true;
//	}	    
//	void buyDoubleShot()
//	{
//	    haveDoubleShot = true;
//	}	    
//	void buyTripleShot()
//	{
//	    haveTripleShot = true;
//	}	    
//
//	bool ownBooster1()
//	{
//	    return haveBooster1;
//	}	    
//	bool ownBooster2()
//	{
//	    return haveBooster2;
//	}	    
//	bool ownBooster3()
//	{
//	    return haveBooster3;
//	}	    
//	bool ownLeftStrafe()
//	{
//	    return haveLeftStrafe;
//	}	    
//	bool ownRightStrafe()
//	{
//	    return haveRightStrafe;
//	}	    
//	bool ownDoubleShot()
//	{
//	    return haveDoubleShot;
//	}	    
//	bool ownTripleShot()
//	{
//	    return haveTripleShot;
//	}	    
//
//	void enableBooster1()
//	{
//	    enabledBooster2 = false;
//	    enabledBooster3 = false;
//	    enabledBooster1 = true;
//	}
//
//	void enableBooster2()
//	{
//	    enabledBooster1 = false;
//	    enabledBooster3 = false;
//	    enabledBooster2 = true;
//	}
//
//	void enableBooster3()
//	{
//	    enabledBooster1 = false;
//	    enabledBooster2 = false;
//	    enabledBooster3 = true;
//	}
//};

Ship2::Ship2()
{
	// Cockpit
	collidables[0].width = 25;
	collidables[0].height = 25;

	// Left wing
	collidables[1].width = 20;
	collidables[1].height = 20;

	// Right wing
	collidables[2].width = 20;
	collidables[2].height = 20;

	// Nose
	collidables[3].width = 20;
	collidables[3].height = 20;

	// Nose Cone
	collidables[4].width = 20;
	collidables[4].height = 20;

	shape.width = collidables[0].width + collidables[1].width + collidables[2].width;
	shape.height = collidables[0].height + collidables[3].height + collidables[4].height;
	// Figure out how to set positions
}

void Ship2::move()
{
    printf("The ship is moving!\n");
}

bool Ship2::collidesWith()
{
    printf("The ship is colliding!\n");
    return true;
}

void Ship2::draw()
{
    //draw ship box
    glColor3ub(0,0,255);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
    glBegin(GL_QUADS);

//    // Coords using center pos
//    glVertex2i(-shape.width,-shape.height);
//    glVertex2i(-shape.width, shape.height);
//    glVertex2i(shape.width, shape.height);
//    glVertex2i(shape.width, -shape.height);

    // Coords using (0,0) pos
    glVertex2i(0,0);
    glVertex2i(0, shape.height);
    glVertex2i(shape.width, shape.height);
    glVertex2i(shape.width, 0);



    glEnd();
    glPopMatrix();

    printf("Drawing the ship!\n");
}

void Ship2::drawRect()
{
    //draw platform
    glColor3ub(255,165,0);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
    glBegin(GL_QUADS);

    glVertex2i(-shape.width,-shape.height);
    glVertex2i(-shape.width, shape.height);
    glVertex2i(shape.width, shape.height);
    glVertex2i(shape.width, -shape.height);

    glEnd();
    glPopMatrix();
}


//class Enemy : public MovableObject {
//
//    private:
//	int health;
//	//Ship * ship;
//
//    public:
//	Enemy()
//	    //Enemy(Ship * s)
//	{
//	    health = 100;
//	    //ship = s;
//	}
//
//	void move() override {
//	    printf("The enemy is moving!\n");
//	}
//
//	void draw() override {
//	    printf("Drawing the enemy!\n");
//	}
//};

//Enemy::Enemy()
//    //Enemy(Ship * s)
//{
//    health = 100;
//    //ship = s;
//}
//
//void Enemy::move() override {
//    printf("The enemy is moving!\n");
//}
//
//void Enemy::draw() override {
//    printf("Drawing the enemy!\n");
//}

//class Chaser : public Enemy {
//
//    private:
//	Ship * ship;
//
//    public:
//	Chaser(Ship * s)
//	{
//	    ship = s;
//	}
//
//	void move() override {
//	    printf("The chaser is moving!\n");
//	}
//
//	void draw() override {
//	    printf("Drawing the chaser!\n");
//	}
//};

//Chaser::Chaser(Ship * s)
//{
//    ship = s;
//}
//
//void Chaser::move() override {
//    printf("The chaser is moving!\n");
//}
//
//void Chaser::draw() override {
//    printf("Drawing the chaser!\n");
//}

//class Circler : public Enemy {
//
//    private:
//	Ship * ship;
//
//    public:
//	Circler(Ship * s)
//	{
//	    ship = s;
//	}
//
//	void move() override
//	{
//	    printf("The circler is moving!\n");
//	}
//
//	void draw() override
//	{
//	    printf("Drawing the circler!\n");
//	}
//};

//Circler::Circler(Ship * s)
//{
//    ship = s;
//}
//
//void Circler::move() override
//{
//    printf("The circler is moving!\n");
//}
//
//void Circler::draw() override
//{
//    printf("Drawing the circler!\n");
//}

//class Asteroid : public MovableObject {
//
//    public:
//	void move() override
//	{
//	    printf("The asteroid is moving!\n");
//	}
//
//	void draw() override
//	{
//	    printf("Drawing the asteroid!\n");
//	}
//};

//void Asteroid::move() override
//{
//    printf("The asteroid is moving!\n");
//}
//
//void Asteroid::draw() override
//{
//    printf("Drawing the asteroid!\n");
//}

//class Platform : public Object {
//
//    public:
//	void draw() override
//	{
//	    //draw platform
//	    glColor3ub(255,165,0);
//	    glPushMatrix();
//	    glTranslatef(pos[0], pos[1], 0);
//	    glBegin(GL_QUADS);
//
//	    glVertex2i(-shape.width,-shape.height);
//	    glVertex2i(-shape.width, shape.height);
//	    glVertex2i(shape.width, shape.height);
//	    glVertex2i(shape.width, -shape.height);
//
////	    glVertex2i(-shape.width,-shape.height);
////	    glVertex2i(-shape.width, shape.height);
////	    glVertex2i(shape.width, shape.height);
////	    glVertex2i(shape.width, -shape.height);
//	    glEnd();
//	    glPopMatrix();
//
//	    printf("Drawing the platform!\n");
//	}
//};

void Platform::draw()
{
    //draw platform
    glColor3ub(255,165,0);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
    glBegin(GL_QUADS);

//    // Coords using center pos
//    glVertex2i(-shape.width,-shape.height);
//    glVertex2i(-shape.width, shape.height);
//    glVertex2i(shape.width, shape.height);
//    glVertex2i(shape.width, -shape.height);

    // Coords using (0,0) pos
    glVertex2i(0,0);
    glVertex2i(0, shape.height);
    glVertex2i(shape.width, shape.height);
    glVertex2i(shape.width, 0);

    glEnd();
    glPopMatrix();
}

//	class Level {
//	    private:
//		float gravity;
//		float time; // Use whatever type is needed
//		int score;
//		Ship ship;
//		Enemy enemies[5]; // This will be changed when we add enemy types
//		Asteroid asteroids[10]; // handle enemies like bullets in asteroid framework
//		Platform platforms[100]; // This can be a normal array
//
//		// Keep track of number of enemies/asteroids (use in add/remove funcs)
//		int enemyCount = 0;
//		int asteroidCount = 0;
//		//Objective objective; // Don't know how to handle this yet
//		/*
//		 * Other variables
//		 * that are needed
//		 */
//
//		// Functions:
//		void loadLevel(char levelFile[]) {printf("%s\n", levelFile);} // Reads xml file and contains switch case to load objects
//		void moveObjects() {} // Calls move() for all MovableObject (ship/enemies/asteroids)
//		void removeEnemy(int index) {}
//		void addEnemy(Enemy enemy) {}
//		void addAsteroid(Asteroid ast) {}
//		void removeAsteroid(int index) {}
//		int calculateFinalScore() {return 0;} // ?
//		bool objectiveComplete() {return false;} // Check if objective has been complete
//
//	    public:
//		void init_level() {} // Run any initialization code
//		void runCycle() {} // Run a single cycle of the game
//
//		Level(char levelFile[])
//		{
//		    score = 0;
//		    loadLevel(levelFile);
//		}
//	};

//void Level::loadLevel(char levelFile[]) {printf("%s\n", levelFile);}
//void Level::moveObjects() {} // Calls move() for all MovableObject (ship/enemies/asteroids)
//void Level::removeEnemy(int index) {}
//void Level::addEnemy(Enemy enemy) {}
//void Level::addAsteroid(Asteroid ast) {}
//void Level::removeAsteroid(int index) {}
//int Level::calculateFinalScore() {return 0;} // ?
//bool Level::objectiveComplete() {return false;} // Check if objective has been complete
//
//void Level::init_level() {} // Run any initialization code
//void Level::runCycle() {} // Run a single cycle of the game
//
//Level::Level(char levelFile[])
//{
//	score = 0;
//	loadLevel(levelFile);
//}

	void drawNicholasMenu(int xres, int yres, Rect r)
	{
	    //Ship ship;
	    //ship.move();
	    //ship.collidesWith();
	    //ship.draw();

	    //Ship ship;
	    //Enemy enemy(&ship);
	    //enemy.move();
	    //enemy.draw();

	    //Ship ship;
	    //Chaser e1(&ship);
	    //e1.move();
	    //e1.draw();

	    //Ship ship;
	    //Circler e2(&ship);
	    //e2.move();
	    //e2.draw();

	    //Asteroid ast;
	    //ast.move();
	    //ast.draw();

	    //Platform plat;
	    //plat.draw();

	    //Level level1((char*)"This is a test");
	    //level1.enemies[0] = e1;
	    //level1.enemies[0].move();
	    //level1.enemies[0].draw();

	    glColor3f(1.0, 0.0, 0.0);
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
	    ggprint8b(&r, 16, 0x00ffff00, "This is Nick's menu");
	    ggprint8b(&r, 16, 0x00ffff00, "Debugging!");
	}
