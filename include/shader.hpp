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

  Shader(std::string_view name, std::string_view vertexPath,
         std::string_view fragmentPath);

  void use() { glUseProgram(ID); }
  void setBool(const std::string &name, bool value) const {
    auto loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    glUniform1i(loc, (int)value);
  }
  void setInt(const std::string &name, int value) const {
    auto loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    glUniform1i(loc, value);
  }
  void setFloat(const std::string &name, float value) const {
    auto loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    glUniform1f(loc, value);
  }
  void setMat4(const std::string &name, glm::mat4 value) const {
    auto loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
  }
  void setVec3(const std::string &name, glm::vec3 value) const {
    auto loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    glUniform3fv(loc, 1, glm::value_ptr(value));
  }

  void setVec3(const std::string &name, float v1, float v2, float v3) const {
    auto loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
      std::cerr << "Uniform not found\n"
                << __LINE__ << "\n name: " << name << "\n";
      abort();
    }
    glUniform3f(loc, v1, v2, v3);
  }

  void bindUBO(const UBO *const UBO) {
    auto name = UBO->getBlockName();
    auto blockID = glGetUniformBlockIndex(ID, name.data());
    glUniformBlockBinding(ID, blockID, UBO->getBindingPoint());
  }

private:
  void checkCompileErrors(unsigned int shader, std::string type);
  std::string shaderFromFile(std::string_view shaderPath);
};

#endif
