#include <iostream>

#include "error.hpp"
#include "input.hpp"
#include "model.hpp"

#include "UI.hpp"
#include "scene.hpp"

GLFWwindow *window;

int openglInit() {
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

  window = glfwCreateWindow(800, 600, "GLFWApp", NULL, NULL);
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

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, nullptr);

  glViewport(0, 0, 800, 600);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  stbi_set_flip_vertically_on_load(true);

  return 0;
}

int main() {
  int result = openglInit();
  if (result != 0)
    return result;

  // Scene and Shader
  scene = new Scene("vertex", "fragment");

  scene->shaders["Default"]->bindUBO(scene->UBOs["Lights"]);

  scene->addShader("Texture", "vertex", "fragtex");
  scene->shaders["Texture"]->bindUBO(scene->camera->getUBO());

  scene->addShader("Depth", "vertex", "depth");
  scene->shaders["Depth"]->bindUBO(scene->camera->getUBO());

  scene->addShader("Border", "vertexBorder", "border");
  scene->shaders["Border"]->bindUBO(scene->camera->getUBO());

  scene->addShader("Screen", "vertScreen", "fragScreen");
  scene->addShader("Skybox", "vertSkybox", "fragSkybox");
  // scene->shaders["Skybox"]->bindUBO(scene->camera->getUBO());

  auto modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

  scene->addModel("backpack/backpack.obj", {modelMatrix});
  scene->addModel("ground/ground.obj", {modelMatrix});

  std::vector<glm::mat4> modelInstances;

  modelInstances.push_back(modelMatrix);

  modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
  modelInstances.push_back(modelMatrix);

  modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 2.0f, 0.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
  modelInstances.push_back(modelMatrix);

  modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 2.0f, 0.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
  modelInstances.push_back(modelMatrix);

  scene->addModel("panel/panel.obj", modelInstances);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    UI::startLoop();

    if (!scene->render())
      glfwSetWindowShouldClose(window, true);

    UI::endLoop();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete scene;
  UI::shutdown();
  glfwTerminate();
  return 0;
}
