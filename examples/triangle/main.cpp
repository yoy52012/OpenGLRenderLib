#include <iostream>
#include <vector>
#include <array>

#include "OpenGLExampleBase.h"
#include "OpenGLUtils.h"


class TriangleExample : public OpenGLExampleBase
{
public:
	struct Vertex 
	{
		std::array<float, 3> position;
		std::array<float, 3> color;
	};

	explicit TriangleExample()
	{

	}

	~TriangleExample()
	{

	}

	void prepareVertices()
	{
		std::vector<Vertex> vertices =
		{
			{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { 0.5f,  -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
		};

		std::vector<uint32_t> indices = {0 , 1, 2};

		glGenVertexArrays(1,  &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void loadAssets()
	{
		prepareVertices();
	}

	void loadShader()
	{
		auto vertexShader = utils::OpenglShader::create(getShadersPath() + "triangle/triangle.vert", GL_VERTEX_SHADER);
		auto fragemShader = utils::OpenglShader::create(getShadersPath() + "triangle/triangle.frag", GL_FRAGMENT_SHADER);
		mProgram = utils::OpenglProgram::create(vertexShader, fragemShader);
	}

	virtual void prepare() override
	{
		OpenGLExampleBase::prepare();

		loadAssets();
		loadShader();
	}

	virtual void render() override
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(0.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		mProgram->use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}

private:
	unsigned int VAO, VBO, EBO;

	std::shared_ptr<utils::OpenglProgram> mProgram;
};


int main()
{
	TriangleExample* triangleExample = new TriangleExample();
	triangleExample->setupWindow();
	triangleExample->prepare();
	triangleExample->renderLoop();
	
	delete(triangleExample);

    return 0;
}