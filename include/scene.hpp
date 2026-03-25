#ifndef SCENE_H
#define SCENE_H

#include "camera.hpp"
#include "model.hpp"
#include <map>
#include <string>

class Scene {
public:
  enum RenderType { normal, depth, texture, border, RENDER_COUNT };

  Scene();

  ~Scene();

  RenderType renderType;
  Camera camera;
  float deltaTime;
  float lastFrame;

  void addModel(const std::string &modelPath);
  void addShader(std::string_view name, std::string vertexPath,
                 std::string fragmentPath);

  bool render();

private:
  std::vector<Model *> models;
  std::map<std::string_view, Shader *> shaders;
};

extern Scene scene;

#endif
