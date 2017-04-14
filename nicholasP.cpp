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

#define PI 3.14159265
int tempcount = 0;

Object::Object()
{
	pos[0] = 0;
	pos[1] = 0;
	shape.width = 0.0;
	shape.height = 0.0;
	shape.base = 0.0;
	shape.radius = 0.0;
	shape.center[0] = 0.0;
	shape.center[1] = 0.0;
}

Object::~Object()
{

}

// ---------- //

MovableObject::MovableObject()
{
	vel[0] = 0;
	vel[1] = 0;
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

	fuel = 100;
}

void Ship2::move()
{
	//printf("The ship is moving!\n");
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
		return true;
	} else {
		return false;
	}
}

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

// x, y are offsets from 0,0
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

void Ship2::draw()
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

void Ship2::drawRect()
{
	//draw platform
	glColor3ub(111,111,111);
	glPushMatrix();
	glBegin(GL_QUADS);

	//glVertex2i(0,0);
	//glVertex2i(0, shape.height);
	//glVertex2i(shape.width, shape.height);
	//glVertex2i(shape.width, 0);

	glEnd();
	glPopMatrix();
}

void Ship2::drawTri()
{
	glColor3ub(255,0,0);
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	//glVertex2i(0,0);
	//glVertex2i(shape.base, 0);
	//glVertex2i(shape.base\2, shape.height);

	glEnd();
	glPopMatrix();
}

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

void Ship2::addGravity(float grav)
{
	printf("VelY: %f\n", vel[1]);
	vel[1] -= grav;
	if (vel[1] < -2.0)
		vel[1] = -2.0;
	if (vel[1] > 4.0)
		vel[1] = 4.0;
}

// ---------- //

Enemy::Enemy()
{
	health = 100;
}

void Enemy::move() {
	printf("The enemy is moving!\n");
}

void Enemy::draw() {
	printf("Drawing the enemy!\n");
}

// ---------- //

//Chaser::Chaser(Ship * s)
//{
//    ship = s;
//}

void Chaser::move() {
	printf("The chaser is moving!\n");
}

void Chaser::draw() {
	printf("Drawing the chaser!\n");
}

// ---------- //

//Circler::Circler(Ship * s)
//{
//    ship = s;
//}

void Circler::move() 
{
	printf("The circler is moving!\n");
}

void Circler::draw() 
{
	printf("Drawing the circler!\n");
}

// ---------- //

void Asteroid2::move()
{
	printf("The asteroid is moving!\n");
}

void Asteroid2::draw()
{
	printf("Drawing the asteroid!\n");
}

// ---------- //

void Platform::draw()
{
	//draw platform
	glColor3ub(255,165,0);
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

void drawNicholasMenu(int xres, int yres, Rect r)
{   

	//
	//    // Test circle collision
	//    Platform plat2;
	//    plat2.setHeight(10);
	//
	//    // Test circle collision
	//    Platform plat3;
	//    plat3.setRadius(10);

	Ship2 ship;
	ship.setPosX(400);
	ship.setPosY(10);
	
	// Test Rectangle collision
	Platform plat;
	plat.setPosX(440);
	plat.setPosY(100);
	plat.setWidth(25);
	plat.setHeight(25);

	ship.draw();
	plat.draw();
	ship.collidesWith(plat);

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
