/**
 *	\class		Cube
 *
 *	\ingroup	shapes
 *
 *	\brief		Class holding a cubes position for use in BSP-tree 
 *
 *	A cube is represented by its midpoint, width and height. 
 *	The radius of th bounding sphere is equivalent to half
 *	the width of the cube 
 *
 *  \note		NOTE MAY BE MODIFIED IN THE INTEREST OF IMPROVING VISUALS
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

class Cube{

public:
	
	Cube::Cube(std::tuple<float, float, float>& spawn, std::tuple<float, float, float>& direction, float w, float speed);
	
	bool checkVitals();
	
	void Cube::drawShape(int dTypeFlag);

private:

	std::tuple<float, float, float> pos;

	std::tuple<float, float, float> direction;

	std::tuple<float, float, float> rotation;

	float speed;

	float gravity;

	int time_alive;

};

#endif