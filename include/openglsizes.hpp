#ifndef OPENGLSIZES_H
#define OPENGLSIZES_H

#include <glad/glad.h>

inline int getTypeSize(GLenum type) {
  switch (type) {
  // --- Scalars ---
  case GL_FLOAT:
    return sizeof(GLfloat);
  case GL_DOUBLE:
    return sizeof(GLdouble);
  case GL_INT:
    return sizeof(GLint);
  case GL_UNSIGNED_INT:
    return sizeof(GLuint);
  case GL_BOOL:
    return sizeof(GLint); // Usually 4 bytes in GLSL

  // --- Vectors (Float) ---
  case GL_FLOAT_VEC2:
    return sizeof(GLfloat) * 2;
  case GL_FLOAT_VEC3:
    return sizeof(GLfloat) * 3;
  case GL_FLOAT_VEC4:
    return sizeof(GLfloat) * 4;

  // --- Vectors (Double) ---
  case GL_DOUBLE_VEC2:
    return sizeof(GLdouble) * 2;
  case GL_DOUBLE_VEC3:
    return sizeof(GLdouble) * 3;
  case GL_DOUBLE_VEC4:
    return sizeof(GLdouble) * 4;

  // --- Vectors (Int) ---
  case GL_INT_VEC2:
    return sizeof(GLint) * 2;
  case GL_INT_VEC3:
    return sizeof(GLint) * 3;
  case GL_INT_VEC4:
    return sizeof(GLint) * 4;

  // --- Vectors (Unsigned Int) ---
  case GL_UNSIGNED_INT_VEC2:
    return sizeof(GLuint) * 2;
  case GL_UNSIGNED_INT_VEC3:
    return sizeof(GLuint) * 3;
  case GL_UNSIGNED_INT_VEC4:
    return sizeof(GLuint) * 4;

  // --- Vectors (Bool) ---
  case GL_BOOL_VEC2:
    return sizeof(GLint) * 2;
  case GL_BOOL_VEC3:
    return sizeof(GLint) * 3;
  case GL_BOOL_VEC4:
    return sizeof(GLint) * 4;

  // --- Matrices (Float) ---
  case GL_FLOAT_MAT2:
    return sizeof(GLfloat) * 4;
  case GL_FLOAT_MAT3:
    return sizeof(GLfloat) * 9;
  case GL_FLOAT_MAT4:
    return sizeof(GLfloat) * 16;
  case GL_FLOAT_MAT2x3:
    return sizeof(GLfloat) * 6;
  case GL_FLOAT_MAT2x4:
    return sizeof(GLfloat) * 8;
  case GL_FLOAT_MAT3x2:
    return sizeof(GLfloat) * 6;
  case GL_FLOAT_MAT3x4:
    return sizeof(GLfloat) * 12;
  case GL_FLOAT_MAT4x2:
    return sizeof(GLfloat) * 8;
  case GL_FLOAT_MAT4x3:
    return sizeof(GLfloat) * 12;

  // --- Matrices (Double) ---
  case GL_DOUBLE_MAT2:
    return sizeof(GLdouble) * 4;
  case GL_DOUBLE_MAT3:
    return sizeof(GLdouble) * 9;
  case GL_DOUBLE_MAT4:
    return sizeof(GLdouble) * 16;
  case GL_DOUBLE_MAT2x3:
    return sizeof(GLdouble) * 6;
  case GL_DOUBLE_MAT2x4:
    return sizeof(GLdouble) * 8;
  case GL_DOUBLE_MAT3x2:
    return sizeof(GLdouble) * 6;
  case GL_DOUBLE_MAT3x4:
    return sizeof(GLdouble) * 12;
  case GL_DOUBLE_MAT4x2:
    return sizeof(GLdouble) * 8;
  case GL_DOUBLE_MAT4x3:
    return sizeof(GLdouble) * 12;

  // --- Samplers ---
  // Samplers are opaque handles. Standard uniforms use 4-byte integers for
  // texture units.
  case GL_SAMPLER_1D:
  case GL_SAMPLER_2D:
  case GL_SAMPLER_3D:
  case GL_SAMPLER_CUBE:
  case GL_SAMPLER_1D_SHADOW:
  case GL_SAMPLER_2D_SHADOW:
  case GL_SAMPLER_1D_ARRAY:
  case GL_SAMPLER_2D_ARRAY:
  case GL_SAMPLER_1D_ARRAY_SHADOW:
  case GL_SAMPLER_2D_ARRAY_SHADOW:
  case GL_SAMPLER_2D_MULTISAMPLE:
  case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
  case GL_SAMPLER_CUBE_SHADOW:
  case GL_SAMPLER_BUFFER:
  case GL_SAMPLER_2D_RECT:
  case GL_SAMPLER_2D_RECT_SHADOW:
  case GL_INT_SAMPLER_1D:
  case GL_INT_SAMPLER_2D:
  case GL_INT_SAMPLER_3D:
  case GL_INT_SAMPLER_CUBE:
  case GL_INT_SAMPLER_1D_ARRAY:
  case GL_INT_SAMPLER_2D_ARRAY:
  case GL_INT_SAMPLER_2D_MULTISAMPLE:
  case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
  case GL_INT_SAMPLER_BUFFER:
  case GL_INT_SAMPLER_2D_RECT:
  case GL_UNSIGNED_INT_SAMPLER_1D:
  case GL_UNSIGNED_INT_SAMPLER_2D:
  case GL_UNSIGNED_INT_SAMPLER_3D:
  case GL_UNSIGNED_INT_SAMPLER_CUBE:
  case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
  case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
  case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
  case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
  case GL_UNSIGNED_INT_SAMPLER_BUFFER:
  case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
    return sizeof(GLint);

  // --- Images ---
  // Image types (GL 4.2+) are also handles/indices.
  case GL_IMAGE_1D:
  case GL_IMAGE_2D:
  case GL_IMAGE_3D:
  case GL_IMAGE_2D_RECT:
  case GL_IMAGE_CUBE:
  case GL_IMAGE_BUFFER:
  case GL_IMAGE_1D_ARRAY:
  case GL_IMAGE_2D_ARRAY:
  case GL_IMAGE_2D_MULTISAMPLE:
  case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
  case GL_INT_IMAGE_1D:
  case GL_INT_IMAGE_2D:
  case GL_INT_IMAGE_3D:
  case GL_INT_IMAGE_2D_RECT:
  case GL_INT_IMAGE_CUBE:
  case GL_INT_IMAGE_BUFFER:
  case GL_INT_IMAGE_1D_ARRAY:
  case GL_INT_IMAGE_2D_ARRAY:
  case GL_INT_IMAGE_2D_MULTISAMPLE:
  case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
  case GL_UNSIGNED_INT_IMAGE_1D:
  case GL_UNSIGNED_INT_IMAGE_2D:
  case GL_UNSIGNED_INT_IMAGE_3D:
  case GL_UNSIGNED_INT_IMAGE_2D_RECT:
  case GL_UNSIGNED_INT_IMAGE_CUBE:
  case GL_UNSIGNED_INT_IMAGE_BUFFER:
  case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
  case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
  case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
  case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
    return sizeof(GLint);

  // --- Miscellaneous ---
  case GL_UNSIGNED_INT_ATOMIC_COUNTER:
    return sizeof(GLuint);

  default:
    return 0;
  }
}
#endif
