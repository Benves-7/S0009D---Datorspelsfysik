#pragma once
#include <core/app.h>
#include <vector>
#include <map>
#include "Vector4D.h"

#pragma region MeshResource
using namespace std;
struct Vertex
{
	GLfloat pos[3];
	GLfloat uv[2];
	GLfloat normal[3];
};

class MeshResource
{
public:

	MeshResource()
	{

	}
	~MeshResource()
	{
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void setup(void* ib, void* vb, int ibSize, int vbSize, int numVerticies, int numIndicies)
	{
		setupHandles();
		setupBuffers();
		bind();
		sizeOfIndexBuffer = numIndicies;
		sizeOfVertexBuffer = numVerticies;
		bindVertexBuffer(vb, vbSize);
		bindIndexBuffer(ib, ibSize);
		loadMeshBuffers();
		//unBindBuffers();
	}

	void setupHandles()
	{
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}
	void setupBuffers()
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	void bind() 
	{
		glBindVertexArray(VAO); 
	}
	void bindVertexBuffer(const void* vb, unsigned int vbSize) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vbSize, vb, GL_STATIC_DRAW);
	}
	void bindIndexBuffer(const void* ib, unsigned int ibSize)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibSize, ib, GL_STATIC_DRAW);
	}
	int  getIndexBufferSize()
	{
		return sizeOfIndexBuffer;
	}
	void loadMeshBuffers()
	{
		int offset = 0;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)0);
		offset += sizeof(GLfloat) * 3;
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)offset);
		offset += sizeof(GLbyte) * 4;
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)offset);
		offset += sizeof(GLfloat) * 2;
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)offset);
		offset += sizeof(GLbyte) * 4;
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_BYTE, GL_TRUE, 10 * sizeof(GLbyte), (void*)offset);
		offset += sizeof(GLbyte) * 4;
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, 10 * sizeof(GLbyte), (void*)offset);
		offset += sizeof(GLbyte) * 4;
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)offset);
		glBindVertexArray(0);
	}

	void unBindBuffers()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:

	int sizeOfIndexBuffer;
	int sizeOfVertexBuffer;
	std::vector<Vertex> vertexBuffer;
	std::vector<int> indexBuffer;

	uint32 EBO;
	uint32 VBO;
	uint32 VAO;
};
#pragma endregion