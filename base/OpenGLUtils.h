
#include <string>
#include <memory>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <cstdint>
#include <vector>


#ifdef __DEBUG
#define GL_CHECK(_call) \
    for(;;) { \
        _call; \
        GLenum gl_err = glGetError(); \
        if(0 != gl_err) {std::cout <<  "GL error("<< gl_err << "): " << glEnumName(gl_err) << "at " << __FILE__ << " (" << __LINE__ << ")" << std::endl; }\
        unused(gl_err); \
        break; \
    } 
#else
#	define GL_CHECK(_call)   _call
#endif

namespace utils
{
    enum BufferFlag : uint32_t
    {
        BUFFER_NONE,
        BUFFER_COMPUTE_READ,
        BUFFER_COMPUTE_WRITE,
        BUFFER_DRAW_INDIRECT,
    };

    struct VertexBuffer
    {
        static std::shared_ptr<VertexBuffer> create(uint32_t size, void* data, BufferFlag flag);

        void init(uint32_t size, void* data, BufferFlag flag);
        void update(uint32_t offset, uint32_t size, void* data);
        void destroy();

        GLuint mId;
        GLuint mSize;
        GLenum mTarget;

    };

    struct IndexBuffer
    {
        static std::shared_ptr<IndexBuffer> create(uint32_t size, void* data, BufferFlag flag);

        void init(uint32_t size, void* data, BufferFlag flag);
        void update(uint32_t offset, uint32_t size, void* data);
        void destroy();

        GLuint mId;
        uint32_t mSize;
        BufferFlag mFlag;
    };

    struct OpenglShader
    {
        GLuint id;
        GLenum type;

        static std::shared_ptr<OpenglShader> create(const std::string& filename, GLenum shaderType);

        ~OpenglShader();

        bool init(const std::string& filename, GLenum shaderType);
        void destroy();
    };

    struct OpenglProgram
    {
        GLuint id = 0;

        GLuint vertex = 0;

        GLuint fragment = 0;

        GLuint geometry = 0;

        GLuint tessellationControl = 0;

        GLuint tessellationEvaluation = 0;

        GLuint compute = 0;

        static std::shared_ptr<OpenglProgram> create(std::shared_ptr<OpenglShader>& vertexShader, std::shared_ptr<OpenglShader>& fragmentShader);

        ~OpenglProgram();
        bool init(std::shared_ptr<OpenglShader>& vertexShader, std::shared_ptr<OpenglShader>& fragmentShader);
        void destroy();

        void use() 
        { 
            glUseProgram(id); 
        }

        void setBool(const std::string &name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const
        { 
            glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const
        { 
            glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const
        { 
            glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
        }
        void setVec2(const std::string &name, float x, float y) const
        { 
            glUniform2f(glGetUniformLocation(id, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        { 
            glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        { 
            glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        { 
            glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
        }
        void setVec4(const std::string &name, float x, float y, float z, float w) 
        { 
            glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    };

    struct Image
    {
    public:
        void loadFromFile(const std::string& filename);

        ~Image();

        int mWidth;
        int mHeight;
        int mDepth;
        int mChannle;
        unsigned char* mData;

        
    };

    struct Texture2D
    {
    public:
        static std::shared_ptr<Texture2D> create(const std::string& filename, bool generatedMipmap = true);

        ~Texture2D() = default;

        void bind(uint32_t slot);

        void unbind();

        const uint32_t getWidth() const  { return mWidth; }
        const uint32_t getHeight() const { return mHeight; }


        uint32_t mId = 0;
        uint32_t mWidth = 0;
        uint32_t mHeight = 0;
        uint32_t mChanngle = 0;
        bool mHasMipmap = false;

    private:
        bool loadFromFile(const std::string& filename, bool generateMipmap);

    };

    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;

        static std::shared_ptr<Mesh> createPlane(float halfExtend);
        
        static std::shared_ptr<Mesh> createPlane(float horizontalExtend, float verticalExtend);

        std::vector<float> mVertices;
        std::vector<float> mNormals;
        std::vector<float> mTangents;
        std::vector<float> mBittangents;
        std::vector<float> mTextCoords;
        std::vector<uint32_t> mIndices;


    };



    
}