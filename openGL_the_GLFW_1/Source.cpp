#include<iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


/*function prototype*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	/*create window*/
	int win_width=800;
	int win_height=600;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
	glViewport(0, 0,win_width,win_height);
	glfwSetKeyCallback(window, key_callback);

	/*define vertex for triangle*/
	GLfloat vertices[] = 
	{
		// Positions                   // Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	

		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,
	};

	/*define texture*/	
	GLuint texture1, texture2;
	
	/*Generate Texture 1*/
	int width1, height1;
	unsigned char* image1 = SOIL_load_image("C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/container.jpg", &width1, &height1, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);	
	
	/*Generate Texture 2*/
	int width2, height2;
	unsigned char* image2 = SOIL_load_image("C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/awesomeface.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);	
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*Array Buffer*/
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*assign position attribute in shader*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/*assign texCoord attribute in shader*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	/*import and link our shader program*/
	Shader shaderProgram = Shader("C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/vertex.ver" , "C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/fragment.frag");

	/*Loop*/
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);				
		
		/*use program*/
		shaderProgram.use();

		/*Create transformation matrix every time during interation*/
		glm::mat4 trans;		
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime()*glm::radians(90.0f), glm::vec3(0.0, 0.2, 0.0));
		GLuint transformLoc = glGetUniformLocation(shaderProgram.program,"transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		/*loading texture to the shader's uniform*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shaderProgram.program, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shaderProgram.program, "ourTexture2"), 1);		
		
		/*bind vertex array*/
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,36);
		glBindVertexArray(0);

		/*window swapping*/
		glfwSwapBuffers(window);
	}
	
	/*house keeping*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

