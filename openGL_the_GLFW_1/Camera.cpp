#include "Camera.h"
Camera::Camera()
{
	this->cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->worldUp = glm::vec3(0,1,0);
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->cameraPos, this->cameraPos+cameraFront + this->cameraFront, this->worldUp);
}
