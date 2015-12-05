/**
*	\class		Shape
*
*	\ingroup	shapes
*
*	\brief		Base class for any shapes defined
*
*	As any physical object in the simulation is some simple shape
*	(what do I look like? a real developer?) all objects will be
*	a central position in 3D space (because BSP-trees are neat),
*	a collection of polygons, a bounding radius, and a velocity
*	vector
*
*	\note		shapes can be more than cubes...neat
*
*	\author		Preston Engstrom
*
*	\date		16/11/2015
*
*/
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <iostream>
#include <tuple>
#include <freeglut.h>
#include <vector>

///use to hold positions in space
typedef std::tuple<float, float, float> Position;

///use to hold angles for applying forces
typedef std::tuple<float, float, float> ForceVector;

class Shape{
public:
	///make a shape
	//Shape::Shape(); 

	//Shape::~Shape();

	/** \brief take 3 floats to modify the velocity components
	 *	\param external the external force being applied
	 *	
	 */
	void Shape::updateVelocity(ForceVector& external);

	/**	\brief openGL call to draw the shape with appropriate transformations
	 *	
	 *	update shapes position, check for collision, apply transformations
	 *
	 */
	 void Shape::drawShape();

protected:
	///{x, y, z} position
	Position pos;

	///
	ForceVector direction;

	///{vx, vy, vz}
	ForceVector actingForces;
	
	float b_sphere_radius;

	float grav;


private:

	///if true, allow to be destroyed after some number of epocs
	bool timed_life;
	///how long has the object existed?
	int epocs_alive;

};
#endif