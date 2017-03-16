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
	public:
		Vec pos;
		Shape shape;

		Object();
		~Object();
		virtual void draw() = 0;
};

class MovableObject : public Object
{
	private:
		Vec vel;

	public:
		MovableObject();
		virtual void move() = 0;
};

//class Ship : public MovableObject {
//	public:
//		void move();
//		bool collidesWith();
//		void draw();
//};
//
//class Enemy : public MovableObject {
//	private:
//		int health;
//
//	public:
//		Enemy();
//		void move();
//		void draw();
//};
//
//class Chaser : public Enemy {
//	private:
//		Ship * ship;
//
//	public:
//		Chaser(Ship*);
//		void move();
//		void draw();
//};
//
//class Circler : public Enemy {
//	private:
//		Ship * ship;
//
//	public:
//		Circler(Ship*);
//		void move();
//		void draw();
//};
//
class Asteroid2 : public MovableObject {
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
//	private:
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
