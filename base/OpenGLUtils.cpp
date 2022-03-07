#include "OpenGLUtils.h"

#include <fstream>
#include <array>
#include <iostream>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace utils
{
    std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size, void *data, BufferFlag flag)
    {
        auto vb = std::make_shared<VertexBuffer>();
        vb->init(size, data, flag);
        return vb;
    }

    void VertexBuffer::init(uint32_t size, void *data, BufferFlag flag)
    {
        mSize = size;
        const bool drawIndirect = 0 != (flag & BufferFlag::BUFFER_DRAW_INDIRECT);

        mTarget = drawIndirect ? GL_DRAW_INDIRECT_BUFFER : GL_ARRAY_BUFFER;

        GL_CHECK(glGenBuffers(1, &mId));
        GL_CHECK(glBindBuffer(mTarget, mId));
        GL_CHECK(glBufferData(mTarget, mSize, data, (nullptr == data) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(mTarget, 0));
    }

    void VertexBuffer::update(uint32_t offset, uint32_t size, void *data)
    {
        if (0 == mId)
        {
            std::cerr << "Updating invalid vertex buffer." << std::endl;
            return;
        }

        GL_CHECK(glBindBuffer(mTarget, mId));
        GL_CHECK(glBufferSubData(mTarget, offset, size, data));
        GL_CHECK(glBindBuffer(mTarget, 0));
    }

    void VertexBuffer::destroy()
    {
        GL_CHECK(glBindBuffer(mTarget, 0));
        GL_CHECK(glDeleteBuffers(1, &mId));
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t size, void *data, BufferFlag flag)
    {
        auto ib = std::make_shared<IndexBuffer>();
        ib->init(size, data, flag);
        return ib;
    }

    void IndexBuffer::init(uint32_t size, void *data, BufferFlag flag)
    {
        mSize = size;
        mFlag = flag;

        GL_CHECK(glGenBuffers(1, &mId));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, (NULL == data) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::update(uint32_t offset, uint32_t size, void *data)
    {
        if (0 == mId)
        {
            std::cerr << "Updating invalid index buffer." << std::endl;
            return;
        }

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId));
        GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::destroy()
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GL_CHECK(glDeleteBuffers(1, &mId));
    }

    std::shared_ptr<OpenglShader> OpenglShader::create(const std::string &filename, GLenum shaderType)
    {
        std::shared_ptr<OpenglShader> shader = std::make_shared<OpenglShader>();
        if (shader->init(filename, shaderType))
        {
            return shader;
        }
        return nullptr;
    }

    OpenglShader::~OpenglShader()
    {
        destroy();
    }

    bool OpenglShader::init(const std::string &filename, GLenum shaderType)
    {
        std::ifstream is;
        is.open(filename);
        if (is.is_open())
        {
            std::string shaderString;
            std::stringstream shaderStream;
            shaderStream << is.rdbuf();
            is.close();
            shaderString = shaderStream.str();

            const char *shaderSource = shaderString.c_str();
            unsigned int shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &shaderSource, nullptr);
            glCompileShader(shader);
            GLint compileStatus;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus != GL_TRUE)
            {
                GLint logLength;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                std::vector<char> infoLog(logLength);
                glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
                std::cerr << "Failed to compile shader. Compile log\n"
                          << infoLog.data() << std::endl;
                return false;
            }
            id = shader;
            type = shaderType;
        }
        else
        {
            std::cerr << "Error: Could not open shader file: " << filename.c_str() << std::endl;
            return false;
        }
    }

    void OpenglShader::destroy()
    {
        if (id != 0)
        {
            GL_CHECK(glDeleteShader(id));
            id = 0;
        }
    }

    std::shared_ptr<OpenglProgram> OpenglProgram::create(std::shared_ptr<OpenglShader> &vertexShader, std::shared_ptr<OpenglShader> &fragmentShader)
    {
        if (!vertexShader && !fragmentShader)
            return nullptr;

        std::shared_ptr<OpenglProgram> program = std::make_shared<OpenglProgram>();
        if (program->init(vertexShader, fragmentShader))
        {
            return program;
        }

        return nullptr;
    }

    OpenglProgram::~OpenglProgram()
    {
        destroy();
    }

    bool OpenglProgram::init(std::shared_ptr<OpenglShader> &vertexShader, std::shared_ptr<OpenglShader> &fragmentShader)
    {
        id = glCreateProgram();
        std::cout << "Program create [" << id << "] "
                  << "vs: " << vertexShader->id << " fs: " << fragmentShader->id << std::endl;

        if (vertexShader->id != 0)
        {
            glAttachShader(id, vertexShader->id);
            if (fragmentShader->id != 0)
            {
                glAttachShader(id, fragmentShader->id);
            }

            glLinkProgram(id);
            GLint linkStatus;
            glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE)
            {
                GLint logLength;
                glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
                std::vector<char> infoLog(logLength);
                glGetProgramInfoLog(id, logLength, nullptr, infoLog.data());
                std::cerr << "Failed to link program. Link log\n"
                          << infoLog.data() << std::endl;

                glDeleteProgram(id);
                id = 0;

                return false;
            }
        }

        return true;
    }

    void OpenglProgram::destroy()
    {
        if (id != 0)
        {
            glDeleteProgram(id);
            id = 0;
        }
    }

    void Image::loadFromFile(const std::string &filename)
    {
        mData = stbi_load(filename.c_str(), &mWidth, &mHeight, &mChannle, 4);
        if (mData == nullptr)
        {
            std::cerr << "Cannot load the image: " << filename << std::endl;
            exit(1);
        }
    }

    Image::~Image()
    {
        if (mData != nullptr)
        {
            free(mData);
        }
    }

    std::shared_ptr<Texture2D> Texture2D::create(const std::string &filename, bool generatedMipmap)
    {
        auto texture = std::make_shared<Texture2D>();
        if (texture->loadFromFile(filename, generatedMipmap))
        {
            return texture;
        }
        return nullptr;
    }

    bool Texture2D::loadFromFile(const std::string &filename, bool generateMipmap)
    {
        GLuint textureID;
        int width;
        int height;
        int channle;
        glGenTextures(1, &textureID);

        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channle, 3);

        if (data)
        {
            glBindTexture(GL_TEXTURE_2D, textureID);

            GLenum internalFormat;
            GLenum format;
            if (channle == 1)
            {
                internalFormat = GL_RED;
                format = GL_RED;
            }
            else if (channle == 3)
            {
                internalFormat = GL_RGB;
                format = GL_RGB;
            }
            else if (channle == 4)
            {
                internalFormat = GL_RGBA;
                format = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            if (generateMipmap)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);

            mId = textureID;
            mWidth = width;
            mHeight = height;
            mChanngle = channle;
            mHasMipmap = generateMipmap;

            return true;
        }
        else
        {
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);
            return false;
        }
    }

    void Texture2D::bind(uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, mId);
    }

    void Texture2D::unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::shared_ptr<Mesh> Mesh::createPlane(float halfExtend)
    {
        auto mesh = std::make_shared<Mesh>();

        mesh->mVertices = {-1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f};
        mesh->mNormals = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
        mesh->mTangents = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
        mesh->mTextCoords = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
        mesh->mIndices = {0, 1, 2, 1, 3, 2};

        for (int32_t i = 0; i < mesh->mVertices.size(); ++i)
        {
            mesh->mVertices[i * 4 + 0] *= halfExtend;
            mesh->mVertices[i * 4 + 1] *= halfExtend;
        }

        return mesh;
    }

    std::shared_ptr<Mesh> Mesh::createPlane(float horizontalExtend, float verticalExtend)
    {
        auto mesh = std::make_shared<Mesh>();

        mesh->mVertices = {-1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f};
        mesh->mNormals = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
        mesh->mTangents = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
        mesh->mTextCoords = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
        mesh->mIndices = {0, 1, 2, 1, 3, 2};

        for (int32_t i = 0; i < mesh->mVertices.size() / 4.0; i++)
        {
            mesh->mVertices[i * 4 + 0] *= horizontalExtend;
            mesh->mVertices[i * 4 + 1] *= verticalExtend;
        }

        return mesh;
    }
}