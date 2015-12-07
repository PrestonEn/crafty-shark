#include "SimState.h"
SimState::SimState(int _draw_type, int _view_mode, float _gravity, float speed, bool spin){
	this->draw_type = _draw_type;
	this->view_mode = _view_mode;
	this->gravity = _gravity;
	this->speed = speed;
	this->spin = spin;

	this->fps_pos = std::make_tuple(50.0f, 2.0f, 50.0f);
	this->fps_dir = std::make_tuple(0.0f, 0.0f, 0.0f);
	
	fountain_position = std::make_tuple(50.0f, 2.0f, 50.0f);
	fountain_direction = std::make_tuple(0.0f, 2.0f, 0.0f);

	overhead_pos = std::make_tuple(166.98f, 60.5f, 146.899f);
	overhead_angle = std::make_tuple(-0.770101f, -0.343837f, -0.637922f);

	this->terrain = new Terrain();
}