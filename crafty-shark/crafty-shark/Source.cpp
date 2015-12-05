#include <iostream>
#include <vector>
#include <ctime>

#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>

#include <stdlib.h>
#include <math.h>
#include "Shape.h"
#include "Cube.h"

enum keys {
	SPACEBAR = 32
};

GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 0.9, 0.5, 0.3, 1.0 };
GLfloat light_specular[] = { 0.9, 0.1, 0.1, 1.0 };
GLfloat light_position[] = { 7.0, 6.0, 1.0, 0.0 };


std::vector<Cube*> activeShapes;
bool semi_auto = true;


// angle of rotation for the camera direction
float angle = 0.0f;
float yangle = 0.0f;
// actual vector representing the camera's direction
float lx = 0.2f, ly = 0.0f, lz = 0.2f;
// XZ position of the camera
float px = 50.0f, py = 5.0f, pz = 50.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
float ydeltaAngle = 0.0f;
float ydeltaMove = 0.0f;
int xOrigin = -1;
int yOrigin = -1;

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 200.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

/**\brief update position on x,z plane. 
 * 
 * 
 */
void computePos(float deltaMove) {
	px += deltaMove * lx * 0.1f;
	pz += deltaMove * lz * 0.1f;
	//TODO
}


void renderScene(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);   /* turns on lighting */
	glEnable(GL_LIGHT0);     /* turns on light 0  */

	glPointSize(1);
	glBegin(GL_POINT);
	glColor3f(1.0, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	//glPushMatrix();
	gluLookAt(px, py, pz,					//eye position
			  px + lx, py + ly, pz + lz,	//direction
			  0.0f, 1.0f, 0.0f);		

	// Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_LINES);
	for (int i = 0; i < 11; i++){
		glVertex3f(i*10.0f, 0.0f, 0.0f);
		glVertex3f(i*10.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, i*10.0f);
		glVertex3f(0.0f, 0.0f, i*10.0f);
	}
	glVertex3f(px-0.3, py, pz);
	glVertex3f(px + 100 * lx, (py - 0.5f) + 100*ly, pz + 100 *lz);
	glEnd();


	//glTranslatef(100.0f, 0.0f, 10.0f);
	glPushMatrix();
	//obangle += 2.0f;
	//glRotatef(obangle, 0.0f, 1.0f, 0.0f); //rotating object continuously by 2 degree
	//glTranslatef(-100.0f, 0.0f, -10.0f);

	glPopMatrix();
	for (Cube* shape : activeShapes){
		shape->drawShape();
	}

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == 27)
		exit(0);
}

void pressKey(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 7.0f; break;
	case GLUT_KEY_LEFT: 
		py = py + 1.5f; 
		break;
	case GLUT_KEY_DOWN: deltaMove = -1.0f; break;
	default:
		break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void mouseMove(int x, int y) {
	// this will only be true when the left button is down
	if (xOrigin >= 0) {
		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		ydeltaAngle = (y - yOrigin) * -0.001f;
		// update camera's direction
		lx = sin(angle + deltaAngle);
		ly = sin(yangle + ydeltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		activeShapes.push_back(new Cube(std::make_tuple(px, py, pz),
										std::make_tuple(4 * lx, 4 * ly , 4 * lz), 
										0.2, 1, 0.001));

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			yangle += ydeltaAngle;
			xOrigin = -1;
			yOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
}

void joystickFunction(unsigned int buttonMask, int x, int y, int z){
	if (!(buttonMask & GLUT_JOYSTICK_BUTTON_B)) semi_auto = true;
	deltaAngle = (x - xOrigin) * 0.001f;
	ydeltaAngle = (y - yOrigin) * -0.001f;
	// update camera's direction
	lx = sin(angle + deltaAngle);
	ly = sin(yangle + ((y - yOrigin) * 0.001f));
	lz = -cos(angle + deltaAngle);

	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {
		activeShapes.push_back(new Cube(std::make_tuple(px, py, pz),
			std::make_tuple(lx, ly, lz),
			0.2, 1, 0.001));
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
		if (semi_auto){
			activeShapes.push_back(new Cube(std::make_tuple(px, py, pz),
				std::make_tuple(lx, ly, lz),
				0.2, 1, 0.001));

			semi_auto = false;
		}
		printf("button B is pressed ");
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_C) {
		printf("button C is pressed ");
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_D) {
		printf("button D is pressed ");
	}
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(900, 600);
	glutCreateWindow("ROTATING");
//	glutFullScreen();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(0);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutJoystickFunc(joystickFunction, 10);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}