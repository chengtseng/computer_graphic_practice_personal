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

enum Camera_Movement 
{
	FORWARD, BACKWARD, RIGHT, LEFT,UP, DOWN, 
	ROTATE_BY_X_CW, ROTATE_BY_Y_CW, ROTATE_BY_Z_CW,
	ROTATE_BY_X_CCW, ROTATE_BY_Y_CCW, ROTATE_BY_Z_CCW
};

class Camera
{
	/*Camera distance from the object*/
	public:
		/*camera position*/
		glm::vec3 cameraPos;
		glm::vec3 originalCameraPos;		
		/*three vector of the camera*/
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		const glm::vec3 worldUp;		
		/*Camera distance from the object*/
		int cameraDistance;

		GLfloat cameraSpeed= 0.01;

		Camera(glm::vec3 CPos, glm::vec3 CUp, glm::vec3 CFront);
		glm::mat4 getViewMatrix();
		void processKeyboard(Camera_Movement direction);		

		~Camera();
	private:
		//void updateCameraVector(glm::vec3 CUp, glm::vec3 CFront);
		//void updateCameraPosition(glm::vec3 newPos);
};

