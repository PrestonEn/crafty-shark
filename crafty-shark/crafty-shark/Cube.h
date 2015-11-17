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

#include "Shape.h"

class Cube:public Shape{

public:
	Cube::Cube(Position& spawn, ForceVector& vF,float w, int h);

	///
	void Cube::drawShape(float drag, float gravity);

private:
	float _w;
	int _h;
};

Cube::Cube(Position& spawn, ForceVector& vF, float w, int h){
	pos = spawn;
	actingForces = vF;
	_w = w;
}

void Cube::drawShape(float drag, float gravity){
	glTranslatef(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
	glBegin(GL_QUADS);
	
	glVertex3f(-_w, -_w, _w);
	glVertex3f(_w, -_w, _w);
	glVertex3f(_w, _w, _w);

	glVertex3f(-_w, _w, _w);
	glVertex3f(-_w, _w, _w);
	glVertex3f(_w, _w, _w);

	glVertex3f(_w, _w, -_w);
	glVertex3f(-_w, -_w, -_w);
	glVertex3f(-_w, _w, -_w);

	glVertex3f(_w, _w, -_w);
	glVertex3f(_w, -_w, -_w);
	glVertex3f(-_w, -_w, -_w);

	glVertex3f(-_w, -_w, -_w);
	glVertex3f(_w, -_w, -_w);
	glVertex3f(_w, -_w, _w);

	glVertex3f(-_w, -_w, _w);
	glVertex3f(_w, -_w, _w);
	glVertex3f(_w, -_w, -_w);

	glVertex3f(_w, _w, -_w);
	glVertex3f(_w, _w, _w);
	glVertex3f(-_w, -_w, -_w);

	glVertex3f(-_w, -_w, _w);
	glVertex3f(-_w, _w, _w);
	glVertex3f(-_w, _w, -_w);

	glEnd();
	glFlush();
	glTranslatef(-1*std::get<0>(pos), -1*std::get<1>(pos), -1*std::get<2>(pos));

	//update velocity based on parameters like drag

	//update position based on the current velocity
	this->pos = std::make_tuple(std::get<0>(pos) + std::get<0>(actingForces), 
								std::get<1>(pos) + std::get<1>(actingForces), 
								std::get<2>(pos) + std::get<2>(actingForces));
}

#endif