#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "../Buffer.h"
#include "glm\glm.hpp"

#include "../misc.h"
// Include GLEW
//#include <GL/glew.h>

#include <memory>


class Buffer;

class MeshInstance
{
public:
    static unsigned int ID;

    MeshInstance();
    ~MeshInstance();
    MeshInstance(const MeshInstance&);
    //todo - move constructors
    MeshInstance& operator=(const MeshInstance&); //this right?

//private:
    //why the fuck does this even need to be a pointer?
    //post construction init I guess
    //easy class size - BUT, memory of class is spread out
    //so might not have good locality. Lets check that
    //Oh dear god, that is correct

    //CAN PROPERTIES BE SHARED ACROSS MESHINSTANCES?
    //CAN THESE BE CHANGED THROUGHOUT THE GAME?
    //keep this shit simple for now, making a fuckin PONG game
    std::unique_ptr<Buffer> mVertices;
    std::unique_ptr<Buffer> mUVBuffer;
    Handle mTextureHandle;

    //this does change throughout the game, not positive
    //this is the right place for it
    glm::mat4 mMVPForScene;
};



#endif