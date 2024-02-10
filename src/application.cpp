//
// Created by 44772 on 2024/1/30.
//

#include "core/application.h"
#include "core/renderer.h"
#include "core/scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>

NAMESPACE_BEGIN

GLFWwindow *window = nullptr;

Application::Application() {
    pRenderer = std::make_unique<Renderer>();
    InitUI();
}

Application::~Application() = default;

void Application::Run() {
    pRenderer->OnInit();

    pRenderer->SetRenderCallback([&](const std::vector<Color3f> &pixels) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(pRenderer->GetScene()->GetWidth(), pRenderer->GetScene()->GetHeight(), GL_RGB, GL_FLOAT,
                     pixels.data());
        glfwSwapBuffers(window);
    });

    std::print("RENDERING BEGIN============================================\n");

    while (!glfwWindowShouldClose(window)) {
        pRenderer->OnRender();
        glfwPollEvents();
    }

    std::print("RENDERING END============================================\n");
}

void Application::InitUI() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(pRenderer->GetScene()->GetWidth(), pRenderer->GetScene()->GetHeight(), "Rendering Window",
                              nullptr, nullptr);
    if (window == nullptr) {
        std::print("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::print("Failed to initialize GLAD\n");
        return;
    }
}

NAMESPACE_END