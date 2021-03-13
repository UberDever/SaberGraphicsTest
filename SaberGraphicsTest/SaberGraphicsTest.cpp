#include "include/app.hpp"
#include "include/MainScene.hpp"

#ifdef _WIN32
#include <windows.h>
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

int main(int, char**) {

	App::instance(1000, 1000);
	Scene* scene = new MainScene();
	App::instance().registerScene(scene);
	App::instance().setInputCallback(scene);
	return App::instance().exec();
}
