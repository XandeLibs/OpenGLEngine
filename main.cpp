#include <iostream>

#include "camera.hpp"
#include "input.hpp"
#include "model.hpp"

#include "UI.hpp"
#include "scene.hpp"

Scene scene;

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

  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  stbi_set_flip_vertically_on_load(true);

  // Shader
  scene.addShader("Default", "vertex", "fragment");
  scene.addShader("Texture", "vertex", "fragtex");
  scene.addShader("Depth", "vertex", "depth");
  scene.addShader("Border", "vertexBorder", "border");

  scene.addModel("backpack/backpack.obj");
  scene.addModel("panel/panel.obj");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    UI::startLoop();

    if (!scene.render())
      glfwSetWindowShouldClose(window, true);

    UI::endLoop();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  UI::shutdown();
  glfwTerminate();
  return 0;
}
