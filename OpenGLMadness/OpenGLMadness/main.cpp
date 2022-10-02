//#include <glad/glad.h> //has to be this order
//#include <GLFW/glfw3.h>
#include "GLCommon.h"
#include <iostream>


#include "callBackFunctions.h"

#include "ShaderManager/cShader.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cCamera.h"

#include "cGameEngine.h"


#include "cGame.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float bx;
float by;

cGame game;
void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		game.engine.ToggleSelectMode();
	}

	//std::cout << key << "\n";
	if (action == GLFW_PRESS)
	{
		game.engine.PressKey(key);
	}
	
	if (action == GLFW_RELEASE)
	{
		game.engine.ReleaseKey(key);
	}
}





void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	bx = (float)xpos;
	by = (float)ypos;
	game.engine.ProcessMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	game.engine.cameraManager.GetMainCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL is Crazy!", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window not made for some reason" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, GLFW_key_callback);

	glfwSwapInterval(1);



	game.Init(window);



	int winX, winY;

	glfwGetWindowSize(window, &winX, &winY);
	

	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	
	


	
	//start of the render loop
 	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		game.Update();
	
	}


	glfwTerminate();
	return 0;
}