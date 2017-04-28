#ifndef nicholasP
#define nicholasP

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

typedef float Vec[3];

struct Shape {
    // width + height = rectangle
    // base + height = triangle
    // radius = circle
    float width;
    float height;
    float base;
    float radius;
    Vec center;
};

struct Point {
	float x;
	float y;
};

struct Line {
	Point p1;
	Point p2;
};

class Object
{
	protected:
		Vec pos;
		float rot; // rotation
		Shape shape;

	public:
		// Constructor/Destructor
		Object();
		~Object();

		// Setters
		void setPosX(float x);
		void setPosY(float y);
		void setRot(float r);
		void setWidth(float w);
		void setHeight(float h);
		void setRadius(float r);

		// Getters
		float getPosX();
		float getPosY();
		float getRot();
		float getWidth();
		float getHeight();
		float getRadius();

		void draw();
};

class MovableObject : public Object
{
	protected:
		Vec vel;

	public:
		// Constructor
		MovableObject();
		
		// Setters
		void setVelX(float x);
		void setVelY(float y);

		// Getters
		float getVelX();
		float getVelY();

		void move();
};

class Goal : public Object
{
	private:
		Object trigger;

    public:
		//Goal();
		//Goal(float, float, float, float);

		void setPosX(float x);
		void setPosY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void draw();

		Object getTrigger() {return trigger;}
};

class Fueler : public Object
{
	private:
		float fuel;
		Object trigger;

    public:
		Fueler();

		void setPosX(float x);
		void setPosY(float y);
		void setWidth(float w);
		void setHeight(float h);

		float getFuelLeft();
		void removeFuel();
		void draw();

		Object getTrigger() {return trigger;}
};

// Rename to Ship when integrating into rocketlander.cpp
class Ship2 : public MovableObject {
//class Ship : public MovableObject {
	private:
		Shape collidables[5];
		float fuel;
		float fuelMax;

		// Choose between the 3
		bool haveBooster1;
		bool enabledBooster1;
		bool haveBooster2;
		bool enabledBooster2;
		bool haveBooster3;
		bool enabledBooster3;
		
		bool haveLeftStrafe;
		bool haveRightStrafe;
		bool haveDoubleShot;
		bool haveTripleShot;

		bool linesIntersect(Line, Line);
		Point* getRectPointArray(float, float, float, float, float);
		Point* getTriPointArray(float, float, float, float, float);
		bool rectCollidesWith(Shape, Object, float, float);
		bool triCollidesWith(Shape, Object, float, float);

	public:
		// Constructor
		Ship2();

		void move();
		bool goalTriggered(Goal);
		bool fuelerTriggered(Fueler);
		bool collidesWith(Object);
		void draw();
		void draw_debug();

		void addFuel();
		float getFuelLeft();
		float getFuelMax();

		bool buyBooster1();
		bool buyBooster2();
		bool buyBooster3();

		bool ownBooster1(); 
		bool ownBooster2();
		bool ownBooster3();

		void enableBooster1();
		void enableBooster2();
		void enableBooster3();

		void accelerate();
		void addGravity(float);

		void rotateLeft();
		void rotateRight();
};

class Enemy : public MovableObject {
	protected:
		int health;

	public:
		Enemy();
		void move();
		void draw();
};

class Chaser : public Enemy {
	private:
		Ship2 * ship;

	public:
		//Chaser(Ship*);
		void move();
		void draw();
};

class Circler : public Enemy {
	private:
		Ship2 * ship;

	public:
		//Circler(Ship*);
		void move();
		void draw();
};

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
};

void drawFuelGauge(float, float, float, float);
void init_ship_image();
void drawNicholasMenu(int xres, int yres, Rect r);

#endif
