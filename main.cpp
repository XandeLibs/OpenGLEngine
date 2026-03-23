#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "stb_image.h"

#include "UI.hpp"

float mixval;

Camera camera;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool cursor_enabled = true;
bool firstMouse = true;
bool depthShader = false;

enum renderType { normal, depth, border };

renderType rendering = normal;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  static float lastX = 400, lastY = 300;

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
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

  if (keyPressed(GLFW_KEY_DOWN)) {
    if (mixval > 0.0)
      mixval -= 0.01;
  }

  if (keyPressed(GLFW_KEY_UP))
    if (mixval < 1.0)
      mixval += 0.01;

  if (keyPressed(GLFW_KEY_W))
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keyPressed(GLFW_KEY_S))
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keyPressed(GLFW_KEY_A))
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keyPressed(GLFW_KEY_D))
    camera.ProcessKeyboard(RIGHT, deltaTime);

  if (keyPressed(GLFW_KEY_TAB)) {
    if (!tab_pressed) {
      if (cursor_enabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        firstMouse = true;
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, nullptr);
      }

      cursor_enabled = !cursor_enabled;
      tab_pressed = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    tab_pressed = false;

  if (keyPressed(GLFW_KEY_R))
    rendering = depth;

  if (keyPressed(GLFW_KEY_T))
    rendering = normal;

  if (keyPressed(GLFW_KEY_Y))
    rendering = border;
}

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),    //
    glm::vec3(2.3f, -3.3f, -4.0f),  //
    glm::vec3(-4.0f, 2.0f, -12.0f), //
    glm::vec3(0.0f, 0.0f, -3.0f)    //
};

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

  UI::setup(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);

  stbi_set_flip_vertically_on_load(true);

  // Shader
  Shader shader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  Shader shaderDepth("src/shaders/vertex.glsl", "src/shaders/depth.glsl");
  Shader shaderBorder("src/shaders/vertexBorder.glsl",
                      "src/shaders/border.glsl");

  Model ourModel("assets/backpack/backpack.obj");

  shader.use();

  mixval = 0.2;

  glm::mat4 projection;
  projection =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    UI::startLoop();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // lightPos.x = cos(glfwGetTime()) * lightPos.x;
    // lightPos.y = sin(glfwGetTime()) * lightPos.y;

    glm::mat4 view;
    view = camera.GetViewMatrix();

    shaderDepth.use();
    shaderDepth.setMat4("view", view);
    shaderDepth.setMat4("projection", projection);

    shaderBorder.use();
    shaderBorder.setMat4("view", view);
    shaderBorder.setMat4("projection", projection);

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", camera.Position);

    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);

    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    shader.setVec3("pointLights[0].position", pointLightPositions[0]);
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09f);
    shader.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    shader.setVec3("pointLights[1].position", pointLightPositions[1]);
    shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.09f);
    shader.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    shader.setVec3("pointLights[2].position", pointLightPositions[2]);
    shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.09f);
    shader.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    shader.setVec3("pointLights[3].position", pointLightPositions[3]);
    shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.09f);
    shader.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    glm::mat4 newModel = glm::mat4(1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setMat4("model", model);
    shaderDepth.use();
    shaderDepth.setMat4("model", model);

    switch (rendering) {
    case depth:
      shaderDepth.use();
      ourModel.Draw(shader);
      break;
    case border:
      shader.use();

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glEnable(GL_STENCIL_TEST);

      glEnable(GL_DEPTH_TEST);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
              GL_STENCIL_BUFFER_BIT);

      ourModel.Draw(shader);

      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      glDisable(GL_DEPTH_TEST);
      shaderBorder.use();

      newModel = glm::translate(newModel, glm::vec3(0.0f, 0.0f, 0.0f));
      newModel = glm::scale(newModel, glm::vec3(1.1f, 1.1f, 1.1f));

      shaderBorder.setMat4("model", model);

      ourModel.Draw(shaderBorder);

      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);

      break;
    case normal:
      shader.use();
      ourModel.Draw(shader);
      break;
    }

    UI::endLoop();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  UI::shutdown();
  glfwTerminate();
  return 0;
}
