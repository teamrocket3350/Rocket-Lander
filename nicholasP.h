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
//#include <GL/gl.h>
//#include <GL/glu.h>
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
    Vec center; // Remove after integrate with rocketlander.cpp
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
		void setPosX(float x) {pos[0]=x;}
		void setPosY(float y) {pos[1]=y;}
		void setRot(float r) {rot=r;}
		void setWidth(float w) {shape.width=w;}
		void setHeight(float h) {shape.height=h;}
		void setRadius(float r) {shape.radius=r;}

		// Getters
		float getPosX() {return pos[0];}
		float getPosY() {return pos[1];}
		float getRot() {return rot;}
		float getWidth() {return shape.width;}
		float getHeight() {return shape.height;}
		float getRadius() {return shape.radius;}

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
		void setVelX(float x) {vel[0]=x;}
		void setVelY(float y) {vel[1]=y;}

		// Getters
		float getVelX() {return vel[0];}
		float getVelY() {return vel[1];}

		void move();
};

class Goal : public Object
{
	private:
		Object trigger;

    public:
		Goal(float, float, float, float);
		void draw();

		Object getTrigger() {return trigger;}
};

// Rename to Ship when integrating into rocketlander.cpp
class Ship2 : public MovableObject {
//class Ship : public MovableObject {
	private:
		Shape collidables[5];
		float fuel = 100;
		float fuelMax = 100;

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
		bool collidesWith(Object);
		void draw();
		void drawRect(); // remove?
		void drawTri(); // remove?

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

		float getFuelLeft() {return fuel;}
		float getFuelMax() {return fuelMax;}
		bool buyBooster1();
		bool buyBooster2();
		bool buyBooster3();

		bool ownBooster1() {return haveBooster1;}
		bool ownBooster2() {return haveBooster2;}
		bool ownBooster3() {return haveBooster3;}

		void enableBooster1();
		void enableBooster2();
		void enableBooster3();

		void accelerate();
		void addGravity(float);
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
void drawNicholasMenu(int xres, int yres, Rect r);

#endif
