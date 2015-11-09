#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "Component.h"
#include "../Buffer.h"
#include <GL/glew.h>
#include <string>

// This class is responsible for handling the OpenGL representation of obj
class MeshInstance : public Component
{
public:
    static unsigned int ID; //TODO

    MeshInstance();
    ~MeshInstance();
    MeshInstance(const MeshInstance&);
    MeshInstance& operator=(const MeshInstance&);
    MeshInstance(MeshInstance&&);

    void Initialize();
    void DebugPrint() override;
    void Update(double timeDelta) override;

    // Getters / Setters

    void SetVertexData(unsigned int vertBufferSize, GLfloat* data);
    void SetTexture(unsigned int uvBufferSize, GLfloat* data, GLuint textureHandle);

    GLuint GetVertBufferHandle() const;
    GLuint GetUVBufferHandle() const;
    GLuint GetTextureHandle() const;

private:
    GLuint mTextureHandle;
    Buffer mVertices;
    Buffer mUVBuffer;
};

#endif