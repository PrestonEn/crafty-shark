#include "Terrain.h"
#include <freeglut.h>

Terrain::Terrain():
width(200), height(200){
	indicies = new int[width*height];
}


Terrain::Terrain(int w, int h) :
width(w), height(h){
	indicies = new int[width*height];
}


///<summary>render the terrain as a series of triangle strips</summary>
///
void Terrain::draw(bool hole){
	bool evodd = true;
	int depth = 0;
	for (int strip = 0; strip < height; strip++){
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3i(0, 0, strip);    
		glVertex3i(0, 0, strip+1);    
		glVertex3i(1, 0, strip);
		for (int j = 1; j <= width; j++){
			if (hole && strip > width / 3 && strip < 2 * width / 3 && j > height / 3 && j < 2 * height / 3)
				 depth = -200;

			if (evodd){
				glVertex3i(j, depth, strip + 1);    
				glVertex3i(j + 1, depth, strip);
				glVertex3i(j + 1, depth, strip + 1);
			}
			else{
				glVertex3i(j, depth, strip);
				glVertex3i(j + 1, depth, strip + 1);
				glVertex3i(j + 1, depth, strip);
			}
			evodd = !evodd;
		}
		glEnd();
	}
}