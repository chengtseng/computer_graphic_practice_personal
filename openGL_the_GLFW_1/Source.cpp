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
#include <cstdlib>
#include "glm/gtx/rotate_vector.hpp"
#include "Mesh.h"
#include "Camera.h"

/*global variables*/
bool keyDown[1024];
int win_width = 800;
int win_height = 600;
int renderMode[3] = { 1,1,1 };//1-->1--fill 2--wire 3--dot,2--->1--Cull off 2--Cull, 3-->1--CCW 2--CW 
float near = 1;
float far = 3000;

/*function prototype*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement(Mesh &, Camera&);
void renderModeCheck();
void cullSettingCheck();
void frontFaceCcwCwCheck();
void initializeBuffers(Mesh &);

/*main funciton*/
int main()
{	
	std::string modelName;
	do {
		std::cout << "Enter the name of the model, quit to close the application. Press esc to reload another model: " << std::endl;		
		std::cin >> modelName;
		std::string modelPath = "models/" + modelName;

		/*prepare mesh*/
		Mesh mesh;
		mesh.read_model(modelPath.c_str());	

		/*initialize camera*/
		Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		camera.cameraDistance = mesh.wide*2;
		
		/*create window*/
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Assignment_1_Wei-Cheng_Tseng", nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		/*initialize glew library*/
		glewExperimental = GL_TRUE;
		glewInit();
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Fail to initialize GLEW" << std::endl;
			return -1;
		}
		glViewport(0, 0, win_width, win_height);
		glfwSetKeyCallback(window, key_callback);

		/*enable context*/
		glEnable(GL_DEPTH_TEST);

		/*prepare buffers*/
		mesh.setupBuffers();

		/*declare matrices*/
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		
		/*translate the model to certain distance from the camera*/		
		model = mesh.loadToCenterOfCamera(camera);
				
		/*import and link our shader program*/
		Shader shaderProgram = Shader("vertex.ver", "fragment.frag");

		/*Loop*/
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/*render mode check*/
			renderModeCheck();
			cullSettingCheck();
			frontFaceCcwCwCheck();

			/*use program*/
			shaderProgram.use();
			do_movement(mesh, camera);

			/*view matrix: will be uploaded every time*/
			view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);//build lookAt matrix of current camera		

			/*Porjection Matrix*/
			projection = glm::perspective(glm::radians(45.0f), (float)win_width / win_height, near, far);

			/*load matrix to shader*/
			GLuint modelLoc = glGetUniformLocation(shaderProgram.program, "model");//find the uniform location in the shader program		
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLuint viewLoc = glGetUniformLocation(shaderProgram.program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLuint projectionLoc = glGetUniformLocation(shaderProgram.program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));//all the vertex in the world coordinate will be  perspective*view*model*world

			GLuint rLoc = glGetUniformLocation(shaderProgram.program, "R");
			glUniform1f(rLoc, mesh.R);

			GLuint gLoc = glGetUniformLocation(shaderProgram.program, "G");
			glUniform1f(gLoc, mesh.G);

			GLuint bLoc = glGetUniformLocation(shaderProgram.program, "B");
			glUniform1f(bLoc, mesh.B);
			
			mesh.draw();
			
			/*window swapping*/
			glfwSwapBuffers(window);
		}

		/*house keeping*/	
		glfwTerminate();
	
	} while (modelName != "quit");

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{	
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS)
	{
		keyDown[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keyDown[key] = false;
	}


}
void do_movement(Mesh& mesh,  Camera &camera)
{
	GLfloat cameraSpeed = 0.05f;
	
	if (keyDown[GLFW_KEY_W])
	{
		camera.processKeyboard(FORWARD);
	}
	if (keyDown[GLFW_KEY_S])
	{
		camera.processKeyboard(BACKWARD);
	}
	if (keyDown[GLFW_KEY_D])
	{
		camera.processKeyboard(RIGHT);
	}
	if (keyDown[GLFW_KEY_A])
	{
		camera.processKeyboard(LEFT);
	}
	if (keyDown[GLFW_KEY_UP] && !(keyDown[GLFW_KEY_R] || keyDown[GLFW_KEY_G]||keyDown[GLFW_KEY_B]) )
	{
		camera.processKeyboard(UP);
	}
	if (keyDown[GLFW_KEY_DOWN] && !(keyDown[GLFW_KEY_R] || keyDown[GLFW_KEY_G] || keyDown[GLFW_KEY_B]))
	{
		camera.processKeyboard(DOWN);
	}
	if (keyDown[GLFW_KEY_Y] && !keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		camera.processKeyboard(ROTATE_BY_Y_CCW);
	}
	if (keyDown[GLFW_KEY_Y] && keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		camera.processKeyboard(ROTATE_BY_Y_CW);
	}
	if (keyDown[GLFW_KEY_X] && !keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		camera.processKeyboard(ROTATE_BY_X_CCW);
	}
	if (keyDown[GLFW_KEY_X] && keyDown[GLFW_KEY_LEFT_CONTROL])
	{		
		camera.processKeyboard(ROTATE_BY_X_CW);
	}
	if (keyDown[GLFW_KEY_Z] && !keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		camera.processKeyboard(ROTATE_BY_Z_CCW);
	}
	if (keyDown[GLFW_KEY_Z] && keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		camera.processKeyboard(ROTATE_BY_Z_CW);
	}	
	if (keyDown[GLFW_KEY_I])
	{
		camera.cameraUp = glm::vec3(0,1,0);
		camera.cameraFront= glm::vec3(0, 0, -1);
		camera.cameraPos = camera.originalCameraPos;		
	}
	if (keyDown[GLFW_KEY_R] || keyDown[GLFW_KEY_G] || keyDown[GLFW_KEY_B])
	{
		if (keyDown[GLFW_KEY_R] && keyDown[GLFW_KEY_UP])
		{
			mesh.R+=0.0001;
		}
		if (keyDown[GLFW_KEY_R] && keyDown[GLFW_KEY_DOWN])
		{
			mesh.R -= 0.0001;
		}
		if (keyDown[GLFW_KEY_G] && keyDown[GLFW_KEY_UP])
		{
			mesh.G+=0.0001;
		}
		if (keyDown[GLFW_KEY_G] && keyDown[GLFW_KEY_DOWN])
		{
			mesh.G -= 0.0001;
		}
		if(keyDown[GLFW_KEY_B] && keyDown[GLFW_KEY_UP])
		{
			mesh.B+=0.0001;
		}
		if (keyDown[GLFW_KEY_B] && keyDown[GLFW_KEY_DOWN])
		{
			mesh.B -= 0.0001;
		}
		if (keyDown[GLFW_KEY_B] && keyDown[GLFW_KEY_DOWN])
		{
			mesh.B -= 0.0001;
		}		
	}
	if (keyDown[GLFW_KEY_1])
	{
		renderMode[0] = 1;
	}
	if (keyDown[GLFW_KEY_2])
	{
		renderMode[0] = 2;
	}
	if (keyDown[GLFW_KEY_3])
	{
		renderMode[0] = 3;
	}
	if (keyDown[GLFW_KEY_V])
	{
		renderMode[1] = 1;
	}
	if (keyDown[GLFW_KEY_C])
	{
		renderMode[1] = 2;
	}
	if (keyDown[GLFW_KEY_C] && keyDown[GLFW_KEY_LEFT])
	{
		renderMode[2] = 1;
		
	}
	if (keyDown[GLFW_KEY_C] && keyDown[GLFW_KEY_RIGHT])
	{		
		renderMode[2] = 2;
	}
	if (keyDown[GLFW_KEY_F] && !keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		far++;
	}
	if (keyDown[GLFW_KEY_F] && keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		if (far <= near)
		{
			far = near;
		}
		else 
		{
			far--;
		}		
	}
	if (keyDown[GLFW_KEY_N] && !keyDown[GLFW_KEY_LEFT_CONTROL])
	{		
		if (near >= far)
		{
			near = far;
		}
		else
		{
			near++;
		}		
	}
	if (keyDown[GLFW_KEY_N] && keyDown[GLFW_KEY_LEFT_CONTROL])
	{
		if (near <= 0)
		{
			near = 0;
		}
		else 
		{
			near-=0.1;
		}
	}
}

void renderModeCheck()
{
	if (renderMode[0] == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (renderMode[0] == 2)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (renderMode[0] == 3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(2.0);
	}
}
void cullSettingCheck()
{
	if (renderMode[1] == 1)
	{
		glDisable(GL_CULL_FACE);
	}
	else if (renderMode[1] == 2)
	{
		glEnable(GL_CULL_FACE);
	}
}
void frontFaceCcwCwCheck()
{
	if (renderMode[1] == 2 && renderMode[2] == 1)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}
	else if (renderMode[1] == 2 && renderMode[2] == 2)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}
}


