#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <span>
#include <stb_image.h>

#include <mesh.hpp>
#include <shader.hpp>

#include <string>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory,
                             bool gamma = false);

class Model {
public:
  // model data
  vector<Texture>
      textures_loaded; // stores all the textures loaded so far, optimization to
                       // make sure textures aren't loaded more than once.
  vector<Mesh> meshes;
  string directory;
  bool gammaCorrection;

  // constructor, expects a filepath to a 3D model.
  Model(string_view path, int instanceCount, bool gamma = false)
      : gammaCorrection(gamma), instanceCount(instanceCount) {
    modelMatrices.reserve(instanceCount);
    modelMatricesUBO = new UBO("instances", 20 * sizeof(glm::mat4));
    loadModel(path);
  }

  ~Model() { delete modelMatricesUBO; }

  // draws the model, and thus all its meshes
  void Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(shader, instanceCount);
  }

  void addMatrix(glm::mat4 modelMatrix);
  void addMatrices(std::vector<glm::mat4> modelMatrices);

  const std::span<glm::mat4> getModelMatrices() { return modelMatrices; }

  void updateModelMatrices();
  const UBO *const getModelUBO() { return modelMatricesUBO; }

private:
  std::vector<glm::mat4> modelMatrices;
  int instanceCount;
  UBO *modelMatricesUBO;

  // loads a model with supported ASSIMP extensions from file and stores the
  // resulting meshes in the meshes vector.
  void loadModel(string_view path);

  // processes a node in a recursive fashion. Processes each individual mesh
  // located at the node and repeats this process on its children nodes (if
  // any).
  void processNode(aiNode *node, const aiScene *scene);

  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  // checks all material textures of a given type and loads the textures if
  // they're not loaded yet. the required info is returned as a Texture struct.
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
};

#endif
