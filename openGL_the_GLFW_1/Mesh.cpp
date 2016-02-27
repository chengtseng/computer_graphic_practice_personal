#include "Mesh.h"
Mesh::Mesh()
{}
Mesh::~Mesh()
{}

void Mesh::read_model(const char* file)
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

	this->vertices = std::vector<Vertex>(3 * numberOfTrianlge);
	this->triangles = std::vector<triangleData>(numberOfTrianlge);

	/*read triangle*/
	for (int i = 0; i < vertices.size(); i += 3)
	{
		fscanf_s(fp, "v0 %f %f %f %f %f %f %d\n",
			&(this->vertices[i].position[0]), &(this->vertices[i].position[1]), &(this->vertices[i].position[2]),
			&(this->vertices[i].normal[0]), &(this->vertices[i].normal[1]), &(this->vertices[i].normal[2]),
			&(color_index[0]));

		fscanf_s(fp, "v1 %f %f %f %f %f %f %d\n",
			&(this->vertices[i + 1].position[0]), &(this->vertices[i + 1].position[1]), &(this->vertices[i + 1].position[2]),
			&(this->vertices[i + 1].normal[0]), &(this->vertices[i + 1].normal[1]), &(this->vertices[i + 1].normal[2]),
			&(color_index[1]));
		fscanf_s(fp, "v2 %f %f %f %f %f %f %d\n",
			&(this->vertices[i + 2].position[0]), &(this->vertices[i + 2].position[1]), &(this->vertices[i + 2].position[2]),
			&(this->vertices[i + 2].normal[0]), &(this->vertices[i + 2].normal[1]), &(this->vertices[i + 2].normal[2]),
			&(color_index[2]));

		fscanf_s(fp, "face normal %f %f %f\n", &(this->triangles[i / 3].face_normal[0]), &(this->triangles[i / 3].face_normal[1]),
			&(this->triangles[i / 3].face_normal[2]));

		this->triangles[i / 3].color[0] = (unsigned char)(int)(255 * (diffuse[color_index[0]].x));
		this->triangles[i / 3].color[1] = (unsigned char)(int)(255 * (diffuse[color_index[0]].y));
		this->triangles[i / 3].color[2] = (unsigned char)(int)(255 * (diffuse[color_index[0]].z));
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
		else if (vertices[i].position[0] <= minX)
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
	this->center[0] = (minX + maxX) / 2;
	this->center[1] = (minY + maxY) / 2;
	this->center[2] = (minZ + maxZ) / 2;
	this->wide = maxX - minX;
	
	std::cout << "wide of model: " << this->wide << std::endl;
	std::cout << "center of the model " << this->center[0] << " " << this->center[1] << " " << this->center[2] << std::endl;
}

void Mesh::setupBuffers() 
{
	/*generate buffer*/
	glGenVertexArrays(1, &(this->VAO));

	glGenBuffers(1, &(this->VBO));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW);

	/*assign position attribute in shader*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(GLfloat)) + sizeof(int), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
}

void Mesh::draw() 
{
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->numberOfTriangle * 3);
	glBindVertexArray(0);
}

glm::mat4 Mesh::loadToCenterOfCamera(Camera cam)
{
	glm::mat4 translate;
	translate = glm::translate(translate,
		glm::vec3(-1 * (this->center[0] - cam.cameraPos[0]), -1 * (this->center[1] - cam.cameraPos[1]), -1 * (this->center[2] - cam.cameraPos[2])));//move camera to the center of obj
	
	translate = glm::translate(translate, glm::vec3(0,0,-1*cam.cameraDistance));
	return translate;
}