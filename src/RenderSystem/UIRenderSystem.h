#pragma once

#ifndef __UI_RENDER_SYSTEM_H__
#define __UI_RENDER_SYSTEM_H__

#include "BaseHeader.h"

#include "../imgui/imgui.h"

class UIRenderSystem
{
public:
    UIRenderSystem();
    ~UIRenderSystem() = delete;

    static void Initialize();
    static void Shutdown();

    static void Update(double dt);
    static void SubRender();

private:
    static bool show_demo_window;
    static bool show_another_window;
    static ImVec4 clear_color;
};

#endif // __UI_RENDER_SYSTEM_H__
