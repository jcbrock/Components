#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "ObjectModel/MeshInstance.h"
#include "ObjectModel/RigidBody.h"
#include <memory>

#include "misc.h"

// Include GLEW
//#include <GL/glew.h>

class GameObject2
{
public:
   static unsigned int ID;

   GameObject2();
   ~GameObject2();
   GameObject2(const GameObject2&);
   //todo - move constructors
   GameObject2& operator=(const GameObject2&); //this right?

   //Not quite sure of this interface is right, but for now I'm
   //assuming all objects have to have vert/texture data
   void InitializeMeshInstance(
       unsigned int vertBufferSize, float * vertData,
       unsigned int uvBufferSize, float * uvData,
       Handle textureId);

   const std::unique_ptr<MeshInstance>& GetMeshInstance() const
   {
       return mMeshInstance;
   }

   void InitializeRigidBodyInstance(
       float width, float height,
       glm::vec4 position, glm::vec4 direction,
       float speed);
   void SetSpeed(float newSpeed);
   void SetDirection(glm::vec4 newDirection);

   const std::unique_ptr<RigidBody>& GetRigidBody() const
   {
       return mRigidBodyInstance;
   }


private:
    //transform    
    std::unique_ptr<MeshInstance> mMeshInstance;
    std::unique_ptr<RigidBody> mRigidBodyInstance;

};



#endif