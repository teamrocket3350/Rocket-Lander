/* Name: Nick Polach
 *
 * Description:
 * My part of the project deals with the ship and the simple objects that
 * it interacts with (platforms/goal). In the main file, my ship object is
 * provided with other objects in the level and it checks if it collides
 * with those objects. My file also provides functions to draw UI items
 * that give info about the ship such as the fuel gauge.
 *
 */

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
#include "ppm.h"
#include "nicholasP.h"
#include "ramonR.h" // Temp

#define PI 3.14159265
int tempcount = 0;

Ppmimage * shipImage;
GLuint shipSilhouetteTexture;
GLuint shipTexture;


// Object with basic properties of shape
Object::Object()
{
	pos[0] = 0;
	pos[1] = 0;
	shape.width = 0.0;
	shape.height = 0.0;
	shape.base = 0.0;
	shape.radius = 0.0;
}

// Setters
void Object::setPosX(float x)
{
	pos[0]=x;
}

void Object::setPosY(float y)
{
	pos[1]=y;
}

void Object::setRot(float r)
{
	rot=r;
}

void Object::setWidth(float w)
{
	shape.width=w;
}

void Object::setHeight(float h)
{
	shape.height=h;
}

void Object::setRadius(float r)
{
	shape.radius=r;
}

// Getters
float Object::getPosX()
{
	return pos[0];
}

float Object::getPosY()
{
	return pos[1];
}

float Object::getRot()
{
	return rot;
}

float Object::getWidth()
{
	return shape.width;
}

float Object::getHeight()
{
	return shape.height;
}

float Object::getRadius()
{
	return shape.radius;
}

Object::~Object()
{

}

// ---------- //

// Object with basic properties for movable shape
MovableObject::MovableObject()
{
	vel[0] = 0;
	vel[1] = 0;
}

// Setters
void MovableObject::setVelX(float x)
{
	vel[0]=x;
}

void MovableObject::setVelY(float y)
{
	vel[1]=y;
}

// Getters
float MovableObject::getVelX()
{
	return vel[0];
}

float MovableObject::getVelY()
{
	return vel[1];
}

// ---------- //

Ship2::Ship2()
{
	// Cockpit
	collidables[0].width = 26;
	collidables[0].height = 26;

	// Left wing
	//collidables[1].base = 40;
	collidables[1].base = 26;
	collidables[1].height = 26;

	// Right wing
	//collidables[2].base = 40;
	collidables[2].base = 26;
	collidables[2].height = 26;

	// Nose
	collidables[3].width = 20;
	collidables[3].height = 20;

	// Nose Cone
	collidables[4].base = 26;
	collidables[4].height = 20;

	shape.width = collidables[0].width + collidables[1].base*.5 + collidables[2].base*.5;
	shape.height = collidables[0].height + collidables[3].height + collidables[4].height;

	printf("%f %f\n", shape.width, shape.height);
	fuel = 100;
	fuelMax = 100;
}

void Ship2::move()
{
	pos[0] += vel[0];
	pos[1] += vel[1];

}

// Checks if goal was triggered
bool Ship2::goalTriggered(Goal goal)
{
	if (collidesWith(goal.getTrigger()) &&
			vel[0] == 0 &&
			vel[1] == 0) {
		//printf("Goal triggered!\n");
		return true;
	}
	return false;
}

bool Ship2::fuelerTriggered(Fueler fueler)
{
	if (collidesWith(fueler.getTrigger()) &&
			vel[0] == 0 &&
			vel[1] == 0) {
		//printf("Fueler triggered!\n");
		return true;
	}
	return false;
}


bool Ship2::collidesWith(Object ob)
{
	// TODO Collision types:
	// ^Rectangle w/ Rectangle 	// Ship w/ platform
	// Rectangle w/ circle 		// Ship w/ asteroid
	// ^Triangle w/ rectangle 	// Ship w/ platform
	// Triangle w/ circle 		// Ship w/ asteroid

	// Use line-line collision testing for all shapes except cirlce
	if (
			triCollidesWith(collidables[1], ob, pos[0], pos[1]) || // left wing
			rectCollidesWith(collidables[0], ob, pos[0]+(collidables[1].base*.5), pos[1]) || //cockpit
			triCollidesWith(collidables[2], ob, pos[0]+collidables[1].base, pos[1]) || // right wing
			rectCollidesWith(collidables[3], ob, pos[0]+(collidables[1].base*.5)+3, pos[1]+collidables[0].height) || // nose box
			triCollidesWith(collidables[4], ob, pos[0]+(collidables[1].base*.5), pos[1]+collidables[0].height+collidables[3].height) // nose cone
	   ) {
		// Place ship above platform
		if (pos[1] < ob.getPosY() + ob.getHeight()) {
			pos[1] = ob.getPosY() + ob.getHeight();
			vel[0] = 0; // Temp
			vel[1] = 0;
			if (rot > 45)
				rot += 2;
			else if (rot < -45)
				rot -= 2;
			else if (rot > 0)
				rot -= 2;
			else if (rot < 0)
				rot += 2;

			// Prevents rot from going over float limit
			if (rot > 360)
				rot -= 360;
			else if (rot < -360)
				rot += 360;
		}
		return true;
	} else {
		return false;
	}
}

// Checks whether to given lines intersect
bool Ship2::linesIntersect(Line l1, Line l2)
{
	if (l1.p1.x <= l2.p2.x && 
			l1.p2.x >= l2.p1.x &&
			l1.p1.y <= l2.p2.y &&
			l1.p2.y >= l2.p1.y) {
		return true;
	}
	return false;
}

// Generates an array of points that create the square with the given parameters
Point * Ship2::getRectPointArray(float x, float y, float rot, float width, float height)
{
	// p2 -> p3 
	// ^     |
	// |     V
	// p1 <- p4

	//TODO p3->p4 not checked?
	Point * pts = new Point[4];

	pts[0].x = x;
	pts[0].y = y;

	pts[1].x = x;
	pts[1].y = y+height;

	pts[2].x = x+width;
	pts[2].y = y+height;

	pts[3].x = x+width;
	pts[3].y = y;

	// Convert rotation from degrees to radians
	float rad = rot * (PI/180);

	// Pivot points
	float pivx = pos[0]+(shape.width*.5);
	float pivy = pos[1]+(shape.height*.5);

	// Rotate points
	float xnew;
	float ynew;
	for (int i=0; i<4; i++) {
		pts[i].x -= pivx;
		pts[i].y -= pivy;

		xnew = pts[i].x * cos(rad) - pts[i].y * sin(rad);
		ynew = pts[i].x * sin(rad) + pts[i].y * cos(rad);

		pts[i].x = xnew + pivx;
		pts[i].y = ynew + pivy;
	}

	return pts;
}


// Generates an array of points that create the triangle with the given parameters
Point * Ship2::getTriPointArray(float x, float y, float rot, float base, float height)
{
	//    p2  
	// ^     |
	// |     V
	// p1 <- p3

	//TODO p2->p3 not checked?
	Point * pts = new Point[3];

	pts[0].x = x;
	pts[0].y = y;

	pts[1].x = x+(base*.5);
	pts[1].y = y+height;

	pts[2].x = x+base;
	pts[2].y = y;

	// Convert rotation from degrees to radians
	float rad = rot * (PI/180);

	// Pivot points
	float pivx = pos[0]+(shape.width*.5);
	float pivy = pos[1]+(shape.height*.5);

	// Rotate points
	float xnew;
	float ynew;
	for (int i=0; i<3; i++) {
		pts[i].x -= pivx;
		pts[i].y -= pivy;

		xnew = pts[i].x * cos(rad) - pts[i].y * sin(rad);
		ynew = pts[i].x * sin(rad) + pts[i].y * cos(rad);

		pts[i].x = xnew + pivx;
		pts[i].y = ynew + pivy;
	}

	return pts;
}

// Checks if any of the rectangular collidables of the ship collide with the given object
bool Ship2::rectCollidesWith(Shape collidable, Object ob, float x, float y)
{
	bool collides = false;
	if (ob.getWidth() > 0) {			// rectangle
		Point * pts1 = getRectPointArray(x, y, rot, collidable.width, collidable.height);
		Point * pts2 = getRectPointArray(ob.getPosX(), ob.getPosY(), ob.getRot(), ob.getWidth(), ob.getHeight());
		Line l1;
		Line l2;
		// Check each line in ship against 4 lines in rectangle 
		for (int i=0; i<4; i++) {
			l1.p1 = pts1[i];
			l1.p2 = pts1[(i+1)%4];
			for (int i2=0; i2<4; i2++) {
				l2.p1 = pts2[i2];
				l2.p2 = pts2[(i2+1)%4];
				if (linesIntersect(l1, l2)) {
					printf("%i. Rectangle collide with rectangle\n", tempcount++);
					collides = true;
					break;
				}
			}
		}
		delete [] pts1;
		delete [] pts2;
	} else if (ob.getRadius() > 0) { 		// circle
		// Rectangle collide with circle
		printf("Rectangle collide with circle\n");
	} else {
		printf("The object has no shape\n");
	}
	return collides;
}

// Checks if any of the triangular collidables of the ship collide with the given object
bool Ship2::triCollidesWith(Shape collidable, Object ob, float x, float y)
{
	bool collides = false;
	if (ob.getWidth() > 0) {			// rectangle
		Point * pts1 = getTriPointArray(x, y, rot, collidable.base, collidable.height);
		Point * pts2 = getRectPointArray(ob.getPosX(), ob.getPosY(), ob.getRot(), ob.getWidth(), ob.getHeight());
		Line l1;
		Line l2;
		// Check each line in ship against 3 lines in triangle
		for (int i=0; i<3; i++) {
			l1.p1 = pts1[i];
			l1.p2 = pts1[(i+1)%3];
			for (int i2=0; i2<4; i2++) {
				l2.p1 = pts2[i2];
				l2.p2 = pts2[(i2+1)%4];
				if (linesIntersect(l1, l2)) {
					printf("%i. Triangle collide with rectangle\n", tempcount++);
					collides = true;
					break;
				}
			}
		}
		delete [] pts1;
		delete [] pts2;
	} else if (ob.getRadius() > 0) { 		// circle
		printf("Triangle collide with circle\n");
	} else {
		printf("The object has no shape\n");
	}
	return collides;
}

// Draw the collidable pieces of the ship to the screen
// Used mainly for debugging
void Ship2::draw_debug()
{
	Point * pts;
	// The area for the ship image
	pts = getRectPointArray(pos[0], pos[1], rot, shape.width, shape.height);
	glColor3ub(111,111,111);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex2i(pts[0].x, pts[0].y);
	glVertex2i(pts[1].x, pts[1].y);
	glVertex2i(pts[2].x, pts[2].y);
	glVertex2i(pts[3].x, pts[3].y);

	glEnd();
	glPopMatrix();
	delete [] pts;
	pts = NULL;
	////////////////////////
	// Visualization of the collision shapes
	//draw cockpit box
	pts = getRectPointArray(pos[0]+(collidables[1].base*.5), pos[1], rot, collidables[0].width, collidables[0].height);
	glColor3ub(255,0,0);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex2i(pts[0].x, pts[0].y);
	glVertex2i(pts[1].x, pts[1].y);
	glVertex2i(pts[2].x, pts[2].y);
	glVertex2i(pts[3].x, pts[3].y);

	glEnd();
	glPopMatrix();
	delete [] pts;
	pts = NULL;
	////////////////////////
	// draw left wing
	pts = getTriPointArray(pos[0], pos[1], rot, collidables[1].base, collidables[1].height);
	glColor3ub(0,0,255);
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	glVertex2i(pts[0].x, pts[0].y);
	glVertex2i(pts[1].x, pts[1].y);
	glVertex2i(pts[2].x, pts[2].y);

	glEnd();
	glPopMatrix();
	delete [] pts;
	pts = NULL;
	////////////////////////
	// draw right wing
	pts = getTriPointArray(pos[0]+collidables[1].base, pos[1], rot, collidables[2].base, collidables[2].height);
	glColor3ub(0,0,255);
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	glVertex2i(pts[0].x, pts[0].y);
	glVertex2i(pts[1].x, pts[1].y);
	glVertex2i(pts[2].x, pts[2].y);

	glEnd();
	glPopMatrix();
	delete [] pts;
	pts = NULL;
	////////////////////////
	//draw nose box
	pts = getRectPointArray(pos[0]+(collidables[1].base*.5)+3, pos[1]+collidables[0].height, rot, collidables[3].width, collidables[3].height);
	glColor3ub(255,0,0);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex2i(pts[0].x, pts[0].y);
	glVertex2i(pts[1].x, pts[1].y);
	glVertex2i(pts[2].x, pts[2].y);
	glVertex2i(pts[3].x, pts[3].y);

	glEnd();
	glPopMatrix();
	delete [] pts;
	pts = NULL;
	////////////////////////
	// draw nose cone
	pts = getTriPointArray(pos[0]+(collidables[1].base*.5), pos[1]+collidables[0].height+collidables[3].height, rot, collidables[4].base, collidables[4].height);
	glColor3ub(0,0,255);
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	glVertex2i(pts[0].x, pts[0].y);
	glVertex2i(pts[1].x, pts[1].y);
	glVertex2i(pts[2].x, pts[2].y);

	glEnd();
	glPopMatrix();
	delete [] pts;
	pts = NULL;
}


void Ship2::draw()
{
	Point * pts;
	pts = getRectPointArray(pos[0], pos[1], rot, shape.width, shape.height);
	// Ship image
	glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, shipSilhouetteTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f); //Alpha
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(pts[0].x, pts[0].y);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(pts[1].x, pts[1].y); 
	glTexCoord2f(1.0f, 0.0f); glVertex2i(pts[2].x, pts[2].y);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(pts[3].x, pts[3].y);

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);

	delete [] pts;
	pts = NULL;
}

///// Swaps enabled booster /////
void Ship2::enableBooster1()
{
	enabledBooster2 = false;
	enabledBooster3 = false;
	enabledBooster1 = true;
}

void Ship2::enableBooster2()
{
	enabledBooster1 = false;
	enabledBooster3 = false;
	enabledBooster2 = true;
}

void Ship2::enableBooster3()
{
	enabledBooster1 = false;
	enabledBooster2 = false;
	enabledBooster3 = true;
}
///////////////////////////////

// Changes velocity to move ship in direction that it's pointing
void Ship2::accelerate()
{
	if (fuel > 0) {
		float rad = (rot+90.0) / 360.0f * PI * 2.0;
		float xdir = cos(rad);
		float ydir = sin(rad);
		if (enabledBooster1) {
			vel[0] += xdir * 0.02f;
			vel[1] += ydir * 0.02f;
		} else if (enabledBooster2) {
			vel[0] += xdir * 0.025f;
			vel[1] += ydir * 0.025f;
		} else if (enabledBooster3) {
			vel[0] += xdir * 0.03f;
			vel[1] += ydir * 0.03f;
		}
		fuel -= .2;
	} else {
		fuel = 0;
	}
}

// Add gravity to move ship to the bottom of the window
void Ship2::addGravity(float grav)
{
	//printf("VelY: %f\n", vel[1]);
	vel[1] -= grav;
	// Prevent infinite downward acceleration
	if (vel[1] < -2.0)
		vel[1] = -2.0;

	// Prevent infinite upward acceleration
	if (vel[1] > 4.0)
		vel[1] = 4.0;
}

void Ship2::rotateLeft()
{
	rot += 4.0;

	// Prevents rot from going over float limit
	if (rot > 360)
		rot -= 360;
}

void Ship2::rotateRight()
{
	rot -= 4.0;
	// Prevents rot from going under float limit
	if (rot < -360)
		rot += 360;
}

bool Ship2::ownBooster1()
{
	return haveBooster1;
}

bool Ship2::ownBooster2()
{
	return haveBooster2;
}

bool Ship2::ownBooster3()
{
	return haveBooster3;
}

float Ship2::getFuelLeft()
{
	return fuel;
}

float Ship2::getFuelMax()
{
	return fuelMax;
}

void Ship2::addFuel()
{
	fuel += 1;
}


// ---------- //

// Basic platform
void Platform::draw()
{
	//draw platform
	//glColor3ub(255,165,0);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glBegin(GL_QUADS);

	// Coords using (0,0) pos
	glVertex2i(0,0);
	glVertex2i(0, shape.height);
	glVertex2i(shape.width, shape.height);
	glVertex2i(shape.width, 0);

	glEnd();
	glPopMatrix();
}

// ---------- //

// Platform with goal trigger
//Goal::Goal(float x, float y, float width, float height)
//{
//	pos[0] = x;
//	pos[1] = y;
//	shape.width = width;
//	shape.height = height;
//
//	trigger.setPosX(x);
//	trigger.setPosY(y);
//	trigger.setWidth(width);
//	trigger.setHeight(height+5);
//}

void Goal::setPosX(float x)
{
	pos[0]=x;
	trigger.setPosX(x);
}

void Goal::setPosY(float y)
{
	pos[1]=y;
	trigger.setPosY(y);
}

void Goal::setWidth(float w)
{
	shape.width=w;
	trigger.setWidth(w);
}

void Goal::setHeight(float h)
{
	shape.height=h;
	trigger.setHeight(h+5);
}

void Goal::draw()
{
	//draw trigger 
	glColor3ub(255,111,111);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glBegin(GL_QUADS);

	// Coords using (0,0) pos
	glVertex2i(0,0);
	glVertex2i(0, trigger.getHeight());
	glVertex2i(trigger.getWidth(), trigger.getHeight());
	glVertex2i(trigger.getWidth(), 0);

	glEnd();
	glPopMatrix();

	//draw platform
	glColor3ub(111,111,111);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glBegin(GL_QUADS);

	// Coords using (0,0) pos
	glVertex2i(0,0);
	glVertex2i(0, shape.height);
	glVertex2i(shape.width, shape.height);
	glVertex2i(shape.width, 0);

	glEnd();
	glPopMatrix();
}

// ---------- //

Fueler::Fueler()
{
	fuel = 100;
}

void Fueler::setPosX(float x)
{
	pos[0]=x;
	trigger.setPosX(x);
}

void Fueler::setPosY(float y)
{
	pos[1]=y;
	trigger.setPosY(y);
}

void Fueler::setWidth(float w)
{
	shape.width=w;
	trigger.setWidth(w);
}

void Fueler::setHeight(float h)
{
	shape.height=h;
	trigger.setHeight(h+5);
}

float Fueler::getFuelLeft()
{
	return fuel;
}

void Fueler::removeFuel()
{
	fuel -= 1;
}

void Fueler::draw()
{
	//draw trigger
	glColor3ub(255,255,255);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glBegin(GL_QUADS);

	// Coords using (0,0) pos
	glVertex2i(0,0);
	glVertex2i(0, trigger.getHeight());
	glVertex2i(trigger.getWidth(), trigger.getHeight());
	glVertex2i(trigger.getWidth(), 0);

	glEnd();
	glPopMatrix();

	//draw platform
	glColor3ub(111,111,111);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glBegin(GL_QUADS);

	// Coords using (0,0) pos
	glVertex2i(0,0);
	glVertex2i(0, shape.height);
	glVertex2i(shape.width, shape.height);
	glVertex2i(shape.width, 0);

	glEnd();
	glPopMatrix();
}

// ---------- //

void drawFuelGauge(float fuelLeft, float fuelMax, float x, float y)
{
	float width = 155;
	float height = 25;
	float fuelWidth = ((fuelLeft)/fuelMax)*width-8;
	//outer outline platform
	glColor3ub(255,0,0);
	glPushMatrix();
	glTranslatef(x-(width*.5), y, 0);
	glBegin(GL_QUADS);

	glVertex2i(0,0);
	glVertex2i(0, height);
	glVertex2i(width, height);
	glVertex2i(width, 0);

	glEnd();
	glPopMatrix();

	// inner outline
	glColor3ub(0,0,0);
	glPushMatrix();
	glTranslatef(x-(width*.5)+2, y+2, 0);
	glBegin(GL_QUADS);

	glVertex2i(0,0);
	glVertex2i(0, height-4);
	glVertex2i(width-4, height-4);
	glVertex2i(width-4, 0);

	glEnd();
	glPopMatrix();

	if (fuelWidth > 0) {
		// fuel gauge
		glColor3ub(0,255,0);
		glPushMatrix();
		glTranslatef(x-(width*.5)+4, y+4, 0);
		glBegin(GL_QUADS);

		glVertex2i(0,0);
		glVertex2i(0, height-8);
		glVertex2i(fuelWidth, height-8);
		glVertex2i(fuelWidth, 0);

		glEnd();
		glPopMatrix();
	}
}

void init_ship_image()
{
	shipImage = ppm6GetImage("./images/RocketFinal.ppm");
	glGenTextures(1, &shipSilhouetteTexture);
	glGenTextures(1, &shipTexture);

	// Ship
	glBindTexture(GL_TEXTURE_2D,shipTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		shipImage->width, shipImage->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, shipImage->data);

	// Alpha
	glBindTexture(GL_TEXTURE_2D,shipSilhouetteTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *silhouetteData = buildAlphaData(shipImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		shipImage->width, shipImage->height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
}
