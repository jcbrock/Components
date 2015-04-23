#ifndef RIGIDBODY_INSTANCE_H
#define RIGIDBODY_INSTANCE_H

#include "glm\glm.hpp"
#include <string>

class RigidBody
{
public:
    void Initialize();
    static unsigned int ID;

    RigidBody();
    ~RigidBody();
    RigidBody(const RigidBody&);

    //todo - move constructors
    RigidBody& operator=(const RigidBody&); //this right?

    void DebugPrint();

    std::string mName;
    float mSpeed;
    float mWidth;
    float mHeight;
    glm::vec4 mPositionWorldCoord;
    glm::vec4 mDirection;
    bool mIsCollidable;
    bool mMarkedForDeletion = false;
};



#endif