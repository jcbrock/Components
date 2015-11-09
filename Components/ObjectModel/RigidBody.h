#ifndef RIGIDBODY_INSTANCE_H
#define RIGIDBODY_INSTANCE_H

#include "Component.h"

#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class RigidBody : public Component
{
public:
    void Initialize();
    static unsigned int ID;

    RigidBody();
    ~RigidBody();
    RigidBody(const RigidBody&);

    //todo - move constructors
    RigidBody& operator=(const RigidBody&); //this right?

    void DebugPrint() override;
    void Update(double timeDelta) override;

    //std::string mName;
    float mSpeed;
    float mWidth;
    float mHeight;
    glm::vec4 mPositionWorldCoord;
    glm::vec4 mDirection;
    bool mIsCollidable;

    glm::mat4 mMVPForScene;


    //TEMP - move this data to a place that makes more sense... NOT on every rigid body...
    //FOV, height, aspect, near, far
    glm::mat4 mProjection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::vec3 cameraPos = glm::vec3(0, 0, 20);
    glm::mat4 mView = glm::lookAt(
        cameraPos, // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
};



#endif