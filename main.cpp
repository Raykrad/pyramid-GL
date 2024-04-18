#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void renderPyramid(float rotation, float scale, float posX, float posY, float posZ) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glMultMatrixf(value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mat4 view = lookAt(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(posX, posY, posZ)); // Изменение координаты Z
    model = rotate(model, radians(rotation), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, vec3(scale, scale, scale));
    mat4 mvp = view * model;

    glLoadMatrixf(value_ptr(mvp));

    GLfloat pyramidVertices[] = {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f
    };

    glColor3f(1.0f, 0.0f, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, pyramidVertices);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Pyramid", NULL, NULL);
    if (window == NULL) {
        std::cout << "Epic GLFW Failure" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glEnable(GL_DEPTH_TEST);

    float initialRotation = 0.0f;
    float initialScale = 1.0f;
    float initialPosX = 0.0f;
    float initialPosY = 0.0f;
    float initialPosZ = 0.0f; // Изначальное значение Z координаты

    float rotation = initialRotation;
    float scale = initialScale;
    float posX = initialPosX;
    float posY = initialPosY;
    float posZ = initialPosZ; // Текущее значение Z координаты

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Interaction Panel");

        ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);
        ImGui::SliderFloat("Scale", &scale, 0.1f, 3.0f);
        ImGui::SliderFloat("The X-axis", &posX, -10.0f, 10.0f);
        ImGui::SliderFloat("The Y-axis", &posY, -10.0f, 10.0f);
        ImGui::SliderFloat("The Z-axis", &posZ, -10.0f, 10.0f); // Добавляем слайдер для изменения Z координаты
        if (ImGui::Button("Reset")) {
            rotation = initialRotation;
            scale = initialScale;
            posX = initialPosX;
            posY = initialPosY;
            posZ = initialPosZ; // Сбрасываем Z координату
        }
        ImGui::End();

        renderPyramid(rotation, scale, posX, posY, posZ); // Передаем текущую Z координату в функцию отрисовки

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

