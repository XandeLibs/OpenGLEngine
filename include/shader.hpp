#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "UBO.hpp"
#include "error.hpp"

class Shader {
public:
  std::string name;
  unsigned int ID;

  template <StringLiteral Name> void update(bool value) {
    static GLint loc = getUniformLoc(Name.value);
    glProgramUniform1f(ID, loc, (int)value);
  };

  template <StringLiteral Name> void update(int value) {
    static GLint loc = getUniformLoc(Name.value);
    glProgramUniform1i(ID, loc, value);
  };

  template <StringLiteral Name> void update(float value) {
    static GLint loc = getUniformLoc(Name.value);
    glProgramUniform1f(ID, loc, value);
  };

  template <StringLiteral Name> void update(glm::mat4 value) {
    static GLint loc = getUniformLoc(Name.value);
    glProgramUniformMatrix4fv(ID, loc, 1, GL_FALSE, glm::value_ptr(value));
  };

  template <StringLiteral Name> void update(glm::vec3 value) {
    static GLint loc = getUniformLoc(Name.value);
    glProgramUniform3fv(ID, loc, 1, glm::value_ptr(value));
  };

  template <StringLiteral Name> void update(float v1, float v2, float v3) {
    static GLint loc = getUniformLoc(Name.value);
    glProgramUniform3f(ID, loc, v1, v2, v3);
  };

  Shader(std::string_view name, std::string_view vertexPath,
         std::string_view fragmentPath);

  void use() { glUseProgram(ID); }

  void bindUBO(const UBO *const UBO) {
    auto name = UBO->getBlockName();
    auto blockID = glGetUniformBlockIndex(ID, name.data());
    if (blockID == GL_INVALID_INDEX) {
      std::cerr << "Error getting index of uniform " << name.data() << "\n";
    }
    glUniformBlockBinding(ID, blockID, UBO->getBindingPoint());
  }

private:
  GLint getUniformLoc(std::string_view name) {
    auto loc = glGetUniformLocation(ID, name.data());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    return loc;
  };

  void checkCompileErrors(unsigned int shader, std::string type);
  std::string shaderFromFile(std::string_view shaderPath);
};

struct UniformCache {
  unsigned int shaderID;
  unsigned int location;

  UniformCache(Shader *shader, std::string_view uniformName) {
    shaderID = shader->ID;
    location = glGetUniformLocation(shaderID, uniformName.data());
  }
  void setBool(bool value) const {
    glProgramUniform1i(shaderID, location, (int)value);
  }
  void setInt(int value) const {
    glProgramUniform1i(shaderID, location, value);
  }
  void setFloat(float value) const {
    glProgramUniform1f(shaderID, location, value);
  }
  void setMat4(glm::mat4 value) const {
    glProgramUniformMatrix4fv(shaderID, location, 1, GL_FALSE,
                              glm::value_ptr(value));
  }
  void setVec3(glm::vec3 value) const {
    glProgramUniform3fv(shaderID, location, 1, glm::value_ptr(value));
  }
  void setVec3(float v1, float v2, float v3) const {
    glProgramUniform3f(shaderID, location, v1, v2, v3);
  }
};

#endif
