#include "Engine.h"

Engine::Engine()
	:
	wnd(1024, 576, "WorldForge", { { 1024,576 } }),
	kbd(wnd.kbd),
	mouse(wnd.mouse),
	gfx(wnd.gfx()),
	AppClock()
{

}

void Engine::Go()
{
	gfx.NewFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Engine::UpdateModel()
{

}

void Engine::ComposeFrame()
{

}