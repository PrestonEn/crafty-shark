#pragma once
#include <tuple>
#include "Terrain.h"
enum drawType { SOLID, VERTEX, WIRE };

enum viewMode { FIRST_PERSON, OVERHEAD };

enum fireMode { SEMI, AUTO, SPRAY };

enum keys {
	SPACEBAR = 32
};


class SimState{

public:
	SimState::SimState(int _draw_type, int _view_mode, float _gravity);

	int draw_type = SOLID;
	int view_mode = OVERHEAD;
	int count_active;
	float gravity;

	std::tuple<float, float, float> fountain_position;
	std::tuple<float, float, float> fountain_direction;
	std::tuple<float, float, float> overhead_pos;

	std::tuple<float, float, float> overhead_angle;
	std::tuple<float, float, float> scene_rotation;
	std::tuple<float, float, float> fps_pos;
	std::tuple<float, float, float> fps_dir;
	Terrain *terrain;
};
