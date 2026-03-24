#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

#endif
