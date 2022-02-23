#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <array>
#include <fstream>

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

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

std::array<float, 12> points = { -0.5f, -0.5f, 0.0f, 1.0f,
								  0.5f, -0.5f, 0.0f, 1.0f,
								  0.0f, 0.5f, 0.0f, 1.0f };


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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window,key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error{ "OpenGL initialization failed" };

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << ", "
		<< "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	std::string vertexString;
	std::ifstream ifs(SHADERFOLDER"/triangle.vert.glsl");
	if(!ifs.good())
		throw std::runtime_error{ std::string("Unable to open shader." )};
	ifs.seekg(0, std::ios::end);
	vertexString.reserve(static_cast<size_t>(ifs.tellg()));
	ifs.seekg(0, std::ios::beg);
	vertexString.assign(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});

	std::string fragmentString;
	std::ifstream ifs1(SHADERFOLDER"/triangle.frag.glsl");
	if (!ifs1.good())
		throw std::runtime_error{ std::string("Unable to open shader.") };
	ifs1.seekg(0, std::ios::end);
	fragmentString.reserve(static_cast<size_t>(ifs1.tellg()));
	ifs1.seekg(0, std::ios::beg);
	fragmentString.assign(std::istreambuf_iterator<char>{ifs1}, std::istreambuf_iterator<char>{});

	const char* vertexSource = vertexString.c_str();
	const char* fragmentSource = fragmentString.c_str();

	int ret;
	std::array<char, 512> infoLog;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ret);
	if (!ret)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog.data());
		std::cout << "VERTEX SHADER COMPILATION FAILED\n" << infoLog.data() << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ret);
	if (!ret)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog.data());
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << infoLog.data() << std::endl;
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &ret);
	if (!ret)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog.data());
		std::cout << "PROGRAM LINK FAILED\n" << infoLog.data() << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(0.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0,3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(program);

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}