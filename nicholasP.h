#ifndef nicholasP
#define nicholasP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctime>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "ppm.h"

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
		void setPosX(float x);
		void setPosY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void draw();
		void draw_debug();

		Object getTrigger() {return trigger;}

		// Image
		Ppmimage * image;
		GLuint texture;
		GLuint silhouette;
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

		float getFuelLeft() {return fuel;}
		void removeFuel();
		void draw();
		void draw_debug();

		Object getTrigger() {return trigger;}

		// Image
		Ppmimage * image;
		GLuint texture;
		GLuint silhouette;
};

class Ship : public MovableObject {
	private:
		Shape collidables[5];
		float fuel;
		float fuelMax;

		bool watchGoal;
		bool watchFuel;
		bool exploded;

		// Choose between the 3
		bool haveBooster1;
		bool haveBooster2;
		bool haveBooster3;
		
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
		Ship();

		void move();
		bool goalTriggered(Goal);
		bool fuelerTriggered(Fueler);
		bool collidesWith(Object);
		bool shipExploded() {return exploded;}
		void reset() {exploded=false;fuel=100;}
		void draw();
		void draw_debug();

		void addFuel();
		void resetFuel();
		float getFuelLeft() {return fuel;}
		float getFuelMax() {return fuelMax;}

		bool buyBooster1();
		bool buyBooster2();
		bool buyBooster3();

		bool ownBooster1() {return haveBooster1;} 
		bool ownBooster2() {return haveBooster2;}
		bool ownBooster3() {return haveBooster3;}

		// True if that booster type is enabled
		bool enabledBooster1;
		bool enabledBooster2;
		bool enabledBooster3;

		// Function to enable booster
		void enableBooster1();
		void enableBooster2();
		void enableBooster3();

		void accelerate();
		void addGravity(float);

		void rotateLeft();
		void rotateRight();

		// Image
		Ppmimage * image;
		GLuint texture;
		GLuint silhouette;
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
		Ship * ship;

	public:
		//Chaser(Ship*);
		void move();
		void draw();
};

class Circler : public Enemy {
	private:
		Ship * ship;

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
		void draw_debug();

		// Image
		Ppmimage * image;
		GLuint texture;
};

void drawFuelGauge(float, float, float, float);
void init_alpha_image(char * imagePath, Ppmimage * image,
		GLuint * texture, GLuint * silhouette);
void init_image(char * imagePath, Ppmimage * image, GLuint * texture);
void drawNicholasMenu(int xres, int yres, Rect r);

#endif
