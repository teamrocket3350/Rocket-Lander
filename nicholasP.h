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

typedef float Vec[3];

struct Shape {
    float width, height;
    float radius;
    Vec center;
};

class Object
{
	protected:
		Vec pos;
		Shape shape;

	public:
		// Constructor/Destructor
		Object();
		~Object();

		// Setters
		void setPosX(float x) {pos[0]=x;}
		void setPosY(float y) {pos[1]=y;}
		void setWidth(float w) {shape.width=w;}
		void setHeight(float h) {shape.height=h;}
		void setRadius(float r) {shape.radius=r;}

		// Getters
		float getPosX() {return pos[0];}
		float getPosY() {return pos[1];}
		float getWidth() {return shape.width;}
		float getHeight() {return shape.height;}
		float getRadius() {return shape.radius;}

		// Pure virtual functions
		virtual void draw() = 0;
};

class MovableObject : public Object
{
	protected:
		Vec vel;

	public:
		// Constructor
		MovableObject();

		// Setters
		void setVelX(float x) {vel[0]=x;}
		void setVelY(float y) {vel[1]=y;}

		// Getters
		float getVelX() {return vel[0];}
		float getVelY() {return vel[1];}

		// Pure virtual functions
		virtual void move() = 0;
};

// Rename to Ship when integrating into rocketlander.cpp
class Ship2 : public MovableObject {
	private:
		Shape collidables[5];
	public:
		// Constructor
		Ship2();

		void move();
		bool collidesWith();
		void draw();
		void drawRect();
		void drawTri();
};

//class Enemy : public MovableObject {
//	protected:
//		int health;
//
//	public:
//		Enemy();
//		void move();
//		void draw();
//};
//
//class Chaser : public Enemy {
//	protected:
//		Ship * ship;
//
//	public:
//		Chaser(Ship*);
//		void move();
//		void draw();
//};
//
//class Circler : public Enemy {
//	protected:
//		Ship * ship;
//
//	public:
//		Circler(Ship*);
//		void move();
//		void draw();
//};
//
class Asteroid2 : public MovableObject {
//class Asteroid : public MovableObject {
	public:
		void move();
		void draw();
};

class Platform : public Object
{
    public:
	void draw();
	//void draw() override;
};

//class Level {
//	protected:
//		float gravity;
//		float time;
//		int score;
//		Ship ship;
//		Enemy enemies[5];
//		Asteroid asteroids[10];
//		Platform platforms[100];
//		int enemyCount;
//		int asteroidCount;
//
//		void loadLevel(char[]);
//		void moveObjects();
//		void removeEnemy(int);
//		void addEnemy(Enemy);
//		void addAsteroid(Asteroid);
//		void removeAsteroid(int);
//		int calculateFinalScore();
//		bool objectiveComplete();
//
//	public:
//		void init_level();
//		void runCycle();
//		Level(char[]);
//};

void drawNicholasMenu(int xres, int yres, Rect r);
