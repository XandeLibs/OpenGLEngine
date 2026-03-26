#include "input.hpp"

#include "imgui.h"
#include "scene.hpp"

bool cursor_enabled = true;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  scene.camera.Projection = glm::perspective(
      glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  static float lastX = 400, lastY = 300;
  if (!cursor_enabled) {
    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    scene.camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void processInput(GLFWwindow *window) {
  static bool tab_pressed = false;

  auto keyPressed = [window](int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
  };

  if (keyPressed(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(window, true);

  if (keyPressed(GLFW_KEY_F))
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if (keyPressed(GLFW_KEY_G))
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if (keyPressed(GLFW_KEY_W))
    scene.camera.ProcessKeyboard(FORWARD, scene.deltaTime);
  if (keyPressed(GLFW_KEY_S))
    scene.camera.ProcessKeyboard(BACKWARD, scene.deltaTime);
  if (keyPressed(GLFW_KEY_A))
    scene.camera.ProcessKeyboard(LEFT, scene.deltaTime);
  if (keyPressed(GLFW_KEY_D))
    scene.camera.ProcessKeyboard(RIGHT, scene.deltaTime);
  if (keyPressed(GLFW_KEY_Q))
    scene.camera.ProcessKeyboard(UP, scene.deltaTime);
  if (keyPressed(GLFW_KEY_E))
    scene.camera.ProcessKeyboard(DOWN, scene.deltaTime);

  if (keyPressed(GLFW_KEY_TAB)) {
    if (!tab_pressed) {
      if (cursor_enabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        firstMouse = true;
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
      }

      cursor_enabled = !cursor_enabled;
      tab_pressed = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    tab_pressed = false;
}
