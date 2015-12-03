#include <iostream>
#include <vector>

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




Position posV = std::make_tuple(50.0f, 5.0f, +50.0f);


// angle of rotation for the camera direction
float angle = 0.0f;
float yangle = 0.0f;
// actual vector representing the camera's direction
float lx = 0.2f, ly = 0.0f, lz = 0.2f;
ForceVector fV = std::make_tuple(4*lx, 0.0, 4*lz);
Cube ccube = Cube(posV, fV, 0.2, 1, 0.001);
// XZ position of the camera
float x = 50.0f, y = 5.0f, z = 50.0f;

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
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
	//TODO
}


void renderScene(void) {
	glPointSize(10);
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
	gluLookAt(x, y, z,					//eye position
			  x + lx, y + ly, z + lz,	//direction
			  0.0f, 1.0f, 0.0f);		

	// Draw ground
	//glColor3f(0.9f, 0.9f, 0.9f);
	//glBegin(GL_LINES);
	//for (int i = 0; i < 11; i++){
	//	glVertex3f(i*10.0f, 0.0f, 0.0f);
	//	glVertex3f(i*10.0f, 0.0f, 100.0f);
	//	glVertex3f(100.0f, 0.0f, i*10.0f);
	//	glVertex3f(0.0f, 0.0f, i*10.0f);
	//}
	//glEnd();


	//glTranslatef(100.0f, 0.0f, 10.0f);
	glPushMatrix();
	//obangle += 2.0f;
	//glRotatef(obangle, 0.0f, 1.0f, 0.0f); //rotating object continuously by 2 degree
	//glTranslatef(-100.0f, 0.0f, -10.0f);
	ccube.drawShape(0, 0);

	glPopMatrix();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == 27)
		exit(0);
}

void pressKey(int key, int xx, int yy) {
	std::cout << key << std::endl;
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 7.0f; break;
	case GLUT_KEY_LEFT: 
		y = y + 1.5f; 
		printf("JUMP\n");
		break;
	case GLUT_KEY_DOWN: deltaMove = -1.0f; break;
	default:
		printf("%d\n", key);
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
		ly = sin(yangle + ((y - yOrigin) * 0.001f));
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

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
	std::cout << x << "\t" << y << "\t" << z << "\n";

	deltaAngle = (x - xOrigin) * 0.001f;
	ydeltaAngle = (y - yOrigin) * -0.001f;
	// update camera's direction
	lx = sin(angle + deltaAngle);
	ly = sin(yangle + ((y - yOrigin) * 0.001f));
	lz = -cos(angle + deltaAngle);

	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {

	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
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