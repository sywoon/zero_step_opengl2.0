#pragma once
#include <string>
#include "sdl2/SDL.h"

class CDisplay
{
public:
	CDisplay(int width, int height, const std::string& title);
	virtual ~CDisplay();

	void Update();
	bool IsClosed();
	void Clear(float r, float b, float g, float a);

private:
	SDL_Window* _window;
	SDL_GLContext _glContext;
	bool _isClosed;
};
