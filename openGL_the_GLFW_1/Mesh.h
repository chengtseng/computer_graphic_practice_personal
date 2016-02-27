#pragma once
#include <iostream>
#include <string>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream> 
#include <iomanip>
#include <cstdlib>/
#include "glm/gtx/rotate_vector.hpp"
#include "Camera.h"

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	int colorIndex;
};

struct Vector3D
{
	float x, y, z;
};

struct triangleData
{
	glm::vec3 face_normal;
	glm::vec3 color;
};

class Mesh
{
	public:
		/*vertices and triangle*/
		std::vector<Vertex> vertices;
		std::vector<triangleData> triangles;		
		/*general model information*/
		glm::vec3 center;		
		float wide;
		int numberOfTriangle;
		/*primry color setting of the model*/
		GLfloat R = 1;
		GLfloat G = 1;
		GLfloat B = 1;

		void Mesh::defineCenter();
		void read_model(const char *);
		void setupBuffers();
		void draw();
		glm::mat4 loadToCenterOfCamera(Camera cam);
		Mesh();
		~Mesh();

	private:
		GLuint VBO, VAO;
};

