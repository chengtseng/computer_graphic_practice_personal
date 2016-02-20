#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
class Camera
{
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 camRight;
	glm::vec3 worldUp;
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;


	public:	
	Camera();
	glm::mat4 getViewMatrix();

	~Camera();
};

