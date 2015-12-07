#include "SimState.h"
SimState::SimState(int _draw_type, int _view_mode, float _gravity){
	this->draw_type = _draw_type;
	this->view_mode = _view_mode;
	this->gravity = _gravity;

	this->fps_pos = std::make_tuple(50.0f, 2.0f, 50.0f);
	this->fps_dir = std::make_tuple(0.0f, 0.0f, 0.0f);
	this->terrain = new Terrain();
}