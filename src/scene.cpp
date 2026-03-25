#include "scene.hpp"
#include <GLFW/glfw3.h>

#include "shader.hpp"

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),    //
    glm::vec3(2.3f, -3.3f, -4.0f),  //
    glm::vec3(-4.0f, 2.0f, -12.0f), //
    glm::vec3(0.0f, 0.0f, -3.0f)    //
};

Scene::Scene() {
  renderType = normal;
  deltaTime = 0.0f;
  lastFrame = 0.0f;
}

Scene::~Scene() {
  for (auto m : models)
    delete m;

  for (auto s : shaders)
    delete s.second;
}

void Scene::addShader(std::string_view name, std::string vertexPath,
                      std::string fragmentPath) {
  vertexPath = "src/shaders/" + vertexPath + ".glsl";
  fragmentPath = "src/shaders/" + fragmentPath + ".glsl";
  Shader *shader = new Shader(name, vertexPath, fragmentPath);
  shaders.insert({shader->name, shader});
}

void Scene::addModel(const std::string &modelPath) {
  Model *model = new Model("assets/" + modelPath);
  models.push_back(model);
}

bool Scene::render() {

  float currentFrame = glfwGetTime();
  Scene::deltaTime = currentFrame - Scene::lastFrame;
  Scene::lastFrame = currentFrame;

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 newModel = glm::mat4(1.0f);

  shaders["Default"]->use();
  shaders["Default"]->setMat4("view", camera.GetViewMatrix());
  shaders["Default"]->setMat4("projection", camera.Projection);
  shaders["Default"]->setVec3("viewPos", camera.Position);

  shaders["Default"]->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shaders["Default"]->setFloat("material.shininess", 32.0f);

  shaders["Default"]->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  shaders["Default"]->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  shaders["Default"]->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  shaders["Default"]->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
  // point light 1
  shaders["Default"]->setVec3("pointLights[0].position",
                              pointLightPositions[0]);
  shaders["Default"]->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
  shaders["Default"]->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
  shaders["Default"]->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
  shaders["Default"]->setFloat("pointLights[0].constant", 1.0f);
  shaders["Default"]->setFloat("pointLights[0].linear", 0.09f);
  shaders["Default"]->setFloat("pointLights[0].quadratic", 0.032f);
  // point light 2
  shaders["Default"]->setVec3("pointLights[1].position",
                              pointLightPositions[1]);
  shaders["Default"]->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
  shaders["Default"]->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
  shaders["Default"]->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
  shaders["Default"]->setFloat("pointLights[1].constant", 1.0f);
  shaders["Default"]->setFloat("pointLights[1].linear", 0.09f);
  shaders["Default"]->setFloat("pointLights[1].quadratic", 0.032f);
  // point light 3
  shaders["Default"]->setVec3("pointLights[2].position",
                              pointLightPositions[2]);
  shaders["Default"]->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
  shaders["Default"]->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
  shaders["Default"]->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
  shaders["Default"]->setFloat("pointLights[2].constant", 1.0f);
  shaders["Default"]->setFloat("pointLights[2].linear", 0.09f);
  shaders["Default"]->setFloat("pointLights[2].quadratic", 0.032f);
  // point light 4
  shaders["Default"]->setVec3("pointLights[3].position",
                              pointLightPositions[3]);
  shaders["Default"]->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
  shaders["Default"]->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
  shaders["Default"]->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
  shaders["Default"]->setFloat("pointLights[3].constant", 1.0f);
  shaders["Default"]->setFloat("pointLights[3].linear", 0.09f);
  shaders["Default"]->setFloat("pointLights[3].quadratic", 0.032f);
  // spotLight
  shaders["Default"]->setVec3("spotLight.position", Scene::camera.Position);
  shaders["Default"]->setVec3("spotLight.direction", Scene::camera.Front);
  shaders["Default"]->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  shaders["Default"]->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  shaders["Default"]->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  shaders["Default"]->setFloat("spotLight.constant", 1.0f);
  shaders["Default"]->setFloat("spotLight.linear", 0.09f);
  shaders["Default"]->setFloat("spotLight.quadratic", 0.032f);
  shaders["Default"]->setFloat("spotLight.cutOff",
                               glm::cos(glm::radians(12.5f)));
  shaders["Default"]->setFloat("spotLight.outerCutOff",
                               glm::cos(glm::radians(15.0f)));

  switch (renderType) {
  case depth:
    shaders["Depth"]->use();
    shaders["Depth"]->updateTRS(camera);
    for (auto m : models) {
      shaders["Default"]->setMat4("model", m->modelMatrix);
      m->Draw(*shaders["Default"]);
    }
    break;
  case border:
    shaders["Default"]->use();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_STENCIL_TEST);

    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shaders["Default"]->updateTRS(camera);
    for (auto m : models) {
      shaders["Default"]->setMat4("model", m->modelMatrix);
      m->Draw(*shaders["Default"]);
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    shaders["Border"]->use();
    shaders["Border"]->updateTRS(camera);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    newModel = glm::translate(newModel, glm::vec3(0.0f, 0.0f, 0.0f));
    newModel = glm::scale(newModel, glm::vec3(1.1f, 1.1f, 1.1f));

    shaders["Border"]->setMat4("model", model);

    for (auto m : models) {
      shaders["Default"]->setMat4("model", m->modelMatrix);
      m->Draw(*shaders["Default"]);
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    break;
  case normal:
    shaders["Default"]->use();
    shaders["Default"]->updateTRS(camera);
    for (auto m : models) {
      shaders["Default"]->setMat4("model", m->modelMatrix);
      m->Draw(*shaders["Default"]);
    }
    break;
  case RENDER_COUNT:
    std::cerr << "invalid render type state: RENDER_COUNT\n";
    return false;
  }

  return true;
}
