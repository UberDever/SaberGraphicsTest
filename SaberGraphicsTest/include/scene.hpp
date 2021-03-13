#ifndef SABERGRAPHICSTEST_SCENE_H
#define SABERGRAPHICSTEST_SCENE_H

#include "core.hpp"

class Scene
{

public:
    virtual void processEvents(GLFWwindow *window) = 0;
    virtual void beginFrame() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void endFrame() = 0;

    virtual void setInputCallback(GLFWwindow *window) = 0;
};

#endif