#include "Core.h"

#include "Utility.h"
#include "GameObjectSystem/GameObjectSystem.h"
#include "RenderSystem/RenderSystem.h"
#include "ResourceSystem/ResourceSystem.h"
#include "SceneSystem/SceneSystem.h"
#include "Scenes/MainScene.h"

GLFWwindow* Core::window;
int Core::windowWidth;
int Core::windowHeight;

bool Core::isPausing;
bool Core::isFinished;
double Core::lastTime;
int Core::currentFrame;

bool Core::isKeyPressed[512];
bool Core::isMousePressed[3];
vec2 Core::mousePosition;

bool Core::Initialize(const char* windowName, int width, int height)
{
    Random::Initizlie();

    for(int i = 0; i < 512; i++) {
        isKeyPressed[i] = false;
    }
    for(int i = 0; i < 3; i++) {
        isMousePressed[i] = false;
    }

    window = nullptr;
    windowWidth = width;
    windowHeight = height;
    isPausing = false;
    isFinished = false;

    glfwSetErrorCallback(Core::GLFW_ErrorCallback);
    if(glfwInit() != GLFW_TRUE) {
        LOG_ERROR("GLFW Init Error!");
        return false;
    }

    // GLFW window setting
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
    if(!window) {
        LOG_ERROR("Failed to create window");
    }
    printf("WOW\n");

    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, &Core::OnResizeEvent);
    glfwSetKeyCallback(window, &Core::OnKeyboardEvent);
    glfwSetMouseButtonCallback(window, &Core::OnMouseClickEvent);
    glfwSetCursorPosCallback(window, &Core::OnMouseMoveEvent);

    // glad init
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("glad Init Error!");
        return false;
    }
    InitializeSystems();

    return true;
}

void Core::Shutdown()
{
    ShutdownSystems();

    glfwDestroyWindow(window);
    glfwTerminate();

    Random::Shutdown();
}

void Core::Run()
{
    lastTime = glfwGetTime();
    currentFrame = 0;

    // Load initial scene
    SceneSystem::ChangeScene<MainScene>();

    while(isFinished == false && !glfwWindowShouldClose(window)) {
        currentFrame++;

        glfwPollEvents();

        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime;

        /*
        while(dt - 0.02 > 0) {
            ProcessUpdate(0.02);
            dt -= 0.02;
        }*/
        ProcessUpdate(dt);

        RenderSystem::Render();

        lastTime = currentTime;
    }
}

void Core::FinishGame()
{
    isFinished = true;
}

void Core::PauseGame()
{
    isPausing = true;
}

void Core::ResumeGame()
{
    isPausing = false;
    glfwPollEvents();
    lastTime = glfwGetTime();
}

void Core::ProcessUpdate(double dt)
{
    UpdateSystemsNonstop(dt);

    if(isPausing == true) return;

    UpdateSystems(dt);
}

void Core::OnResizeEvent(GLFWwindow* window, int width, int height)
{
    RenderSystem::OnResize(width, height);

    windowWidth = width;
    windowHeight = height;
}

void Core::OnKeyboardEvent(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_UNKNOWN) return;

        isKeyPressed[key] = true;
        SceneSystem::OnKeyPress(key);
    } else if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_UNKNOWN) return;

        isKeyPressed[key] = false;
        SceneSystem::OnKeyRelease(key);
    }
}

void Core::OnMouseClickEvent(GLFWwindow * window, int button, int action, int mods)
{
    if(action == GLFW_PRESS) {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            isMousePressed[(int)MouseButton::Left] = true;
        } else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
            isMousePressed[(int)MouseButton::Middle] = true;
        } else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
            isMousePressed[(int)MouseButton::Right] = true;
        }
    } else if(action == GLFW_RELEASE) {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            isMousePressed[(int)MouseButton::Left] = false;
        } else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
            isMousePressed[(int)MouseButton::Middle] = false;
        } else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
            isMousePressed[(int)MouseButton::Right] = false;
        }
    }
}

void Core::OnMouseMoveEvent(GLFWwindow* window, double x, double y)
{
    mousePosition.x = (float)x;
    mousePosition.y = (float)y;
}

void Core::InitializeSystems()
{
    GameObjectSystem::Initizlie();
    RenderSystem::Initizlie();
    // Dispatch resize event for getting aspect ratio
    RenderSystem::OnResize(windowWidth, windowHeight);
    SceneSystem::OnResize(windowWidth, windowHeight);
    ResourceSystem::Initizlie();
    SceneSystem::Initialize();
}

void Core::ShutdownSystems()
{
    SceneSystem::Shutdown();
    ResourceSystem::Shutdown();
    RenderSystem::Shutdown();
    GameObjectSystem::Shutdown();
}

void Core::UpdateSystems(double dt)
{
    GameObjectSystem::Update(dt);
}

void Core::UpdateSystemsNonstop(double dt)
{
    SceneSystem::Update(dt);
}

void Core::GLFW_ErrorCallback(int errorCode, const char* description)
{
    LOG_ERROR("GLFW Error! (%d)\n%s", errorCode, description);
}
