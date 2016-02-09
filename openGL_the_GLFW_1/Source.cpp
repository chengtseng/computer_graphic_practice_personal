#include<iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
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

	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Fail to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetKeyCallback(window, key_callback);

	/*define vertex for triangle*/
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};	

	/*VAO*/
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/*VBO*/
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	
	/*link the shader with input*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	/*enable vertex attribute array*/
	glEnableVertexAttribArray(0);
	
	/*link the shader with input*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	/*enable vertex attribute array*/
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shaderProgram = Shader("C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/vertex.ver" , "C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/fragment.frag");

	/*Loop*/
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//std::cout << glfwGetTime() << std::endl;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
				
		
		/*use program*/
		shaderProgram.use();
		


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
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

