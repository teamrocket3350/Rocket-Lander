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

// Get this from rocketlander.cpp
typedef float Vec[3];

struct Shape {
        float width, height;
        float radius;
        Vec center;
};

// Objects such as: platforms
class Object
{
        private:
                Vec pos;
                Shape shape;
                
        public:
                Object()
                {
                        pos[0] = 0;
                        pos[1] = 0;
                        shape.width = 0.0;
                        shape.height = 0.0;
                        shape.radius = 0.0;
                        shape.center[0] = 0.0;
                        shape.center[1] = 0.0;
                }

		~Object()
		{

		}

		virtual void draw() = 0;
};

// Objects such as: player, enemies, asteroids
class MovableObject : public Object
{
        private:
                Vec vel;
                
        public:
                MovableObject()
                {
                        vel[0] = 0;
                        vel[1] = 0;
                }

                virtual void move() = 0;
};

class Ship : public MovableObject {

        public:
                void move() override {
                    printf("The ship is moving!\n");
                }

                bool collidesWith() {
                    printf("The ship is colliding!\n");
                    return true;
                }

                void draw() override {
                    printf("Drawing the ship!\n");
                }

};

class Enemy : public MovableObject {

        private:
                int health;
                Ship * ship;

        public:
                Enemy(Ship * s)
                {
                        health = 100;
                        ship = s;
                }

                void move() override {
                    printf("The enemy is moving!\n");
                }

                void draw() override {
                    printf("Drawing the enemy!\n");
                }
};

class Asteroid : public MovableObject {

        public:
                void move() override {
                    printf("The asteroid is moving!\n");
                }

                void draw() override {
                    printf("Drawing the asteroid!\n");
                }
};

class Platform : public Object {

	public:
		void draw() override {
			printf("Drawing the platform!\n");
		}
};

class Level {
	private:
		float gravity;
		float time; // Use whatever type is needed
		int score;
		Ship * ship;
		//Enemy enemies[10](s); // handled in constructor
		Asteroid asteroids[10]; // handle enemies like bullets in asteroid framework
		Platform platforms[100]; // This can be a normal array
		//Objective objective; // Don't know how to handle this yet
		/*
		 * Other variables
		 * that are needed
		 */

		// Functions:
		void loadLevel() {} // Reads xml file and contains switch case to load objects
		void moveObjects() {} // Calls move() for all MovableObject (ship/enemies/asteroids)
		void removeEnemy(int index) {}
		void addEnemy(Enemy enemy) {}
		void addAsteroid(Asteroid ast) {}
		void removeAsteroid(int index) {}
		int calculateFinalScore() {return 0;}
		bool objectiveComplete() {return false;} // Check if objective has been complete

	public:
		void init_level() {} // Run any initialization code
		void runCycle() {} // Run a single cycle of the game

		Level(Ship * s)
		{
			time = 60.0;
			score = 0;
			ship = s;
			Enemy enemies[10](s); // handle enemies like bullets in asteroid framework
			loadLevel();
		}
//
//		~Level()
//		{
//
//		}

};

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

        //Asteroid ast;
        //ast.move();
	//ast.draw();

	//Platform plat;
	//plat.draw();

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
}
