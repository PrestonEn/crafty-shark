#include "Cube.h"
#include <iostream>
/// <summary> construct a cube...its a cube constructor </summary>
/// <param name="spawn"> position at witch the cube is spawned in the world </param> 
/// <param name="direction"> direction vector the partical is moving along: normalized </param>
/// <param name="grav"> Gravitational force to be applied to the Y axis </param>
Cube::Cube(std::tuple<float, float, float>& spawn, std::tuple<float, float, float>& direction, float grav, float speed, float w){
	this->pos = spawn;
	this->direction = direction;
	this->speed = speed;
	this->gravity = grav;
	this->w = w;
}

/// <summary>I NEED A MEMORY MANAGEMENT MEDIC
/// TO MAKE SURE THIS PARTICLE IS STILL ALIVE
/// </summary>
bool Cube::checkVitals(){
	if (std::get<1>(pos) < -1000.0f || time_alive > 10000){
		return true;
	}
	else return false;
}


/// <summary>Listen...I know you're not very modular, and if this compilation unit is going to
///			work, we need to make some changes. It's not just you, I know i have some things
///			to figure out to. So lets work together and keep this relationship strong.
///			I love you graphics
///
///			F@%K it. vertex arrays YOLO
///</summary>
void Cube::drawShape(int dTypeFlag){
	
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	// vertex coords array for glDrawArrays() =====================================
	// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
	// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
	// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
	// array is 108 floats (36 * 3 = 108).
	GLfloat vertices1[] = { w, w, w, -w, w, w, -w, -w, w,      // v0-vw-v2 (front)
		-w, -w, w, w, -w, w, w, w, w,      // v2-v3-v0

		w, w, w, w, -w, w, w, -w, -w,      // v0-v3-v4 (right)
		w, -w, -w, w, w, -w, w, w, w,      // v4-v5-v0

		w, w, w, w, w, -w, -w, w, -w,      // v0-v5-v6 (top)
		-w, w, -w, -w, w, w, w, w, w,      // v6-vw-v0

		-w, w, w, -w, w, -w, -w, -w, -w,      // vw-v6-v7 (left)
		-w, -w, -w, -w, -w, w, -w, w, w,      // v7-v2-vw

		-w, -w, -w, w, -w, -w, w, -w, w,      // v7-v4-v3 (bottom)
		w, -w, w, -w, -w, w, -w, -w, -w,      // v3-v2-v7

		w, -w, -w, -w, -w, -w, -w, w, -w,      // v4-v7-v6 (back)
		-w, w, -w, w, w, -w, w, -w, -w };    // v6-v5-v4

	// normal array
	GLfloat normals1[] = { 
		0, 0, 1, 0, 0, 1, 0, 0, 1,      // v0-v1-v2 (front)
		0, 0, 1, 0, 0, 1, 0, 0, 1,      // v2-v3-v0
		1, 0, 0, 1, 0, 0, 1, 0, 0,      // v0-v3-v4 (right)
		1, 0, 0, 1, 0, 0, 1, 0, 0,      // v4-v5-v0
		0, 1, 0, 0, 1, 0, 0, 1, 0,      // v0-v5-v6 (top)
		0, 1, 0, 0, 1, 0, 0, 1, 0,      // v6-v1-v0
		-1, 0, 0, -1, 0, 0, -1, 0, 0,   // v1-v6-v7 (left)
		-1, 0, 0, -1, 0, 0, -1, 0, 0,   // v7-v2-v1
		0, -1, 0, 0, -1, 0, 0, -1, 0,   // v7-v4-v3 (bottom)
		0, -1, 0, 0, -1, 0, 0, -1, 0,   // v3-v2-v7
		0, 0, -1, 0, 0, -1, 0, 0, -1,   // v4-v7-v6 (back)
		0, 0, -1, 0, 0, -1, 0, 0, -1 }; // v6-v5-v4

	// color array
	GLfloat colors[] = 
	  { 1, 1, 1, 1, 1, 0, 1, 0, 0,      // v0-v1-v2 (front)
		1, 0, 0, 1, 0, 1, 1, 1, 1,      // v2-v3-v0
		1, 1, 1, 1, 0, 1, 0, 0, 1,      // v0-v3-v4 (right)
		0, 0, 1, 0, 1, 1, 1, 1, 1,      // v4-v5-v0
		1, 1, 1, 0, 1, 1, 0, 1, 0,      // v0-v5-v6 (top)
		0, 1, 0, 1, 1, 0, 1, 1, 1,      // v6-v1-v0
		1, 1, 0, 0, 1, 0, 0, 0, 0,      // v1-v6-v7 (left)
		0, 0, 0, 1, 0, 0, 1, 1, 0,      // v7-v2-v1
		0, 0, 0, 0, 0, 1, 1, 0, 1,      // v7-v4-v3 (bottom)
		1, 0, 1, 1, 0, 0, 0, 0, 0,      // v3-v2-v7
		0, 0, 1, 0, 0, 0, 0, 1, 0,      // v4-v7-v6 (back)
		0, 1, 0, 0, 1, 1, 0, 0, 1 };    // v6-v5-v4



	glTranslatef(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
	// enble and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals1);
	glColorPointer(3, GL_FLOAT, 0, colors1);
	glVertexPointer(3, GL_FLOAT, 0, vertices1);

	glPushMatrix();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPopMatrix(); //pop pop pop watch the matrix stack drop

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);



	glTranslatef(-1 * std::get<0>(pos), -1 * std::get<1>(pos), -1 * std::get<2>(pos));
	std::get<1>(direction) = std::get<1>(direction)-gravity;

	//update position based on the current velocity
	this->pos = std::make_tuple(std::get<0>(pos) +std::get<0>(direction)*speed,
		std::get<1>(pos) + std::get<1>(direction) * speed,
		std::get<2>(pos) +std::get<2>(direction)*speed);

	//chec for bounce conditions (give it a reference to terrain?)
	if (std::get<1>(pos) <= 0 && 
		std::get<0>(pos) >= 0 && std::get<0>(pos) <= 100 && 
		std::get<2>(pos) >= 0 && std::get<2>(pos) <= 100) std::get<1>(direction) = std::get<1>(direction)*-1;
	time_alive++;
}