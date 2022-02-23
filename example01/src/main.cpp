#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

#define WIDTH 960
#define HEIGHT 540

static void error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error (" << error << ") " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main()
{
	if (!glfwInit())
		throw std::runtime_error{ "GLFW initialization failed" };

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
	glfwSetErrorCallback(error_callback);
#endif

	auto window = glfwCreateWindow(WIDTH, HEIGHT, "Example01", nullptr, nullptr);
	if (!window)
		throw std::runtime_error{ "Window creation failed" };


	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* /*window*/, const int width, const int height) noexcept {
		glViewport(0, 0, width, height);
	});
	glfwSetKeyCallback(window,key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error{ "OpenGL initialization failed" };

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << ", "
		<< "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}