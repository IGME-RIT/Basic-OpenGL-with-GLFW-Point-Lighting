/*
Title: Point Lighting
File Name: main.cpp
Copyright � 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "FreeImage.h"
#include <vector>
#include "mesh.h"
#include "fpsController.h"
#include "transform3d.h"
#include "shader.h"
#include <iostream>

Mesh* cube;

// The transform being used to draw our shape
Transform3D transform1;
Transform3D transform2;
Transform3D transform3;
Transform3D transform4;

// These shader objects wrap the functionality of loading and compiling shaders from files.
Shader vertexShader;
Shader fragmentShader;

// GL index for shader program
GLuint shaderProgram;

// Index of the world matrix in the vertex shader.
GLuint worldMatrixUniform;

// Index of the camera matrix in the vertex shader.
GLuint viewMatrixUniform;

// Textures also use uniform variables.
GLuint textureUniform;

// Texture buffer index
GLuint texture;

// Store the current dimensions of the viewport.
glm::vec2 viewportDimensions = glm::vec2(800, 600);
glm::vec2 mousePosition = glm::vec2();

FPSController controller = FPSController();

// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
    viewportDimensions = glm::vec2(width, height);
}

// This will get called when the mouse moves.
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
    mousePosition = glm::vec2(mouseX, mouseY);
}


int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "Point Light!", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);

	// Initializes the glew library
	glewInit();


	// At first glance, a cube may appear simple...
    //    [C]------[G]
	// [D]------[H] |
	//	|  |     |  |
	//	|  |     |  |
	//	| [A]----|-[E]
	// [B]------[F]


	std::vector<Vertex3dNormalUV> vertices;
    
    // Here we have vertices ordered by face.
    // In addition to position, and uv, we're now adding normals to our vertices.
    // Normals are vectors that point in the same direction as the surface of the face.
    // We use those directional vectors in the shader to determine how much light is affecting it based on its angle.

    // ABCD
	vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 0)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, -1, 1), glm::vec3(-1, 0, 0), glm::vec2(0, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, 1, -1), glm::vec3(-1, 0, 0), glm::vec2(.5, 0)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, 1, 1), glm::vec3(-1, 0, 0), glm::vec2(.5, .5)));
    // EFGH
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec2(.5, 0)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, 1, -1), glm::vec3(1, 0, 0), glm::vec2(1, 0)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(.5, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec2(1, .5)));
    // ABEF
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(.5, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(.5, 1)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 1)));
    // CDGH
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(.5, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(.5, 1)));
    // ACEG
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(.25, .25)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(.75, .25)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(.25, .75)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(.75, .75)));
    // BDFH
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(.5, 0)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(0, .5)));
    vertices.push_back(Vertex3dNormalUV(glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(.5, .5)));


	std::vector<unsigned int> indices;
    // The vertices are ordered by faces, so we can just add indices one face at a time.
    for (int i = 0; i < 6; i++)
    {
        indices.push_back(i * 4 + 0);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 3);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 1);
    }



	// Create shape object
	cube = new Mesh(vertices, indices);
	

    // Position the cube
	transform1.SetPosition(glm::vec3(0, 0, -5));
    transform2.SetPosition(glm::vec3(-5, 0, -5));
    transform3.SetPosition(glm::vec3(-5, 0, 0));
    transform4.SetPosition(glm::vec3(0, 0, 0));
    


    // Texture Setup:

    // Load the file.
    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType("../assets/texture.png"), "../assets/texture.png");
    // Convert the file to 32 bits so we can use it.
    FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);


    // Create an OpenGL texture.
    glGenTextures(1, &texture);

    // Bind our texture.
    glBindTexture(GL_TEXTURE_2D, texture);

    // Fill our openGL side texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FreeImage_GetWidth(bitmap32), FreeImage_GetHeight(bitmap32),
        0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(FreeImage_GetBits(bitmap32)));

    // We can unload the images now that the texture data has been buffered with opengl
    FreeImage_Unload(bitmap);
    FreeImage_Unload(bitmap32);

    // Set texture sampling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, 0);







	// SHADER STUFF

	// Load and compile shaders.
	vertexShader.InitFromFile("../Assets/vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader.InitFromFile("../Assets/fragment.glsl", GL_FRAGMENT_SHADER);

	// Create a shader program.
	shaderProgram = glCreateProgram();
	vertexShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);
	glLinkProgram(shaderProgram);

	// After the program has been linked, we can ask it where it put our world matrix and camera matrix
	worldMatrixUniform = glGetUniformLocation(shaderProgram, "worldMatrix");
    viewMatrixUniform = glGetUniformLocation(shaderProgram, "cameraView");
    textureUniform = glGetUniformLocation(shaderProgram, "tex");



    std::cout << "Use WASD to move, and the mouse to look around." << std::endl;
    std::cout << "Press escape to exit" << std::endl;




	// Main Loop
	while (!glfwWindowShouldClose(window))
	{

        // Exit when escape is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            break;
        }

        // Calculate delta time.
        float dt = glfwGetTime();
        // Reset the timer.
        glfwSetTime(0);

        

        // Update the player controller
        controller.Update(window, viewportDimensions, mousePosition, dt);


        // rotate square
        transform1.RotateY(-1.0f * dt);
        transform2.RotateY(-1.0f * dt);
        transform3.RotateY(1.0f * dt);
        transform4.RotateY(1.0f * dt);



        // Calculate view matrix.
        glm::mat4 viewMatrix = controller.GetTransform().GetInverseMatrix();

        // Shortcut: use glm perspective matrix.
        glm::mat4 perspectiveProjection= glm::perspective(.7f, viewportDimensions.x / viewportDimensions.y, 1.f, 100.f);

        // Compose view and projection into one matrix to send to the gpu
        glm::mat4 viewProjection = perspectiveProjection * viewMatrix;




        // Clear the color and depth buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 0.0);

		// Set the current shader program.
		glUseProgram(shaderProgram);


        // Send the camera matrix to the shader
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, &(viewProjection[0][0]));

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Use the texture.
        glUniform1i(textureUniform, 0);

		// Draw using the worldMatrixUniform
		cube->Draw(transform1.GetMatrix(), worldMatrixUniform);
        cube->Draw(transform2.GetMatrix(), worldMatrixUniform);
        cube->Draw(transform3.GetMatrix(), worldMatrixUniform);
        cube->Draw(transform4.GetMatrix(), worldMatrixUniform);

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

		// Stop using the shader program.
		glUseProgram(0);

		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}

	// Free memory from shader program and individual shaders
	glDeleteProgram(shaderProgram);


	// Free memory from shape object
	delete cube;

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
