#pragma once
/**
*	\class		Terrain
*
*	\ingroup	Terrain
*
*	\brief		Base class for Triangle strip plane
*
*	Triangle strip square, with a definied length/width. Implements a default constructor to make a 
*	100 by 100 point plane.
*
*	\TODO		IMPLEMENT UNEVEN TERRAIN AND BITMAP READING
*
*	\note		EHR MAH GERD TRIANGLES
*
*	\author		Preston Engstrom
*
*	\date		3/12/2015
*
*/

class Terrain{
public:
	Terrain::Terrain();
	Terrain::Terrain(int width, int height);
	void Terrain::draw(bool hole);
	//Terrain::Terrain(); takes in a bitmap?
	int* indicies;
	float* points;

private:
	const int width;
	const int height;

	
};


