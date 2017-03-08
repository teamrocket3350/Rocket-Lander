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


class Object
{
        private:
                Vec pos;
                Vec vel;
                Shape shape;
                
        public:
                Object()
                {
                        pos[0] = 0;
                        pos[1] = 0;
                        vel[0] = 0;
                        vel[1] = 0;
                        shape.width = 0.0;
                        shape.height = 0.0;
                        shape.radius = 0.0;
                        shape.center[0] = 0.0;
                        shape.center[1] = 0.0;
                }

		~Object()
		{

		}

                virtual void move() = 0;
                virtual bool collidesWith() = 0;
};

class Ship : public Object {

        public:
                void move() override {
                    printf("The ship is moving!\n");
                }

                bool collidesWith() override {
                    printf("The ship is colliding!\n");
                    return true;
                }
};

class Enemy : public Object {

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

                bool collidesWith() override {
                    printf("The enemy is colliding!\n");
                    return true;
                }
};

class Asteroid : public Object {

        public:
                void move() override {
                    printf("The asteroid is moving!\n");
                }

                bool collidesWith() override {
                    printf("The asteroid is colliding!\n");
                    return true;
                }
};


void drawNicholasMenu(int xres, int yres, Rect r)
{
        //Ship ship;
        //ship.move();
        //ship.collidesWith();
        //

        //Ship ship;
        //Enemy enemy(&ship);
        //enemy.move();
        //enemy.collidesWith();

        //Asteroid ast;
        //ast.move();
        //ast.collidesWith();

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
