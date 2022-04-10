#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLExampleBase.h"
#include "OpenGLUtils.h"

#include <chrono>
#include <memory>
#include <windows.h>

class Timer {
public:
    Timer() : tpStart(std::chrono::high_resolution_clock::now()), tpStop(tpStart) {}
public:
    void start() { tpStart = std::chrono::high_resolution_clock::now(); }
    void stop() { tpStop = std::chrono::high_resolution_clock::now(); }

    auto getMilliseconds() const { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tpStart).count(); }

    auto getSeconds() const { return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - tpStart).count(); }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> tpStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> tpStop;
};

	void mtxRotateXY(float* _result, float _ax, float _ay)
	{
		const float sx = sin(_ax);
		const float cx = cos(_ax);
		const float sy = sin(_ay);
		const float cy = cos(_ay);

		memset(_result, 0, sizeof(float)*16);
		_result[ 0] = cy;
		_result[ 2] = sy;
		_result[ 4] = sx*sy;
		_result[ 5] = cx;
		_result[ 6] = -sx*cy;
		_result[ 8] = -cx*sy;
		_result[ 9] = sx;
		_result[10] = cx*cy;
		_result[15] = 1.0f;
	}

    int64_t getHPCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		int64_t i64 = li.QuadPart;
		return i64;
	}

    int64_t getHPFrequency()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}

class CubesExample : public OpenGLExampleBase
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    CubesExample()
    {

    }

    ~CubesExample()
    {

    }

    void prepare() override
    {
        const std::vector<Vertex> cubeVertices =
        {
            { {-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 0.0f, 1.0f} },
            { { 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
            { {-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
            { { 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
            { {-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
            { { 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
            { {-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
            { { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },
        };

        const uint32_t cubeTriList[] =
        {
            0, 1, 2, // 0
            1, 3, 2,
            4, 6, 5, // 2
            5, 6, 7,
            0, 2, 4, // 4
            4, 2, 6,
            1, 5, 3, // 6
            5, 7, 3,
            0, 4, 1, // 8
            4, 5, 1,
            2, 3, 6, // 10
            6, 3, 7,
        };

        const uint16_t cubeTriStrip[] =
        {
            0, 1, 2,
            3,
            7,
            1,
            5,
            0,
            4,
            2,
            6,
            7,
            4,
            5,
        };

        const uint32_t cubeLineList[] =
        {
            0, 1,
            0, 2,
            0, 4,
            1, 3,
            1, 5,
            2, 3,
            2, 6,
            3, 7,
            4, 5,
            4, 6,
            5, 7,
            6, 7,
        };

        const uint16_t cubeLineStrip[] =
        {
            0, 2, 3, 1, 5, 7, 6, 4,
            0, 2, 6, 4, 5, 7, 3, 1,
            0,
        };

        const uint16_t cubePoints[] =
        {
            0, 1, 2, 3, 4, 5, 6, 7
        };

        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(Vertex), cubeVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mIbo0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo0);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeTriList), cubeTriList, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mIbo1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeTriStrip), cubeTriStrip, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mIbo2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeLineList), cubeLineList, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mIbo3);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo3);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeLineStrip), cubeLineStrip, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mIbo4);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo4);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubePoints), cubePoints, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo2);

        glBindVertexArray(0);

        auto vertexShader   = utils::OpenglShader::create(getShadersPath() + "cubes/cubes.vert", GL_VERTEX_SHADER);
        auto fragmentShader = utils::OpenglShader::create(getShadersPath() + "cubes/cubes.frag", GL_FRAGMENT_SHADER);

        mProgram = utils::OpenglProgram::create(vertexShader, fragmentShader);

        mMVPMatrixLocation = glGetUniformLocation(mProgram->id, "u_modelViewProjectionMatrix");

        mTimer.start();

        m_timeOffset = getHPCounter();

    }


    void render() override
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glViewport(0, 0, mWidth, mHeight);

        const glm::vec3 at  = { 0.0f, 0.0f,   0.0f };
        const glm::vec3 eye = { 0.0f, 0.0f, -35.0f };
        const glm::vec3 up =  { 0.0f, 1.0f,   0.0f };

        const glm::mat4 view = glm::lookAt(eye, at, up);
        const glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(mWidth)/float(mHeight), 0.1f, 100.0f);


        //float time = (float)mTimer.getSeconds();
        float time = (float)((getHPCounter() - m_timeOffset) / double(getHPFrequency()));

        float mtx[16];
        mtxRotateXY(mtx, time + 1.0f * 0.21f, time + 1.0f * 0.37f);
        mtx[12] = -15.0f + float(0) * 3.0f;
        mtx[13] = -15.0f + float(0) * 3.0f;
        mtx[14] = 0.0f;

        auto model = glm::make_mat4(mtx);
        const glm::mat4 mvp = proj * view * model;

        glUniformMatrix4fv(mMVPMatrixLocation, 1, GL_FALSE, &mvp[0][0]);

		mProgram->use();
		glBindVertexArray(mVao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

    }


private:
    GLuint mVao;
    GLuint mVbo;
    GLuint mIbo0;
    GLuint mIbo1;
    GLuint mIbo2;
    GLuint mIbo3;
    GLuint mIbo4;

    GLint mMVPMatrixLocation;

    std::shared_ptr<utils::OpenglProgram> mProgram;

    Timer mTimer;
    int64_t m_timeOffset;
};


int main()
{
    CubesExample cubesExample;
    cubesExample.setupWindow();
    cubesExample.prepare();
    cubesExample.renderLoop();

    return 0;
}