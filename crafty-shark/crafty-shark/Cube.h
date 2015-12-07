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
class Cube{

public:
	
	Cube::Cube(std::tuple<float, float, float>& spawn, std::tuple<float, float, float>& direction, float w, float speed);
	
	bool checkVitals();
	
	void Cube::drawShape(int dTypeFlag);

private:

	std::tuple<float, float, float> pos;

	std::tuple<float, float, float> direction;

	std::tuple<float, float, float> rotation;

	std::tuple<float, float, float> rotation_update_values;

	float speed;

	float gravity;

	float w;

	int time_alive;

};

#endif