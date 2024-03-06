//
// Created by 44772 on 2024/1/30.
//

#include "core/application.h"
#include "core/scene.h"
#include "base/integrator.h"
#include "base/camera.h"
#include "base/film.h"

#include "util/parser.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>

NAMESPACE_BEGIN

GLFWwindow *window = nullptr;
Integer windowWidth{};
Integer windowHeight{};

Application::Application() {
    Object *obj = LoadSceneXML("../testlight.xml");
    pScene = std::unique_ptr<Scene>(dynamic_cast<Scene *>(obj));
    windowWidth = pScene->GetIntegrator()->GetCamera()->GetFilm()->width;
    windowHeight = pScene->GetIntegrator()->GetCamera()->GetFilm()->height;
    InitUI();
}

Application::~Application() = default;

void Application::Run() {

    const auto pIntegrator = pScene->GetIntegrator();
    const auto pFilm = pIntegrator->GetCamera()->GetFilm();
    pFilm->SetCallBack([&](const std::vector<Color3f> &pixels) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_FLOAT,
                     pixels.data());
        glfwSwapBuffers(window);
    });

    while (!glfwWindowShouldClose(window)) {
        pIntegrator->Render();
        glfwPollEvents();
    }

}

void Application::InitUI() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(windowWidth, windowHeight, "Rendering Window", nullptr, nullptr);
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