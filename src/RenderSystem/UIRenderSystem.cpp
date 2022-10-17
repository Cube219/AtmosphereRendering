#include "UIRenderSystem.h"

#include "CloudRenderSystem.h"
#include "Core.h"
#include "RenderSystem.h"
#include "SkyRenderSystem.h"

#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

bool UIRenderSystem::show_demo_window;
bool UIRenderSystem::show_another_window;
ImVec4 UIRenderSystem::clear_color;

float UIRenderSystem::sunAngle;
float UIRenderSystem::cloudHeight;

void UIRenderSystem::Initialize()
{
    sunAngle = 30.0f / 180.0f * 3.141593;
    cloudHeight = 0.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Core::GetGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430 core");

    show_demo_window = true;
    show_another_window = false;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // NewFrame function should be called before render ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIRenderSystem::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIRenderSystem::Update(double dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&show_demo_window);

    // static float f = 0.0f;
    // static int counter = 0;
    //
    // ImGui::Begin("Hello, world!");
    //
    // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    // ImGui::Checkbox("Another Window", &show_another_window);
    //
    // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
    //
    // if(ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //     counter++;
    // ImGui::SameLine();
    // ImGui::Text("counter = %d", counter);
    //
    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    // ImGui::End();
    //
    // if(show_another_window)
    // {
    //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //     ImGui::Text("Hello from another window!");
    //     if(ImGui::Button("Close Me"))
    //         show_another_window = false;
    //     ImGui::End();
    // }

    ImGui::Begin("Atmosphere setting");

    ImGui::DragFloat("innerSphereRadius", &CloudRenderSystem::innerSphereRadius, 400);
    ImGui::DragFloat("outerSphereRadius", &CloudRenderSystem::outerSphereRadius, 400);
    ImGui::DragFloat("sphereCenterY", &CloudRenderSystem::sphereCenterY, 400);

    // ImGui::SameLine();

    ImGui::SliderAngle("Sun Angle", &sunAngle, 0, 180);
    RenderSystem::SetLightDir(vec3(0, sin(sunAngle), cos(sunAngle)));

    ImGui::SliderFloat("Cloud Height", &cloudHeight, -30000.0f, 30000.0f);
    CloudRenderSystem::SetHeightFactor(cloudHeight);

    ImGui::End();
}

void UIRenderSystem::SubRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIRenderSystem::IsMouseHovered()
{
    return ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}
