#include "OpenGLExampleBase.h"

#include <iostream>

OpenGLExampleBase* OpenGLExampleBase::exampleBase = nullptr;

OpenGLExampleBase::OpenGLExampleBase()
{
    exampleBase = this;
}

OpenGLExampleBase::~OpenGLExampleBase()
{
}

void OpenGLExampleBase::handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        exampleBase->onKeyDown(key);
    }

    if(action == GLFW_RELEASE)
    {
        exampleBase->onKeyUp(key);
    }
}

void OpenGLExampleBase::handleMouseButtonCallback(GLFWwindow* window, int key, int action, int modes)
{

}

void OpenGLExampleBase::handleWindowResize(GLFWwindow* window, int width, int height)
{

}

void OpenGLExampleBase::setupWindow()
{
    if (!glfwInit())
    {
		std::cerr <<  "GLFW initialization failed" << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSwapInterval(1);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwSetErrorCallback([](int error, const char* description)
    {
	    std::cerr << "GLFW Error (" << error << ") " << description << std::endl;
    });
#endif

	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLRenderLib", nullptr, nullptr);
	if (!mWindow)
    {
        std::cerr << "Window creation failed!" << std::endl;
        exit(1);
    }

	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, handleWindowResize);
	glfwSetKeyCallback(mWindow, handleKeyCallback);
    glfwSetMouseButtonCallback(mWindow, handleMouseButtonCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "OpenGL initialization failed" << std::endl;
        exit(1);
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << ", " << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void OpenGLExampleBase::onKeyDown(int key)
{

}

void OpenGLExampleBase::onKeyUp(int key)
{

}

void OpenGLExampleBase::onMouseMove()
{
    
}

const std::string OpenGLExampleBase::getShadersPath() const
{
    return getAssetPath() + "shaders/";
}

const std::string OpenGLExampleBase::getTexturePath() const
{
    return getAssetPath() + "textures/";
}

void OpenGLExampleBase::prepare()
{
    
}

void OpenGLExampleBase::renderLoop()
{
	while (!glfwWindowShouldClose(mWindow))
	{
        render();

        glfwSwapBuffers(mWindow);
		glfwPollEvents();
    }

    destroyWindow();
}


void OpenGLExampleBase::destroyWindow()
{
    if(mWindow)
    {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }
}