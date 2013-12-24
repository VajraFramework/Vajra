// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include "Libraries/glm/glm.hpp"
using namespace glm;

#include "ExampleGame/Test/TestFile.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Profiler/Profiler.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Placeholder/Tesserakonteres.h"

#include <fstream>

int main( void ) {

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

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow(1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW) )
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "Vajra" );
	glfwSetWindowPos(0, 0);

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

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

	setupGraphics(FRAMEWORK->GetDeviceProperties()->GetWidthPixels(),
				  FRAMEWORK->GetDeviceProperties()->GetHeightPixels());

	Tesserakonteres::initGameObjectsForScene();
	Tesserakonteres::initUiGameObjects();

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

		renderFrame(1/1000.0f);

		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

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
