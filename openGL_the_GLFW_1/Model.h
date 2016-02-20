#pragma once
#include<iostream>
#include  <string>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
using namespace std;
class Model
{
	public:
		Model(GLchar* path);
		void Draw(Shader shader);
		~Model();

	private:
		vector<Mesh> meshes;
		string directory;

		void loadModel(string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh * mesh, const aiScene *secne);
		vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);

};

