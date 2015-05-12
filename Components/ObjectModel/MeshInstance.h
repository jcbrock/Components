#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "../Buffer.h"
#include <GL/glew.h>
#include <string>

// This class is responsible for handling the OpenGL representation of obj
class MeshInstance
{
public:
    static unsigned int ID; //TODO

    MeshInstance();
    ~MeshInstance();
    MeshInstance(const MeshInstance&);
    MeshInstance& operator=(const MeshInstance&);
    MeshInstance(MeshInstance&&);

    void Initialize();
    void DebugPrint();

    // Getters / Setters

    void SetName(const std::string& name);
    void SetVertexData(unsigned int vertBufferSize, GLfloat* data);
    void SetTexture(unsigned int uvBufferSize, GLfloat* data, GLuint textureHandle);

    std::string GetName() const;
    GLuint GetVertBufferHandle() const;
    GLuint GetUVBufferHandle() const;
    GLuint GetTextureHandle() const;
    bool IsMarkedForDeletion() const;

private:
    bool mMarkedForDeletion = false;
    GLuint mTextureHandle;
    Buffer mVertices;
    Buffer mUVBuffer;

private:
    char mName[16]; //stack 15 bytes
};

#endif