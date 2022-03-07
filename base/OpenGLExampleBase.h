#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class OpenGLExampleBase
{
public:
    OpenGLExampleBase();
    virtual ~OpenGLExampleBase();

    virtual void setupWindow();

    virtual void destroyWindow();

    virtual void prepare();

    /** @brief Render function to be implemented by the sample application */
	virtual void render() {}

    void renderLoop();

    virtual void onKeyDown(int key);

    virtual void onKeyUp(int key);

    virtual void onMouseMove();

protected:
    const std::string getShadersPath() const;
    const std::string getTexturePath() const;

    const std::string getAssetPath() const 
    {
    #if defined(ROOT_DATA_DIR)
        return ROOT_DATA_DIR;
    #else
        return "./../data/";
    #endif
    }

private:
    static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    static void handleMouseButtonCallback(GLFWwindow* window, int key, int action, int modes);

    static void handleWindowResize(GLFWwindow* window, int width, int height);

protected:
    GLFWwindow* mWindow;

    static OpenGLExampleBase* exampleBase;

    uint32_t mWidth = 1280;
	uint32_t mHeight = 720;
};


