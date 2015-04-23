#ifndef BUFFER_H
#define BUFFER_H

// Include GLEW
#include <GL/glew.h>
// Include standard headers
#include <cstring>


#include <iostream>

class Buffer
{
public:
	//Buffer(){}
	Buffer(unsigned int bufferSize, GLfloat * data) : mBufferSize(bufferSize)
	{
		std::cout << "Buffer made" << std::endl;
		//todo - does size == 0 break?
		mBufferData = new GLfloat[mBufferSize];
		std::memset(mBufferData, 0, mBufferSize);
		std::memcpy(mBufferData, data, mBufferSize);

		//GLuint mBufferHandle;
		glGenBuffers(1, &mBufferHandle); //creates buffer object for vertexbuffer varialbe
	}
	~Buffer()
	{
		std::cout << "Buffer destroyed" << std::endl;
		delete[] mBufferData;
	}

	// handle to GL_ARRAY_BUFFER containing the data for this object
	GLuint mBufferHandle;

	// buffer data (could be vertices, could be uv data)
	GLfloat* mBufferData;

	unsigned int mBufferSize;

private:
	//Buffer(const Buffer&) = delete; //I don't want any copies atm
};

class Buffer2
{
public:
	Buffer2() : mBufferSize(0), mBufferHandle(0), mInitialized(false)
	{
		// Nice for debugging
		std::memset(mBufferData, 0, 500);

	}
	~Buffer2()
	{
		if (mInitialized)
		{
			std::cout << "Buffer destroyed" << std::endl;
			delete[] mBufferData;
		}

	}

	void Initialize(unsigned int bufferSize, GLfloat * data)
	{
		if (!mInitialized)
		{
			if (bufferSize > 500)
			{
				//TODO throw error
				//todo - does size == 0 break?
				return;
			}

			mBufferSize = bufferSize;
			std::memset(mBufferData, 0, bufferSize);
			std::memcpy(mBufferData, data, bufferSize);

			//GLuint mBufferHandle;
			glGenBuffers(1, &mBufferHandle); //creates buffer object for vertexbuffer varialbe

			std::cout << "Buffer made! Now binding it to a GL_ARRAY_BUFFER. Handle: " << mBufferHandle << std::endl;

			//vector of (vertextBufferData or uvBufferData, gl_array_buffer_id it is bound to)
			//could be reused by objects - how to bind to an shape? shape could store index into vector
			//or I could have a key, like a name (or ID), and maybe the above vector is a hashtable?
			//  yeah - i'll do a name for now, but keep that shit UNIQUE.
			glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle); //binds vertexbuffer variable to GL_ARRAY_BUFFER. Future opengl calls will operate on this buffer
			glBufferData(GL_ARRAY_BUFFER, mBufferSize, mBufferData, GL_STATIC_DRAW); //do shit to bound buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0); //clear the binding, so we don't accendently make modifcations to the bound buffer
		}
		mInitialized = true;
	}

	bool mInitialized;

	// handle to GL_ARRAY_BUFFER containing the data for this object
	GLuint mBufferHandle;
	
	unsigned int mBufferSize;

	// buffer data (could be vertices, could be uv data)
	GLfloat mBufferData[500];



private:
	//Buffer(const Buffer&) = delete; //I don't want any copies atm
};

#endif