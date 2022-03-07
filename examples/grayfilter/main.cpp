#include <iostream>
#include <vector>
#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include "OpenGLExampleBase.h"
#include "OpenGLUtils.h"

class GrayfilterExample :  public OpenGLExampleBase
{
public:
    
    explicit GrayfilterExample()
    {

    }

    ~GrayfilterExample()
    {

    }

    void prepare() override
    {
        // create shader
        auto vertexShader = utils::OpenglShader::create(getShadersPath() + "grayfilter/grayfilter.vert", GL_VERTEX_SHADER);
        auto fragementShader = utils::OpenglShader::create(getShadersPath() + "grayfilter/grayfilter.frag", GL_FRAGMENT_SHADER);

        // create program
        mProgram = utils::OpenglProgram::create(vertexShader, fragementShader);

        // get attribute location
        mVertexLocation = glGetAttribLocation(mProgram->id, "a_vertex");
        mTexCoordLocation = glGetAttribLocation(mProgram->id, "a_texCoord");

        // get uniform location
        mMVPMatrixLocation = glGetUniformLocation(mProgram->id, "u_modelViewProjectionMatrix");
        mTextureLocation = glGetUniformLocation(mProgram->id, "u_texture");

        // create texture
        mTexture = utils::Texture2D::create(getTexturePath() + "desert.tga");

        // create plane mesh
        auto plane = utils::Mesh::createPlane(mTexture->getWidth() / 2.0f, mTexture->getHeight() / 2.0f);

        // create buffers
        glGenBuffers(1, &mVerticesBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVerticesBuffer);
        glBufferData(GL_ARRAY_BUFFER, plane->mVertices.size() * sizeof(float), plane->mVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mTexCoordsBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsBuffer);
        glBufferData(GL_ARRAY_BUFFER, plane->mTextCoords.size() * sizeof(float), plane->mTextCoords.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mIndicesBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane->mIndices.size() * sizeof(uint32_t), plane->mIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        
        // create vertext array and bind attribute buffers
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVerticesBuffer);
        glVertexAttribPointer(mVertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(mVertexLocation);

        glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsBuffer);
        glVertexAttribPointer(mTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(mTexCoordLocation);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);

        glBindVertexArray(0);


        // Also bind created texture ...
        //glBindTexture(GL_TEXTURE_2D, mTexture->mId);
        // ... and as this is texture number 0, bind the uniform to the program.
        //glUniform1i(mTextureLocation, 0);


    }

    void render() override
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, mWidth, mHeight);

        // Create the view matrix.
        glm::vec3 position(0.0f, 0.0f, -5.0f);
        glm::vec3 target(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        auto viewMat = glm::lookAt(position, target, up);
        auto projMat = glm::ortho(-(float) mWidth / 2.0f, (float) mWidth / 2.0f, -(float) mHeight / 2.0f, (float) mHeight / 2.0f, 1.0f, 100.0f);

        auto modelViewProjectionMatrix = projMat * viewMat;

        // Pass the model view projection matrix to the current active program.
        glUniformMatrix4fv(mMVPMatrixLocation, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

        mProgram->use();


        glBindVertexArray(mVAO);
        glBindTexture(GL_TEXTURE_2D, mTexture->mId);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


private:
    GLuint mVAO;
    GLuint mVerticesBuffer;
    GLuint mTexCoordsBuffer;
    GLuint mIndicesBuffer;

    GLint mVertexLocation;
    GLint mTexCoordLocation;
    GLint mTextureLocation;
    GLint mMVPMatrixLocation;


    std::shared_ptr<utils::OpenglProgram> mProgram;
    std::shared_ptr<utils::Texture2D> mTexture;

};

int main()
{
	GrayfilterExample* grayfilterExample = new GrayfilterExample();
	grayfilterExample->setupWindow();
	grayfilterExample->prepare();
	grayfilterExample->renderLoop();
	
	delete(grayfilterExample);

    return 0;
}