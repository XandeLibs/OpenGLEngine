#include "scene.hpp"
#include <GLFW/glfw3.h>

#include "UBO.hpp"
#include "shader.hpp"

Scene *scene = NULL;

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),    //
    glm::vec3(2.3f, -3.3f, -4.0f),  //
    glm::vec3(-4.0f, 2.0f, -12.0f), //
    glm::vec3(0.0f, 0.0f, -3.0f)    //
};

Scene::Scene(std::string_view defaultVertexPath,
             std::string_view defaultFragmentPath) {
  addShader("Default", defaultVertexPath, defaultFragmentPath);
  UBO *cameraUBO = new UBO(shaders["Default"]->ID, "Transforms");
  camera = new Camera(cameraUBO);
  auto ubo = camera->getUBO();
  shaders["Default"]->bindUBO(ubo);
  shaders["Default"]->update<"material.shininess">(32.0f);

  renderType = normal;
  deltaTime = 0.0f;
  lastFrame = 0.0f;

  createUBO(*shaders["Default"], "Lights");
  initializeLights();
  initializeScene();
}

Scene::~Scene() {
  for (auto m : models)
    delete m;

  for (auto s : shaders)
    delete s.second;

  delete camera;
  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteTextures(1, &quadTexture);

  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &skyboxVBO);
  glDeleteTextures(1, &skyboxTexture);

  glDeleteFramebuffers(1, &quadFBO);
  glDeleteFramebuffers(1, &resolvedFBO);
  glDeleteRenderbuffers(1, &quadDepthStencilRBO);
}

void Scene::initializeScene() {
  glGenRenderbuffers(1, &quadDepthStencilRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, quadDepthStencilRBO);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 800,
                                   600);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenFramebuffers(1, &quadFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, quadFBO);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, quadDepthStencilRBO);

  glGenTextures(1, &quadTexture);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, quadTexture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 800, 600,
                          GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D_MULTISAMPLE, quadTexture, 0);

  glGenVertexArrays(1, &quadVAO);
  glBindVertexArray(quadVAO);

  glGenBuffers(1, &quadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));

  // skybox
  glGenVertexArrays(1, &skyboxVAO);
  glBindVertexArray(skyboxVAO);

  glGenBuffers(1, &skyboxVBO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  skyboxTexture = loadCubemap(skyboxFaces);

  glGenFramebuffers(1, &resolvedFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO);

  glGenTextures(1, &resolvedTexture);
  glBindTexture(GL_TEXTURE_2D, resolvedTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         resolvedTexture, 0);
}

void Scene::initializeLights() {
  auto lightsUBO = UBOs["Lights"];

  lightsUBO->setUBOMember<"dirLight.direction">(glm::vec3(-0.2f, -1.0f, -0.3f));
  lightsUBO->setUBOMember<"dirLight.ambient">(glm::vec3(0.05f, 0.05f, 0.05f));
  lightsUBO->setUBOMember<"dirLight.diffuse">(glm::vec3(0.4f, 0.4f, 0.4f));
  lightsUBO->setUBOMember<"dirLight.specular">(glm::vec3(0.5f, 0.5f, 0.5f));

  // point light 1
  lightsUBO->setUBOMember<"pointLights[0].position">(pointLightPositions[0]);
  lightsUBO->setUBOMember<"pointLights[0].ambient">(
      glm::vec3(0.05f, 0.05f, 0.05f));
  lightsUBO->setUBOMember<"pointLights[0].diffuse">(
      glm::vec3(0.8f, 0.8f, 0.8f));
  lightsUBO->setUBOMember<"pointLights[0].specular">(
      glm::vec3(1.0f, 1.0f, 1.0f));
  lightsUBO->setUBOMember<"pointLights[0].constant">(1.0f);
  lightsUBO->setUBOMember<"pointLights[0].linear">(0.09f);
  lightsUBO->setUBOMember<"pointLights[0].quadratic">(0.032f);

  // point light 2
  lightsUBO->setUBOMember<"pointLights[1].position">(pointLightPositions[1]);
  lightsUBO->setUBOMember<"pointLights[1].ambient">(
      glm::vec3(0.05f, 0.05f, 0.05f));
  lightsUBO->setUBOMember<"pointLights[1].diffuse">(
      glm::vec3(0.8f, 0.8f, 0.8f));
  lightsUBO->setUBOMember<"pointLights[1].specular">(
      glm::vec3(1.0f, 1.0f, 1.0f));
  lightsUBO->setUBOMember<"pointLights[1].constant">(1.0f);
  lightsUBO->setUBOMember<"pointLights[1].linear">(0.09f);
  lightsUBO->setUBOMember<"pointLights[1].quadratic">(0.032f);

  // point light 3
  lightsUBO->setUBOMember<"pointLights[2].position">(pointLightPositions[2]);
  lightsUBO->setUBOMember<"pointLights[2].ambient">(
      glm::vec3(0.05f, 0.05f, 0.05f));
  lightsUBO->setUBOMember<"pointLights[2].diffuse">(
      glm::vec3(0.8f, 0.8f, 0.8f));
  lightsUBO->setUBOMember<"pointLights[2].specular">(
      glm::vec3(1.0f, 1.0f, 1.0f));
  lightsUBO->setUBOMember<"pointLights[2].constant">(1.0f);
  lightsUBO->setUBOMember<"pointLights[2].linear">(0.09f);
  lightsUBO->setUBOMember<"pointLights[2].quadratic">(0.032f);

  // point light 4
  lightsUBO->setUBOMember<"pointLights[3].position">(pointLightPositions[3]);
  lightsUBO->setUBOMember<"pointLights[3].ambient">(
      glm::vec3(0.05f, 0.05f, 0.05f));
  lightsUBO->setUBOMember<"pointLights[3].diffuse">(
      glm::vec3(0.8f, 0.8f, 0.8f));
  lightsUBO->setUBOMember<"pointLights[3].specular">(
      glm::vec3(1.0f, 1.0f, 1.0f));
  lightsUBO->setUBOMember<"pointLights[3].constant">(1.0f);
  lightsUBO->setUBOMember<"pointLights[3].linear">(0.09f);
  lightsUBO->setUBOMember<"pointLights[3].quadratic">(0.032f);

  // spotLight
  lightsUBO->setUBOMember<"spotLight.position">(camera->Position);
  lightsUBO->setUBOMember<"spotLight.direction">(camera->Front);
  lightsUBO->setUBOMember<"spotLight.ambient">(glm::vec3(0.0f, 0.0f, 0.0f));
  lightsUBO->setUBOMember<"spotLight.diffuse">(glm::vec3(1.0f, 1.0f, 1.0f));
  lightsUBO->setUBOMember<"spotLight.specular">(glm::vec3(1.0f, 1.0f, 1.0f));
  lightsUBO->setUBOMember<"spotLight.constant">(1.0f);
  lightsUBO->setUBOMember<"spotLight.linear">(0.09f);
  lightsUBO->setUBOMember<"spotLight.quadratic">(0.032f);
  lightsUBO->setUBOMember<"spotLight.cutOff">(glm::cos(glm::radians(12.5f)));
  lightsUBO->setUBOMember<"spotLight.outerCutOff">(
      glm::cos(glm::radians(15.0f)));
}

void Scene::addShader(std::string_view name, std::string_view vertexPath,
                      std::string_view fragmentPath) {
  std::string vertexAssetPath = "src/shaders/";
  vertexAssetPath.append(vertexPath);
  vertexAssetPath.append(".glsl");

  std::string fragmentAssetPath = "src/shaders/";
  fragmentAssetPath.append(fragmentPath);
  fragmentAssetPath.append(".glsl");

  Shader *shader = new Shader(name, vertexAssetPath, fragmentAssetPath);
  shaders.insert({shader->name, shader});
}

void Scene::addModel(const std::string &modelPath,
                     const std::vector<glm::mat4> &modelInstances) {
  Model *model = new Model("assets/" + modelPath, modelInstances);
  model->modelInstances = modelInstances;
  models.push_back(model);
}

bool Scene::render() {
  float currentFrame = glfwGetTime();
  Scene::deltaTime = currentFrame - Scene::lastFrame;
  Scene::lastFrame = currentFrame;

  glBindFramebuffer(GL_FRAMEBUFFER, quadFBO);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 newModel = glm::mat4(1.0f);

  shaders["Default"]->update<"viewPos">(camera->Position);

  auto lightsUBO = scene->UBOs["Lights"];
  lightsUBO->setUBOMember<"spotLight.position">(Scene::camera->Position);
  lightsUBO->setUBOMember<"spotLight.direction">(Scene::camera->Front);
  switch (renderType) {
  case depth:
    shaders["Depth"]->use();
    for (auto m : models) {
      shaders["Depth"]->update<"model">(m->modelMatrix);
      m->Draw(*shaders["Depth"]);
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

    for (auto m : models) {
      shaders["Default"]->update<"model">(m->modelMatrix);
      m->Draw(*shaders["Default"]);
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    shaders["Border"]->use();

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    newModel = glm::translate(newModel, glm::vec3(0.0f, 0.0f, 0.0f));
    newModel = glm::scale(newModel, glm::vec3(1.1f, 1.1f, 1.1f));

    shaders["Border"]->update<"model">(model);

    for (auto m : models) {
      shaders["Default"]->update<"model">(m->modelMatrix);
      m->Draw(*shaders["Default"]);
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    break;
  case normal:
    shaders["Default"]->use();
    for (auto m : models) {
      if (!m->instanced) {
        shaders["Default"]->update<"model">(m->modelMatrix);
        m->Draw(*shaders["Default"]);
      }
    }
    shaders["Instanced"]->use();
    for (auto m : models) {
      if (m->instanced) {
        m->Draw(*shaders["Instanced"]);
      }
    }
    break;
  case texture:
    shaders["Texture"]->use();
    for (auto m : models) {
      shaders["Texture"]->update<"model">(m->modelMatrix);
      m->Draw(*shaders["Texture"]);
    }
    break;
  case RENDER_COUNT:
    std::cerr << "invalid render type state: RENDER_COUNT\n";
    return false;
  }

  glDepthFunc(GL_LEQUAL);
  shaders["Skybox"]->use();
  shaders["Skybox"]->update<"view">(camera->View);

  shaders["Skybox"]->update<"projection">(camera->Projection);
  glBindVertexArray(skyboxVAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LESS);

  return drawPostProcessing();
}

bool Scene::drawPostProcessing() {

  glBindFramebuffer(GL_READ_FRAMEBUFFER, quadFBO);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolvedFBO);
  glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT,
                    GL_NEAREST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shaders["Screen"]->use();
  glBindVertexArray(quadVAO);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, resolvedTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  return true;
}

unsigned int Scene::loadCubemap(vector<std::string> faces) {
  stbi_set_flip_vertically_on_load(false);

  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    const std::string face = "assets/skybox/" + faces[i];
    unsigned char *data =
        stbi_load(face.data(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i]
                << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  stbi_set_flip_vertically_on_load(true);
  return textureID;
}

void Scene::createUBO(Shader &shader, string_view blockName) {
  UBO *newUBO = new UBO(shader.ID, blockName);
  UBOs.insert({blockName, newUBO});
}
