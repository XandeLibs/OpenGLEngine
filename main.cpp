#include <iostream>

#include "camera.hpp"
#include "input.hpp"
#include "model.hpp"

#include "UI.hpp"
#include "scene.hpp"

int main() {
  // Opengl Init
  const char *glfwError;

  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

  if (!glfwInit()) {
    std::cout << "glfw error: " << std::hex << glfwGetError(&glfwError);
    std::cout << std::endl << glfwError << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  GLFWwindow *window = glfwCreateWindow(800, 600, "GLFWApp", NULL, NULL);
  if (window == NULL) {
    std::cout << "glfw error: " << std::hex << glfwGetError(&glfwError);
    std::cout << std::endl << glfwError << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  UI::setup(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glEnable(GL_DEPTH_TEST);

  stbi_set_flip_vertically_on_load(true);

  // Shader

  Scene::addShader("Default", "vertex", "fragment");
  Scene::addShader("Depth", "vertex", "depth");
  Scene::addShader("Border", "vertexBorder", "border");

  Scene::addModel("backpack/backpack.obj");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    UI::startLoop();

    if (!Scene::render())
      glfwSetWindowShouldClose(window, true);

    UI::endLoop();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  UI::shutdown();
  glfwTerminate();
  return 0;
}
