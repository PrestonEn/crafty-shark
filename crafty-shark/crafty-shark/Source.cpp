#include <iostream>
#include <list>
#include <ctime>
#include <tuple>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>

#include <stdlib.h>
#include <math.h>
#include "Cube.h"
#include "SimState.h"




SimState sim = SimState(WIRE, FIRST_PERSON, 0.5f);

float obangle = 0.0f;
bool rotate = true;
int xsum = 0;
std::list<Cube*> activeShapes;
bool semi_auto = true;
// angle of rotation for the camera direction
float angle = 0.0f;
float yangle = 0.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
float ydeltaAngle = 0.0f;
int xOrigin = -1;
int yOrigin = -1;




void displayText(float x, float y, int r, int g, int b, const char *string) {
	glLoadIdentity();
	int j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}


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
	gluPerspective(45.0f, ratio, 0.1f, 800.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

}

/**\brief update position on x,z plane. 
 * 
 * 
 */
void computePos(float deltaMove) {
	std::get<0>(sim.fps_pos) += deltaMove * std::get<0>(sim.fps_dir) * 0.1f;
	std::get<2>(sim.fps_pos) += deltaMove * std::get<2>(sim.fps_dir) * 0.1f;
	std::cout << std::get<0>(sim.fps_pos) << " " << std::get<2>(sim.fps_pos) << std::endl;
}


std::tuple<float, float, float> normalize(std::tuple<float, float, float> v){
	float len = std::sqrt(std::get<0>(v)*std::get<0>(v) +std::get<1>(v)*std::get<1>(v) + std::get<2>(v)*std::get<2>(v));
	return std::make_tuple(std::get<0>(v) / len, std::get<1>(v) / len, std::get<2>(v) / len);
}


void renderScene(void) {
	if (deltaMove)
		std::cout << "delta" << std::endl;
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	
	glLoadIdentity();
	// Set the camera
	//glPushMatrix();
	gluLookAt(std::get<0>(sim.fps_pos), std::get<1>(sim.fps_pos), std::get<2>(sim.fps_pos),					//eye position
			  std::get<0>(sim.fps_pos) + std::get<0>(sim.fps_dir), std::get<1>(sim.fps_pos) + std::get<1>(sim.fps_dir), std::get<2>(sim.fps_pos) + std::get<2>(sim.fps_dir),	//direction
			  0.0f, 1.0f, 0.0f);

	//if (rotate){
	//	obangle += 0.002f;
	//	glTranslated(50.0, 0.0, 50.0);
	//	glRotatef(obangle, 0.0f, 1.0f, 0.0f); //rotating object continuously by 2 degree
	//	glTranslated(-50, 0.0, -50);
	//}

	sim.terrain->draw();

	for (Cube* cube : activeShapes){
		cube->drawShape(1);
	}

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == 27)
		exit(0);
	if (key == 'f'){
		rotate = false;
	}
}

void pressKey(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.10f; break;
	case GLUT_KEY_LEFT: 

		break;

	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_DOWN: deltaMove = -0.10f; break;
	default:
		std::cout << key << std::endl;
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
		std::get<0>(sim.fps_dir) = sin(angle + deltaAngle);
		std::get<1>(sim.fps_dir) = sin(yangle + ydeltaAngle);
		std::get<2>(sim.fps_dir) = -cos(angle + deltaAngle);
		std::cout << "angle: " << std::get<0>(sim.fps_dir) << " " << std::get<1>(sim.fps_dir) << " " << std::get<2>(sim.fps_dir) << std::endl;
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
		//count_active++;
		

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

float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void joystickFunction(unsigned int buttonMask, int x, int y, int z){
	if (!(buttonMask & GLUT_JOYSTICK_BUTTON_B)) semi_auto = true;
	xsum = xsum + x/100;
	deltaAngle = (xsum) * 0.001f;
	ydeltaAngle = (0 - y) * -0.001f;

	std::get<0>(sim.fps_dir) = sin(angle + deltaAngle);
	std::get<1>(sim.fps_dir) = sin(yangle + ydeltaAngle);
	std::get<2>(sim.fps_dir) = -cos(angle + deltaAngle);

	std::cout << std::get<0>(sim.fps_dir) << std::endl;


	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {
		std::cout << std::get<0>(sim.fps_pos);
		activeShapes.push_back(new Cube(sim.fps_pos,
			normalize(sim.fps_dir),
			0.01f, 0.10f));

	}

	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
		if (semi_auto){
			activeShapes.push_back(new Cube(std::make_tuple(50.f, 2.0f, 50.f),
				normalize(std::make_tuple(0.0f, 1.0f, 0.0f)),
				0.05f, 0.2f));
			semi_auto = false;
		}
	}
}

int main(int argc, char **argv) {
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(900, 600);
	glutCreateWindow("ROTATING");
	//glutFullScreen();
	//register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	//glutMouseFunc(mouseButton);
	//glutMotionFunc(mouseMove);
	glutJoystickFunc(joystickFunction, 10);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 11111111111111111111;
}