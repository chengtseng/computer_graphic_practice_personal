#include "Camera.h"
Camera::Camera(glm::vec3 CPos, glm::vec3 CUp, glm::vec3 CFront)
{
	this->cameraPos = CPos;
	this->cameraUp = CUp;
	this->cameraFront = CFront;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->cameraPos, this->cameraPos+cameraFront + this->cameraFront, this->worldUp);
}

void Camera::processKeyboard(Camera_Movement direction)
{
	if ( direction == FORWARD)
	{
		cameraPos += cameraSpeed*cameraFront;
	}
	if (direction == BACKWARD)
	{
		cameraPos -= cameraSpeed*cameraFront;
	}
	if (direction == RIGHT)
	{
		cameraPos -= normalize(glm::cross(cameraUp, cameraFront))*cameraSpeed;
	}
	if (direction == LEFT)
	{
		cameraPos += normalize(glm::cross(cameraUp, cameraFront))*cameraSpeed;
	}
	if (direction == UP)
	{
		cameraPos += cameraUp*cameraSpeed;
	}
	if (direction == DOWN)
	{
		cameraPos -= cameraUp*cameraSpeed;
	}
		
	if (direction == ROTATE_BY_X_CCW)
	{
		cameraFront = glm::normalize(glm::rotate(cameraFront, 0.1f*glm::radians(1.0f), glm::cross(cameraUp, cameraFront)));
		cameraUp = glm::normalize(glm::rotate(cameraUp, 0.1f*glm::radians(1.0f), glm::cross(cameraUp, cameraFront)));
	}
	if (direction == ROTATE_BY_X_CW)
	{
		cameraFront = glm::normalize(glm::rotate(cameraFront, 0.1f*glm::radians(-1.0f), glm::cross(cameraUp, cameraFront)));
		cameraUp = glm::normalize(glm::rotate(cameraUp, 0.1f*glm::radians(-1.0f), glm::cross(cameraUp, cameraFront)));
	}
	if (direction == ROTATE_BY_Y_CCW)
	{
		cameraFront = glm::normalize(glm::rotate(cameraFront, 0.1f*glm::radians(-1.0f), cameraUp));
	}
	if (direction == ROTATE_BY_Y_CW)
	{
		cameraFront = glm::normalize(glm::rotate(cameraFront, 0.1f*glm::radians(1.0f), cameraUp));
	}
	if (direction == ROTATE_BY_Z_CCW)
	{
		cameraUp = glm::rotate(cameraUp, 0.1f*glm::radians(1.0f), cameraFront);
	}
	if (direction == ROTATE_BY_Z_CW)
	{
		cameraUp = glm::rotate(cameraUp, 0.1f*glm::radians(-1.0f), cameraFront);
	}
}

Camera::~Camera()
{
}