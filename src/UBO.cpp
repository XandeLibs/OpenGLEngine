#include "UBO.hpp"
#include "openglsizes.hpp"
#include <error.hpp>
#include <glad/glad.h>

unsigned int UBO::nextBindingPoint = 1;

UBO::UBO(unsigned int programID, std::string_view blockName) {
  glGenBuffers(1, &ID);

  this->blockName = blockName;

  auto blockIndex = glGetUniformBlockIndex(programID, blockName.data());

  ASSERT(blockIndex != GL_INVALID_INDEX, "Block name: " << blockName);

  GLint numUniforms;
  glGetActiveUniformBlockiv(programID, blockIndex,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);

  std::vector<GLint> uniformsIndexes(numUniforms);

  glGetActiveUniformBlockiv(programID, blockIndex,
                            GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                            uniformsIndexes.data());

  std::vector<GLuint> uniformsIdx = {uniformsIndexes.begin(),
                                     uniformsIndexes.end()};

  std::vector<GLint> uniformsOffsets(numUniforms);
  glGetActiveUniformsiv(programID, uniformsIdx.size(), uniformsIdx.data(),
                        GL_UNIFORM_OFFSET, uniformsOffsets.data());

  uniforms.reserve(numUniforms);
  GLsizei totalSize = 0;
  for (unsigned int i = 0; i < uniformsIdx.size(); i++) {
    GLint size;
    GLsizei length;
    GLenum type;
    GLchar name[255];

    glGetActiveUniform(programID, uniformsIdx[i], 255, &length, &size, &type,
                       name);

    int byteSize = getTypeSize(type) * size;
    totalSize += byteSize;

    uniforms.push_back({name, type, uniformsOffsets[i], byteSize});
  }

  glBindBuffer(GL_UNIFORM_BUFFER, ID);
  glBufferData(GL_UNIFORM_BUFFER, totalSize, NULL, GL_DYNAMIC_DRAW);

  bindingPoint = nextBindingPoint;
  nextBindingPoint++;

  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);
}

UBO::~UBO() { glDeleteBuffers(1, &ID); }

void UBO::update(unsigned int index, void *value) {
  ASSERT(index < uniforms.size(),
         "UBO update invalid index, UBO: " << this->blockName
                                           << " index: " << index);
  glBindBuffer(GL_UNIFORM_BUFFER, ID);
  glBufferSubData(GL_UNIFORM_BUFFER, uniforms[index].offset,
                  uniforms[index].size, value);
}
