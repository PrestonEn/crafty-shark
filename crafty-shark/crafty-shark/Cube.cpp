#include "Cube.h"
#include <iostream>
/// <summary> construct a cube </summary>
/// <param name="spawn"> position at witch the cube is spawned in the world </param> 
/// <param name="direction"> direction vector the partical is moving along: normalized </param>
/// <param name="grav"> Gravitational force to be applied to the Y axis </param>
Cube::Cube(std::tuple<float, float, float>& spawn, std::tuple<float, float, float>& direction, float grav, float speed){
	this->pos = spawn;
	this->direction = direction;
	this->speed = speed;
	this->gravity = grav;
}


bool Cube::checkVitals(){
	if (std::get<1>(pos) < -1000.0f || time_alive > 10000){
		return true;
	}
	else return false;
}


void Cube::drawShape(int dTypeFlag){
	glTranslatef(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
	glBegin(GL_QUADS);


	glEnd();
	glFlush();
	glTranslatef(-1 * std::get<0>(pos), -1 * std::get<1>(pos), -1 * std::get<2>(pos));
	std::get<1>(direction) = std::get<1>(direction)-gravity;

	//update position based on the current velocity
	this->pos = std::make_tuple(std::get<0>(pos) +std::get<0>(direction)*speed,
		std::get<1>(pos) + std::get<1>(direction) * speed,
		std::get<2>(pos) +std::get<2>(direction)*speed);


	if (std::get<1>(pos) <= 0 && 
		std::get<0>(pos) >= 0 && std::get<0>(pos) <= 100 && 
		std::get<2>(pos) >= 0 && std::get<2>(pos) <= 100) std::get<1>(direction) = std::get<1>(direction)*-1;
	time_alive++;
}