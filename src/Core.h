#pragma once

#ifndef __CORE_H__
#define __CORE_H__

#include "BaseHeader.h"

enum class MouseButton : uint
{
    Left, Middle, Right
};

class Core
{
public:
    Core() = delete;
    ~Core() = delete;

    static bool Initialize(const char* windowName, int width, int height);
    static void Shutdown();
    static void Run();

    static void FinishGame();

    static void PauseGame();
    static void ResumeGame();

    static GLFWwindow* GetGLFWwindow() { return window; }

    static int GetWindowWidth() { return windowWidth; }
    static int GetWindowHeight() { return windowHeight; }

    static bool IsKeyPressed(int code) { return isKeyPressed[code]; }
    static bool IsMousePressed(MouseButton btn) { return isMousePressed[(int)btn]; }
    static vec2 GetMousePosition() { return mousePosition; }

private:
    static void ProcessUpdate(double dt);

    static void OnResizeEvent(GLFWwindow* window, int width, int height);
    static void OnKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnMouseClickEvent(GLFWwindow* window, int button, int action, int mods);
    static void OnMouseMoveEvent(GLFWwindow* window, double x, double y);

    static void InitializeSystems();
    static void ShutdownSystems();
    static void UpdateSystems(double dt);
    static void UpdateSystemsNonstop(double dt);

    static void GLFW_ErrorCallback(int errorCode, const char* description);

    static GLFWwindow* window;
    static int windowWidth;
    static int windowHeight;

    static bool isPausing;
    static bool isFinished;
    static double lastTime;
    static int currentFrame;

    static bool isKeyPressed[512];
    static bool isMousePressed[3];
    static vec2 mousePosition;
};

#endif // __CORE_H__
