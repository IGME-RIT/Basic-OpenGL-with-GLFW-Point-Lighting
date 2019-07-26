/*
Title: Point Lighting
File Name: shape.h
Copyright ? 2016
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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>


//struct for vertex with uv
struct Vertex3dNormalUV {

    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_texCoord;

    // Makes a 2d vertex with uc and color data.
    Vertex3dNormalUV(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord) {
        m_position = position;
        m_normal = normal;
        m_texCoord = texCoord;
    }
};

class Mesh {


private:
	// Vectors of shape information
	std::vector<Vertex3dNormalUV> m_vertices;
	std::vector<unsigned int> m_indices;

	// Buffered shape info
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;


public:
	// Constructor for a shape, takes a vector for vertices and indices
	Mesh(std::vector<Vertex3dNormalUV> vertices, std::vector<unsigned int> indices);

	// Shape destructor to clean up buffers
	~Mesh();


	// Draws the shape using a given world matrix
	void Draw(glm::mat4 worldMatrix, GLuint uniformLocation);
};