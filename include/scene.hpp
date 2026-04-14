#ifndef SCENE_H
#define SCENE_H

#include "camera.hpp"
#include "model.hpp"
#include <map>
#include <string>

class Scene {
public:
  enum RenderType { normal, depth, texture, border, RENDER_COUNT };

  Scene(std::string_view defaultVertexPath,
        std::string_view defaultFragmentPath);

  ~Scene();

  RenderType renderType;
  Camera *camera;
  float deltaTime;
  float lastFrame;
  map<string_view, UBO *> UBOs;

  void addModel(
      const std::string &modelPath,
      const std::vector<glm::mat4> &modelInstances = std::vector<glm::mat4>());
  void addShader(std::string_view name, std::string_view vertexPath,
                 std::string_view fragmentPath);

  bool render();

  void createUBO(Shader &shader, string_view blockName);

  std::map<std::string_view, Shader *> shaders;

private:
  std::vector<Model *> models;

  vector<std::string> skyboxFaces = {"right.jpg",  "left.jpg",  "top.jpg",
                                     "bottom.jpg", "front.jpg", "back.jpg"};

  static constexpr float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  static constexpr float quadVerts[] = {
      -1.0f, 1.0f,  0.0f, 1.0f, //
      -1.0f, -1.0f, 0.0f, 0.0f, //
      1.0f,  -1.0f, 1.0f, 0.0f, //

      -1.0f, 1.0f,  0.0f, 1.0f, //
      1.0f,  -1.0f, 1.0f, 0.0f, //
      1.0f,  1.0f,  1.0f, 1.0f  //
  };

  unsigned int quadVAO, quadVBO;
  unsigned int quadTexture, quadFBO, quadDepthStencilRBO;
  unsigned int resolvedFBO, resolvedTexture;

  unsigned int skyboxVAO, skyboxVBO;
  unsigned int skyboxTexture;

  bool drawPostProcessing();
  unsigned int loadCubemap(vector<std::string> faces);
  void initializeScene();
  void initializeLights();
};

extern Scene *scene;

#endif
