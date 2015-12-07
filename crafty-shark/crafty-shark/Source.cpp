#include <iostream>
#include <list>
#include <ctime>
#include <tuple>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <MMSystem.h>
#include <stdlib.h>
#include <math.h>
#include "Cube.h"
#include "SimState.h"


float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


SimState* sim = new SimState(SOLID, FIRST_PERSON, 0.01f, 1.0f, true);
std::list<Cube*> activeShapes;

int shots_fired = 0;

float obangle = 0.0f;
float spin_factor = 0.0f;
bool rotate = true;
int xsum = 0;
bool semi_auto = true;

// angle of rotation for the camera direction
float angle = 0.0f;
float yangle = 0.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
float zdeltaMove = 0;
float ydeltaAngle = 0.0f;
int xOrigin = -1;
int yOrigin = -1;


//
//
//
std::tuple<float, float, float>& particle_start_pos = sim->fps_pos;
std::tuple<float, float, float>& particle_start_dir = sim->fps_dir;
std::tuple<float, float, float>& camera_pos = sim->fps_pos;
std::tuple<float, float, float>& camera_dir = sim->fps_dir;

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
	std::get<0>(sim->fps_pos) += deltaMove * std::get<0>(sim->fps_dir) * 0.1f;
	std::get<2>(sim->fps_pos) += deltaMove * std::get<2>(sim->fps_dir) * 0.1f;

}


std::tuple<float, float, float> normalize(std::tuple<float, float, float> v){
	float len = std::sqrt(std::get<0>(v)*std::get<0>(v) +std::get<1>(v)*std::get<1>(v) + std::get<2>(v)*std::get<2>(v));
	return std::make_tuple(std::get<0>(v) / len, std::get<1>(v) / len, std::get<2>(v) / len);
}

std::tuple<float, float, float> addSpray(std::tuple<float, float, float> v, float spray){
	return std::make_tuple(std::get<0>(v) +randomFloat(-1 * spray, spray),
		std::get<1>(v) +randomFloat(-1 * spray, spray),
		std::get<2>(v) +randomFloat(-1 * spray, spray));
}




void renderScene(void) {
	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (sim->draw_type ==SOLID)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (sim->draw_type == VERTEX)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (sim->draw_type == WIRE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Reset transformations
	
	glLoadIdentity();
	// Set the camera
	//glPushMatrix();
	gluLookAt(std::get<0>(camera_pos), std::get<1>(camera_pos), std::get<2>(camera_pos),					//eye position
		std::get<0>(camera_pos) +std::get<0>(camera_dir), std::get<1>(camera_pos) +std::get<1>(camera_dir), std::get<2>(camera_pos) +std::get<2>(camera_dir),	//direction
			  0.0f, 1.0f, 0.0f);

	if (rotate){
		obangle += spin_factor;
		glTranslated(100.0, 0.0, 100.0);
		glRotatef(obangle, 0.0f, 1.0f, 0.0f); //rotating object continuously by 2 degree
		glTranslated(-100, 0.0, -100);
	}

	sim->terrain->draw(false);

	auto itr = std::begin(activeShapes);
	while (itr != std::end(activeShapes)){
		(*itr)->drawShape(1);
		if ((*itr)->isStrike()){
			for (int i = 0; i < 10; i++){

				std::get<1>((*itr)->pos) = std::get<1>((*itr)->pos) + 0.1;
				activeShapes.push_back(new Cube((*itr)->pos,
					normalize(std::make_tuple(randomFloat(-1, 1),
					randomFloat(0, 1), randomFloat(-1, 1))),
					0,
					randomFloat(0.001, 0.01), 0.1, 400, true, sim));
			}
		}
		if ((*itr)->isKill() == IMPACT || (*itr)->isKill() == TIME){
			if ((*itr)->is_expl == false && (*itr)->isKill() == TIME){
				PlaySound(NULL, 0, SND_ASYNC);
				PlaySound(TEXT("peh.wav"), 0, SND_FILENAME | SND_ASYNC); // Play sound
				for (int i = 0; i < 10; i++){

					std::get<1>((*itr)->pos) = std::get<1>((*itr)->pos) + 0.1;
					activeShapes.push_back(new Cube((*itr)->pos, 
						normalize(std::make_tuple(randomFloat(-1, 1), 
						randomFloat(0, 1), randomFloat(-1, 1))), 
						0, 
						randomFloat(0.001, 0.01), 0.1, 800, true, sim));
				}
			}
			itr = activeShapes.erase(itr);
		}
		else ++itr;
	}

	glutSwapBuffers();
}


void pressKey(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 1.0f; break;
	case GLUT_KEY_LEFT: 

		break;

	case GLUT_KEY_RIGHT:
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
		std::get<0>(sim->fps_dir) = sin(angle + deltaAngle);
		std::get<1>(sim->fps_dir) = sin(yangle + ydeltaAngle);
		std::get<2>(sim->fps_dir) = -cos(angle + deltaAngle);
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
	
	if (!(buttonMask & GLUT_JOYSTICK_BUTTON_B)) semi_auto = true;
	xsum = xsum + x/100;
	deltaAngle = (xsum) * 0.001f;
	ydeltaAngle = (0 - y) * -0.001f;
	if (sim->view_mode != OVERHEAD){
		std::get<0>(sim->fps_dir) = sin(angle + deltaAngle);
		std::get<1>(sim->fps_dir) = sin(yangle + ydeltaAngle);
		std::get<2>(sim->fps_dir) = -cos(angle + deltaAngle);
	}
	//std::cout << std::get<0>(sim->fps_dir) << " " << std::get<1>(sim->fps_dir) << " " << std::get<2>(sim->fps_dir) << std::endl;
	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {
		if (shots_fired > 0){ --shots_fired; }
		else{

			shots_fired = 10;
			activeShapes.push_back(new Cube(particle_start_pos,
				normalize(addSpray(particle_start_dir, sim->spray)),
				sim->gravity, sim->speed, 0.2, 2000, false, sim));
	

		}
	}

	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
		if (semi_auto){
			activeShapes.push_back(new Cube(particle_start_pos,
				normalize(addSpray(particle_start_dir, sim->spray)),
				sim->gravity, sim->speed, 1.0,2000, false, sim));
			semi_auto = false;

		}
	}
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == 27)
		exit(0);
	if (key == 'f'){
		sim->view_mode = OVERHEAD;
		camera_pos = std::make_tuple(256.98f, 120.5f, 256.899f);
		camera_dir = std::make_tuple(-0.770101f, -0.343837f, -0.637922f);


		rotate = true;
	}
	if (key == 'p'){
		sim->view_mode = FIRST_PERSON;
		glutJoystickFunc(joystickFunction, 20);

		camera_pos = sim->fps_pos = std::make_tuple(50.0f, 2.0f, 50.0f);
		camera_dir = sim->fps_dir = std::make_tuple(0.0f, 0.0f, 0.0f);

		particle_start_pos = sim->fps_pos;
		particle_start_dir = sim->fps_dir;

		rotate = false;
	}

	if (key == '+'){
		sim->spray += 0.01;
	}

	if (key == '-'){
		sim->spray -= 0.01;
	}

	if (key == 'a'){
		activeShapes.push_back(new Cube(particle_start_pos,
			normalize(addSpray(particle_start_dir, sim->spray)),
			0.001, 0.5, 1, 2000, false, sim));
	}

	if (key == '['){
		spin_factor -= 0.02f;
	}

	if (key == ']'){
		spin_factor += 0.02f;
	}
	
	if (key == '9'){
		sim->friction -= 0.02f;
	}

	if (key == '0'){
		sim->friction += 0.02f;
	}


	if (key == ']'){
		spin_factor += 0.02f;
	}

	if (key == 'r'){
		sim = new SimState(WIRE, FIRST_PERSON, 0.5f, 1.0f, true);
	}
	if (key == '1'){
		sim->draw_type = SOLID;
	}
	if (key == '2'){
		sim->draw_type = VERTEX;
	}
	if (key == '3'){
		sim->draw_type = WIRE;
	}
}



int main(int argc, char **argv) {
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(900, 600);
	glutCreateWindow("PEW PEW");
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

	//glutMouseFunc(mouseButton);
	//glutMotionFunc(mouseMove);
	glutJoystickFunc(joystickFunction, 20);
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}