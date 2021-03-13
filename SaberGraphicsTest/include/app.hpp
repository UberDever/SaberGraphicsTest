#ifndef SABERGRAPHICSTEST_APP_H
#define SABERGRAPHICSTEST_APP_H

#include "core.hpp"
#include "scene.hpp"

class App
{
	GLFWwindow* mWindow;
	int mWindowW, mWindowH;
	glm::vec2 mLastMouseCoords;

	std::vector<std::unique_ptr<Scene>> mScenes;
	int mCurScene;

	static void framebuffer_size_callback(GLFWwindow* window, int w, int h)
	{
		glViewport(0, 0, w, h);
	}

	App(int w, int h);

public:
	int exec();
	static App& instance(int w = 0, int h = 0)
	{
		static App app(w, h);
		return app;
	}

	void swapBuffers() { glfwSwapBuffers(mWindow); }
	void close() { glfwSetWindowShouldClose(mWindow, true); }

	void registerScene(Scene* ptr)
	{
		mScenes.emplace_back(ptr);
	}

	void setInputCallback(Scene* ptr)
	{
		ptr->setInputCallback(mWindow);
	}

	void changeScene(std::string sceneName)
	{
		mCurScene = (mCurScene + 1) % mScenes.size();
		mScenes[mCurScene]->setInputCallback(mWindow);
		glfwSetWindowTitle(mWindow, sceneName.c_str());
	}

	int getW() { return mWindowW; }
	int getH() { return mWindowH; }
	glm::vec2 calculateRelMouse(const glm::vec2& mouseCoords)
	{
		float relX = mLastMouseCoords.x - mouseCoords.x;
		float relY = mouseCoords.y - mLastMouseCoords.y;

		mLastMouseCoords = mouseCoords;

		return { relX, relY };
	}
};

#endif