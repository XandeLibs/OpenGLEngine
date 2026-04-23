#include "UBO.hpp"
#include <error.hpp>
#include <glad/glad.h>

unsigned int UBO::nextBindingPoint = 1;

UBO::UBO(unsigned int programID, std::string_view blockName) {
  this->blockName = blockName;
  this->programID = programID;

  bindingPoint = nextBindingPoint;
  nextBindingPoint++;

  glGenBuffers(1, &ID);
  glBindBuffer(GL_UNIFORM_BUFFER, ID);

  const GLbitfield flags =
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

  GLint blockSize;
  auto blockIndex = glGetUniformBlockIndex(programID, blockName.data());

  glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE,
                            &blockSize);

  glBufferStorage(GL_UNIFORM_BUFFER, blockSize, nullptr, flags);

  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);

  bufferPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, blockSize, flags);
}

UBO::UBO(unsigned int programID, std::string_view blockName, int arraySize) {
  this->blockName = blockName;
  this->programID = programID;

  bindingPoint = nextBindingPoint;
  nextBindingPoint++;

  glGenBuffers(1, &ID);
  glBindBuffer(GL_UNIFORM_BUFFER, ID);

  const GLbitfield flags =
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

  glBufferStorage(GL_UNIFORM_BUFFER, arraySize, nullptr, flags);

  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);

  bufferPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, arraySize, flags);
}

UBO::UBO(std::string_view blockName, int arraySize) {
  this->blockName = blockName;
  programID = 0;

  bindingPoint = nextBindingPoint;
  nextBindingPoint++;

  glGenBuffers(1, &ID);
  glBindBuffer(GL_UNIFORM_BUFFER, ID);

  const GLbitfield flags =
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

  glBufferStorage(GL_UNIFORM_BUFFER, arraySize, nullptr, flags);

  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);

  bufferPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, arraySize, flags);
}

UBO::~UBO() { glDeleteBuffers(1, &ID); }
