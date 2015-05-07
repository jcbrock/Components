//// Include GLEW
//#include <GL/glew.h>
//
//// Include GLFW
//#include <glfw3.h>
//GLFWwindow* window;
//
//// Include GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <common/shader.hpp>
//#include <common/texture.hpp>
//
//// Include GLM
//#include <glm/gtc/type_ptr.hpp>
//#include "glm/gtc/quaternion.hpp"
//#include "glm/gtx/quaternion.hpp"
//
//bool OpenGLInit()
//{
//    
//    // Initialise GLFW
//    if (!glfwInit())
//    {
//        fprintf(stderr, "Failed to initialize GLFW\n");
//        return false;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//
//
//    // Open a window and create its OpenGL context
//    window = glfwCreateWindow(1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
//    if (window == NULL){
//        fprintf(stderr, "Failed to open GLFW window.\n");
//        glfwTerminate();
//        return false;
//    }
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLEW
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        return false;
//    }
//
//    // Ensure we can capture the escape key being pressed below
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//    // Dark blue background
//    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//
//    // Enable depth test
//    glEnable(GL_DEPTH_TEST);
//    // Accept fragment if it closer to the camera than the former one
//    glDepthFunc(GL_LESS);
//    
//
//
//
//
//	/*
//
//    // Initialise GLFW
//    if (!glfwInit())
//    {
//        fprintf(stderr, "Failed to initialize GLFW\n");
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//
//
//    // Open a window and create its OpenGL context
//    window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
//    if (window == NULL){
//        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLEW
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        return -1;
//    }
//
//    // Ensure we can capture the escape key being pressed below
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//    // Dark blue background
//    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//
//    // Create and compile our GLSL program from the shaders
//    GLuint programID = LoadShaders("Shaders/TransformVertexShader.vertexshader", "Shaders/TextureFragmentShader.fragmentshader");
//
//    // Get a handle for our buffers
//    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
//
//    static const GLfloat g_vertex_buffer_data[] = {
//        -1.0f, -1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,
//        0.0f, 1.0f, 0.0f,
//    };
//
//    GLuint vertexbuffer;
//    glGenBuffers(1, &vertexbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//    do{
//
//        // Clear the screen
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Use our shader
//        glUseProgram(programID);
//
//        // 1rst attribute buffer : vertices
//        glEnableVertexAttribArray(vertexPosition_modelspaceID);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(
//            vertexPosition_modelspaceID, // The attribute we want to configure
//            3,                  // size
//            GL_FLOAT,           // type
//            GL_FALSE,           // normalized?
//            0,                  // stride
//            (void*)0            // array buffer offset
//            );
//
//        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//        glDisableVertexAttribArray(vertexPosition_modelspaceID);
//
//        // Swap buffers
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//
//    } // Check if the ESC key was pressed or the window was closed
//    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
//    glfwWindowShouldClose(window) == 0);
//
//
//    // Cleanup VBO
//    glDeleteBuffers(1, &vertexbuffer);
//    glDeleteProgram(programID);
//
//    // Close OpenGL window and terminate GLFW
//    glfwTerminate();
//	*/
//    return true;
//}
