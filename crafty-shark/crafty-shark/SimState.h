#ifndef _SIMSTATE_H_
#define _SIMSTATE_H_

#include <tuple>
#include "Terrain.h"
enum drawType { SOLID, VERTEX, WIRE };

enum viewMode { FIRST_PERSON, OVERHEAD };

enum fireMode { SEMI, AUTO, SPRAY, FOUNTAIN};

enum sprayFactor { LITTLE, FAIR, LOTS};

enum keys {
	SPACEBAR = 32
};


///<summary> Holds flags values and vectors relavant to the simulation </summary>
class SimState{

public:
	SimState::SimState(int _draw_type, int _view_mode, float _gravity, float speed, bool spin);

	int draw_type = SOLID;
	int view_mode = OVERHEAD;
	int fireMode = FOUNTAIN;

	float gravity;
	float speed;
	float spray =0.0f;
	float friction = 0.0f;
	bool spin;


	std::tuple<float, float, float> fountain_position;
	std::tuple<float, float, float> fountain_direction;
	
	std::tuple<float, float, float> overhead_pos;
	std::tuple<float, float, float> overhead_angle;
	std::tuple<float, float, float> scene_rotation;
	std::tuple<float, float, float> fps_pos;
	std::tuple<float, float, float> fps_dir;
	Terrain *terrain;
};
#endif