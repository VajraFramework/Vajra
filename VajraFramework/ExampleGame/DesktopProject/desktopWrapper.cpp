// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
// #include <GL/glfw.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

// Include GLM
#include "Libraries/glm/glm.hpp"
using namespace glm;

#include "ExampleGame/Test/TestFile.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Profiler/Profiler.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Placeholder/Tesserakonteres.h"

#include <fstream>

void error_callback(int /* error */, const char* description) {
    fputs(description, stderr);
}

int main( void ) {

	glfwSetErrorCallback(error_callback);

	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

#if defined(PLATFORM_LINUX)
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
#endif // PLATFORM_LINUX

	glfwWindowHint(GLFW_DECORATED, false);

	int width  = 1024;
	int height = 768;
	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(width, height, "Vajra", glfwGetPrimaryMonitor(), nullptr);
	if(window == nullptr) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	Framework::SetGLFWWindow(window);

	glfwMakeContextCurrent(window);

#if 0
	glfwWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
    //glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_FSAA_SAMPLES, 4);
    // glfwDisable( GLFW_AUTO_POLL_EVENTS );
#endif

	 // #ifdef DEBUG
	    // glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	 // #endif


	// Initialize GLEW
	glewExperimental=GL_TRUE;
	int glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle(Framework::GetGLFWWindow(), "Vajra");
	glfwSetWindowPos(Framework::GetGLFWWindow(), 0, 0);

	// Ensure we can capture the escape key being pressed below
	// glfwEnable( GLFW_STICKY_KEYS );

	#if 0
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "./Resources/shaders/SimpleVertexShader.vertexshader", "./Resources/shaders/SimpleFragmentShader.fragmentshader" );

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	#endif

	// TODO [Implement] send the correct DPI
	setupGraphics(width, height, 0);

	Tesserakonteres::initGameObjectsForScene();

	TestFuntion();

	do {
		#if 0
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID, // The attribute we want to configure
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		#endif

		if (renderFrame()) {
			// Swap buffers
			glfwSwapBuffers(Framework::GetGLFWWindow());
		}

	} // Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(Framework::GetGLFWWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	// Print profiler data to file
	std::ofstream logFile;
	logFile.open(FRAMEWORK->GetFileSystemUtils()->GetDeviceLoggingResourcesPath() +
			     FRAMEWORK->GetDeviceProperties()->GetOperatingSystem() + ".log", std::ios_base::out | std::ios_base::app);
	ENGINE->GetProfiler()->PrintAllExperimentData(logFile);
	logFile.close();

	delete ENGINE;

	#if 0
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	#endif

	return 0;
}
