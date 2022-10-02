#pragma once
#include <glad/glad.h> //has to be this order
#include <GLFW/glfw3.h>

//resize the render screen to the size of the glfw screen
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


void processInput(GLFWwindow* window);