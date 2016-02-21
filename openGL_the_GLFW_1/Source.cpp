#include<iostream>
#include  <string>
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

/*global variables*/
bool keyDown[1024];
int win_width = 800;
int win_height = 600;
GLfloat lastX = win_width / 2.0;
GLfloat lastY = win_height / 2.0;
bool firstMouse = true;

/*define original camera variables*/
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 originalCameraPos;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f,-1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
int cameraDistance;

struct Vector3D
{
	float x, y, z;
};

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	int colorIndex;
};

struct triangleData
{
	glm::vec3 face_normal;
	glm::vec3 color;
};

class Mesh
{
	public:
	std::vector<Vertex> vertices;
	std::vector<triangleData> triangles;
	void Mesh::defineCenter();
	void read_model(char *);	
	glm::vec3 center;
	float wide;
	int numberOfTriangle;
	GLfloat R = 1;
	GLfloat G = 1;
	GLfloat B = 1;
	
	private:	
};

/*function prototype*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement(Mesh &, glm::mat4&);
void Mesh::read_model(char* file)
{
	/*mesh requierd data*/
	int color_index[3];
	float shine[1];
	char ch;
	Vector3D ambient[3], diffuse[3], specular[3];
	
	

	/*open file*/
	FILE *fp;
	errno_t err;
	if ((err = fopen_s(&fp, file, "r")) != 0)
	{
		if (fp == NULL) { printf("ERROR: unable to open TriObj [%s]!\n", file); exit(1); }
	}

	/*skip firsr line*/
	fscanf_s(fp, "%c", &ch);

	while (ch != '\n')
		fscanf_s(fp, "%c", &ch);
	
	int numberOfTrianlge;
	fscanf_s(fp, "# triangles = %d\n", &numberOfTrianlge);
	this->numberOfTriangle = numberOfTrianlge;
	
	int materialCount;
	fscanf_s(fp, "Material count = %d\n", &materialCount);

	float dummy;
	for (int i = 0; i < materialCount; i++)
	{
		fscanf_s(fp, "ambient color %f %f %f\n", &dummy, &dummy, &dummy);
		fscanf_s(fp, "diffuse color %f %f %f\n", &dummy, &dummy, &dummy);
		fscanf_s(fp, "specular color %f %f %f\n", &dummy, &dummy, &dummy);
		fscanf_s(fp, "material shine %f\n", &dummy);
	}

	/*skip another line*/
	fscanf_s(fp, "%c", &ch);
	while (ch != '\n')							
		fscanf_s(fp, "%c", &ch);

	printf("Reading in %s (%d triangles). . .\n", file, numberOfTrianlge);
	
	this->vertices = std::vector<Vertex> (3* numberOfTrianlge);
	this->triangles = std::vector<triangleData>(numberOfTrianlge);
	
	/*read triangle*/
	for (int i = 0; i < vertices.size(); i+=3)
	{
		fscanf_s(fp, "v0 %f %f %f %f %f %f %d\n",
			&(this->vertices[i].position[0]), &(this->vertices[i].position[1]), &(this->vertices[i].position[2]),
			&(this->vertices[i].normal[0]), &(this->vertices[i].normal[1]), &(this->vertices[i].normal[2]),
			&(color_index[0]));
		
		fscanf_s(fp, "v1 %f %f %f %f %f %f %d\n",
			&(this->vertices[i+1].position[0]), &(this->vertices[i+1].position[1]), &(this->vertices[i+1].position[2]),
			&(this->vertices[i+1].normal[0]), &(this->vertices[i+1].normal[1]), &(this->vertices[i+1].normal[2]),
			&(color_index[1]));
		fscanf_s(fp, "v2 %f %f %f %f %f %f %d\n",
			&(this->vertices[i+2].position[0]), &(this->vertices[i+2].position[1]), &(this->vertices[i+2].position[2]),
			&(this->vertices[i+2].normal[0]), &(this->vertices[i+2].normal[1]), &(this->vertices[i+2].normal[2]),
			&(color_index[2]));
		
		fscanf_s(fp, "face normal %f %f %f\n", &(this->triangles[i/3].face_normal[0]), &(this->triangles[i/3].face_normal[1]),
			&(this->triangles[i/3].face_normal[2]));		

		this->triangles[i/3].color[0] = (unsigned char)(int)(255 * (diffuse[color_index[0]].x));
		this->triangles[i/3].color[1] = (unsigned char)(int)(255 * (diffuse[color_index[0]].y));
		this->triangles[i/3].color[2] = (unsigned char)(int)(255 * (diffuse[color_index[0]].z));
		
	}
	fclose(fp);
	this->defineCenter();
	
}

void Mesh::defineCenter() 
{
	float minX = vertices[0].position[0];
	float maxX = vertices[0].position[0];
	float minY = vertices[0].position[1];
	float maxY = vertices[0].position[1];
	float minZ = vertices[0].position[2];
	float maxZ = vertices[0].position[2];
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].position[0]>maxX) 
		{
			maxX = vertices[i].position[0];
		}
		else if (vertices[i].position[0]<=minX) 
		{
			minX = vertices[i].position[0];
		}

		if (vertices[i].position[1]>maxY)
		{
			maxY = vertices[i].position[1];
		}
		else if (vertices[i].position[1] <= minY)
		{
			minY = vertices[i].position[1];
		}
		if (vertices[i].position[2]>maxZ)
		{
			maxZ = vertices[i].position[2];
		}
		else if (vertices[i].position[2] <= minZ)
		{
			minZ = vertices[i].position[2];
		}
	}
	std::cout << maxX << " "<<minX<<" " << maxY << " " << minY <<" "<< maxZ << " " << minZ <<std::endl;
	this->center[0] = (minX + maxX )/ 2;
	this->center[1] = (minY + maxY) / 2;
	this->center[2] = (minZ + maxZ) / 2;
	this->wide = maxX - minX;
	cameraDistance = wide / 2 * 10;
	std::cout <<"center of model: "<< this->center[0] << " " << this->center[1] << " " << this->center[2]<<std::endl;
	std::cout << "wide of model: " << this->wide <<std::endl;
	
}

/*main funciton*/
int main()
{	
	Mesh mesh;
	mesh.read_model("C:/Users/Wei-Cheng/Desktop/models/cube.in");
	
	/*create window*/
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(win_width, win_height, "LearnOpenGL", nullptr, nullptr);
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
	
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	
	
	glEnable(GL_DEPTH_TEST);
	
	/*Array Buffer*/
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &(mesh.vertices[0]), GL_STATIC_DRAW);
	
	
	/*assign position attribute in shader*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(GLfloat))+sizeof(int), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	
	glBindVertexArray(0);
	
	/*declare matrices*/
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	
	/*translate the camera to the center of the model*/	
	glm::mat4 translate;
	translate = glm::translate(translate, glm::vec3(-1 * (mesh.center[0] - cameraPos[0]), -1 * (mesh.center[1] - cameraPos[1]), -1 * (mesh.center[2] - cameraPos[2])));//move camera to the center of obj
	view = translate * view;							//build the view matrix for this frame
	cameraPos += mesh.center - cameraPos;				//update camer pos	to the cneter of the model	
	cameraPos[2] += cameraDistance*0.4;
	originalCameraPos = cameraPos;
	/*import and link our shader program*/
	Shader shaderProgram = Shader
		("C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/vertex.ver", 
		"C:/Users/Wei-Cheng/Documents/Visual Studio 2015/Projects/openGL_the_GLFW_1/fragment.frag");
	
	
	
	
	
	
	
	/*Loop*/
	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		
		/*use program*/
		shaderProgram.use();
		do_movement(mesh, view);
		/*view matrix: will be uploaded every time*/
		
		
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//build lookAt matrix of current camera		
		
		/*Porjection Matrix*/
		projection = glm::perspective(glm::radians(45.0f), (float)win_width / win_height, 0.1f, 120000.0f);
		
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

		//glRenderMode(GL_LINE);
		/*bind vertex array*/
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh.numberOfTriangle*3);
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

//glm::vec3 a = glm::cross(cameraFront, cameraUp);
//std::cout<<"origin camer Front: " << cameraFront[0] << " " << cameraFront[1] << " " << cameraFront[2] << std::endl;
//std::cout <<a[0]<<" " << a[1] <<" "<< a[2]<<std::endl;

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
void do_movement(Mesh& mesh,  glm::mat4& view)
{
	GLfloat cameraSpeed = 0.01f;
	
	if (keyDown[GLFW_KEY_W])
	{
		cameraPos +=cameraSpeed*cameraFront;		
	}
	if (keyDown[GLFW_KEY_S])
	{
		cameraPos -= cameraSpeed*cameraFront;
		std::cout << cameraPos[0] << " " << cameraPos[1] <<" " << cameraPos[2]<<std::endl;
	}
	if (keyDown[GLFW_KEY_D])
	{
		cameraPos -= normalize(glm::cross(cameraUp, cameraFront))*cameraSpeed;
	}
	if (keyDown[GLFW_KEY_A])
	{
		cameraPos += normalize(glm::cross(cameraUp, cameraFront))*cameraSpeed;		
	}
	if (keyDown[GLFW_KEY_Y])
	{
		glm::mat4 a;
		cameraFront = glm::normalize( glm::rotate(cameraFront, 0.1f*glm::radians(1.0f), cameraUp));
		
	}
	if (keyDown[GLFW_KEY_X])
	{		
		cameraFront = glm::normalize(glm::rotate(cameraFront, 0.1f*glm::radians(1.0f), glm::cross(cameraUp, cameraFront)));
		cameraUp = glm::normalize(glm::rotate(cameraUp, 0.1f*glm::radians(1.0f), glm::cross(cameraUp, cameraFront)));	
	}
	if (keyDown[GLFW_KEY_Z])
	{		
		cameraUp = glm::rotate(cameraUp, 0.1f*glm::radians(-1.0f),  cameraFront);
		std::cout <<"cameraFront: "<< cameraFront[0] << " " << cameraFront[1] << " " << cameraFront[2] << " " << std::endl;
		std::cout << "cameraUp: " << cameraUp[0] << " " << cameraUp[1] << " " << cameraUp[2] << " " << std::endl;
	}
	if (keyDown[GLFW_KEY_I])
	{
		cameraUp = glm::vec3(0,1,0);
		cameraFront= glm::vec3(0, 0, -1);
		cameraPos = originalCameraPos;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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
		if (keyDown[GLFW_KEY_F])
		{

			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}

