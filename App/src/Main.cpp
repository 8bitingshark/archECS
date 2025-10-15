#include "core/Engine.h"
#include "DemoECSLayer.h"

int main(int argc, char** argv)
{
	core::Engine engine;
	engine.AddLayerToStack<app::DemoECSLayer>(engine.GetRenderer());

	engine.Start();
}


// temp for demo
#ifdef _WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	extern int main(int, char**);
	return main(__argc, __argv);
}
#endif