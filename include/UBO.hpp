#ifndef UBO_H
#define UBO_H

#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <cstddef>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>

struct Uniform {
  std::string name;
  GLenum type;
  int offset, size;
};

template <size_t N>
struct StringLiteral {
  char value[N];
  constexpr StringLiteral(const char (&str)[N]) {
    std::ranges::copy_n(str, N, value);
  }
};

template <typename T>
concept GlmVec = requires {
  // Check that T is a specialization of glm::vec
  []<glm::length_t L, typename U, glm::qualifier Q>(glm::vec<L, U, Q>) {
  }(std::declval<T>());
};

template <typename T>
concept GlmMat = requires {
  []<glm::length_t C, glm::length_t R, typename U, glm::qualifier Q>(
      glm::mat<C, R, U, Q>) {}(std::declval<T>());
};

template <typename T>
concept GlmType = GlmVec<T> || GlmMat<T>;

class UBO {
public:
  UBO(unsigned int programID, std::string_view blockName);
  UBO(unsigned int programID, std::string_view blockName, int arraySize);
  UBO(std::string_view blockName, int arraySize);
  ~UBO();
  inline const unsigned int getBindingPoint() const { return bindingPoint; };
  inline const std::string_view getBlockName() const { return blockName; };

  template <StringLiteral MemberName, typename T>
  void setUBOMember(const T &value) {
    static GLint offset = [this]() {
      if (this->programID == 0) {
        std::cerr << "UBO does not contain shader to get offset from";
        return -1;
      }
      const char *name = MemberName.value;
      GLuint index = glGetProgramResourceIndex(programID, GL_UNIFORM, name);
      GLenum property = GL_OFFSET;
      GLint offset;
      glGetProgramResourceiv(programID, GL_UNIFORM, index, 1, &property, 1,
                             NULL, &offset);
      return offset;
    }();
    memcpy(static_cast<uint8_t *>(bufferPtr) + offset, &value, sizeof(T));
  }

  template <StringLiteral MemberName, GlmType T>
  void setUBOMember(const T &value) {
    static GLint offset = [this]() {
      if (this->programID == 0) {
        std::cerr << "UBO does not contain shader to get offset from";
        return -1;
      }
      const char *name = MemberName.value;
      GLuint index = glGetProgramResourceIndex(programID, GL_UNIFORM, name);
      GLenum property = GL_OFFSET;
      GLint offset;
      glGetProgramResourceiv(programID, GL_UNIFORM, index, 1, &property, 1,
                             NULL, &offset);
      return offset;
    }();
    memcpy(static_cast<uint8_t *>(bufferPtr) + offset, glm::value_ptr(value),
           sizeof(T));
  }

  template <StringLiteral MemberName>
  GLint getUBOMemberOffset() {
    static GLint offset = [this]() {
      if (this->programID == 0) {
        std::cerr << "UBO does not contain shader to get offset from";
        return -1;
      }
      const char *name = MemberName.value;
      GLuint index = glGetProgramResourceIndex(programID, GL_UNIFORM, name);
      GLenum property = GL_OFFSET;
      GLint offset;
      glGetProgramResourceiv(programID, GL_UNIFORM, index, 1, &property, 1,
                             NULL, &offset);
      return offset;
    }();
    return offset;
  }

  std::byte *getBufferPtr() { return static_cast<std::byte *>(bufferPtr); }

private:
  static unsigned int nextBindingPoint;
  unsigned int programID;
  unsigned int ID;
  unsigned int bindingPoint;
  void *bufferPtr;
  std::string blockName;
  std::vector<Uniform> uniforms;
};

extern UBO UBOCamera;

#endif
