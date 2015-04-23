#ifndef OPEN_GL_MANAGER_H
#define OPEN_GL_MANAGER_H

#include <GL/glew.h>

class OpenGLManager
{

public:
    //void Initialize(unsigned int maxSize);

    bool OpenGLInit();
    void InitOtherStuff();

    //In shader.cpp
    GLuint LoadProgramShaders(const char * vertex_file_path, const char * fragment_file_path);
    //In texture.cpp
    GLuint LoadProgramDDS(const char * imagePath);

    GLuint programID = -1;
    GLuint mMVPMatrixInputHandle = -1;
    GLuint mVertexInputHandle = -1;
    GLuint mUVCoordinateInputHandle = -1;
    GLuint mTextureInputHandle = -1;
    GLuint Texture = -1;
    /*
private:

    void* mMeshInstanceArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    //unsigned int highestAllocatedIndex;
	unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;

	// maybe this isn't the best place, but then again, this is kinda the point
	// of having a mgr classs like this we can use all this stuff once
	glm::mat4 mProjection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	
	// Camera matrix
	glm::vec3 cameraPos = glm::vec3(0, 0, 10);
	glm::mat4 mView = glm::lookAt(
		cameraPos, // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)



    //1024 bytes / sizeof rigidbodies - that is how many elements my bit array needs to be. w/e, i'll create it extra large now
    */
    };

#endif