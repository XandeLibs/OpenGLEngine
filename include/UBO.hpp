#ifndef UBO_H
#define UBO_H

#include <glad/glad.h>
#include <string>
#include <vector>

struct Uniform {
  std::string name;
  GLenum type;
  int offset, size;
};

class UBO {
public:
  UBO(unsigned int programID, std::string_view blockName);
  ~UBO();
  void update(unsigned int index, void *value);
  inline const unsigned int getBindingPoint() const { return bindingPoint; };
  inline const std::string_view getBlockName() const { return blockName; };

private:
  static unsigned int nextBindingPoint;
  unsigned int ID;
  unsigned int bindingPoint;
  std::string blockName;
  std::vector<Uniform> uniforms;
};

extern UBO UBOCamera;

#endif
