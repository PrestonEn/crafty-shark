/**
 *	\class		Cube
 *
 *
 *	\brief		ITS A CUBE

 *	
 *	\note		cubes are neat
 *
 *	\author		Preston Engstrom
 *
 */

#ifndef _CUBE_H_
#define _CUBE_H_

#include <tuple>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include "Terrain.h"
#include "SimState.h"

enum cause_of_death{ IM_OKAY, IMPACT, TIME };

class Cube{

public:
	
	Cube::Cube(std::tuple<float, float, float>& spawn, std::tuple<float, float, float>& direction, float grav, float speed, float w, int life, bool is_expl, SimState* s);
	int isKill();
	void Cube::drawShape(int dTypeFlag);
	std::tuple<float, float, float> pos;
	bool Cube::isStrike();
	bool is_expl;
private:

	SimState* state;

	std::tuple<float, float, float> direction;

	std::tuple<float, float, float> rotation;

	std::tuple<float, float, float> rotation_update_values;

	float speed;

	float gravity;

	float w;

	int time_alive;



};

#endif