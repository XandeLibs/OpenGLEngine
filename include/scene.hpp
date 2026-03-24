#ifndef SCENE_H
#define SCENE_H

#include "camera.hpp"
#include <string>

namespace Scene {

enum sceneRenderType { normal, depth, border, RENDER_COUNT };

extern sceneRenderType renderType;
extern Camera camera;
extern float deltaTime;
extern float lastFrame;

void addModel(const std::string &modelPath);
void addShader(std::string_view name, std::string vertexPath,
               std::string fragmentPath);

bool render();
} // namespace Scene

#endif
