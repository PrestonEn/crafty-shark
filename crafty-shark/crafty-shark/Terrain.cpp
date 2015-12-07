#include "Terrain.h"
#include <freeglut.h>

Terrain::Terrain():
width(100), height(100){
	indicies = new int[width*height];
}


Terrain::Terrain(int w, int h) :
width(w), height(h){
	indicies = new int[width*height];
}


///<summary>render the terrain as a series of triangle strips</summary>
///
void Terrain::draw(){
	bool evodd = true;
	for (int strip = 0; strip < height; strip++){
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3i(0, 0, strip);    
		glVertex3i(0, 0, strip+1);    
		glVertex3i(1, 0, strip);

		for (int j = 1; j <= width; j++){
			if (evodd){
				glVertex3i(j, 0, strip + 1);    
				glVertex3i(j + 1, 0, strip);    
				glVertex3i(j + 1, 0, strip + 1);    
			}
			else{
				glVertex3i(j, 0, strip);    
				glVertex3i(j + 1, 0, strip + 1);    
				glVertex3i(j + 1, 0, strip);    
			}
			evodd = !evodd;
		}
		glEnd();
	}
}