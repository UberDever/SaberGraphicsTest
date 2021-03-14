#include "include/app.hpp"
#include "include/MainScene.hpp"

// Forcing to discrete card video mode
#ifdef _WIN32
#include <windows.h>
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

int main(int, char**) {

	// All non-graphical functionality is covered by tests
	// Third graphical assignment is complited with OpenGL representation
	// Details of OpenGL scene is irrelevant
	App::instance(800, 600);
	Scene* scene = new MainScene();
	App::instance().registerScene(scene);
	App::instance().setInputCallback(scene);
	return App::instance().exec();
}
