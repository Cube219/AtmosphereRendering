#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

#include "BaseHeader.h"

class Scene
{
public:
    Scene() {}
    virtual ~Scene() {}

    virtual void Begin() {}
    virtual void End() {}

    virtual void Update(double dt) {}

    virtual void OnResize(int width, int height) {}
    virtual void OnKeyPress(int code) {}
    virtual void OnKeyRelease(int code) {}
};

#endif // __SCENE_H__
