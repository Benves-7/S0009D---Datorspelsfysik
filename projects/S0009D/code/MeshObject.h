#pragma once
#include "stb_image.h"

#include <string>
#include <vector>
#include <map>
#include <cassert>

#include "math.h"
#include "Square.h"

#define assertm(exp, msg) assert(((void)msg, exp))

using namespace std;
struct Vertex
{
	float pos[3];
	float uv[2];
	float normal[3];
};
struct MeshInfo
{
	vector<Vector4D> vertices;
	vector<Vector4D> uvs;
	vector<Vector4D> normals;
	vector<Vertex> combinedBuffer;
	vector<int> indices;

};
struct TextureInfo
{
	int width, height, numComponents;
	unsigned char* imageData;
	unsigned int texture;
};

class MeshObject
{
public:
	MeshObject(char* meshName, char* textureName, Vector4D pos = Vector4D(0,0,0), Vector4D dir = Vector4D(0,0,-1), Vector4D sca = Vector4D(1,1,1))
	{
		position = pos;
		direction = dir;
		scale = sca;

		assert(loadMesh(meshName), "Mesh failed to load");
		assert(loadTexture(textureName), "Texture failed to load");
		makeAABB();
		

		setup();
	}
	~MeshObject()
	{

	}

	void draw(Matrix4D viewMatrix)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texInfo.texture);

		glUseProgram(program);

		unsigned int location = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(location, 1, GL_TRUE, viewMatrix.getPointer());

		Matrix4D pos = Matrix4D::getPositionMatrix(position);
		Matrix4D rot = Matrix4D::getRotationMatrix(direction);
		Matrix4D sca = Matrix4D::getScaleMatrix(scale);
		Matrix4D model = pos * rot * sca;

		location = glGetUniformLocation(program, "position");
		glUniformMatrix4fv(location, 1, GL_TRUE, pos.getPointer());

		location = glGetUniformLocation(program, "rotation");
		glUniformMatrix4fv(location, 1, GL_TRUE, rot.getPointer());

		location = glGetUniformLocation(program, "scale");
		glUniformMatrix4fv(location, 1, GL_TRUE, sca.getPointer());


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, meshInfo.combinedBuffer.size() * sizeof(Vertex), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	vector<Square> getAABB()
	{
		return AABB_squares;
	}

	Vector4D getPosition()
	{
		return position;
	}
	void setPosition(Vector4D pos)
	{
		position = pos;
		updateAABB();
	}

	Vector4D getDirection()
	{
		return direction;
	}
	void setDirection(Vector4D dir)
	{
		direction = dir;
		updateAABB();
	}

	Vector4D getScale()
	{
		return scale;
	}
	void setScale(Vector4D sca)
	{
		scale = sca;
		updateAABB();
	}

	MeshInfo getMeshInfo()
	{
		return meshInfo;
	}

	Vector4D getAABBDimention()
	{
		return AABB_dimention;
	}
	Vector4D getAABBPosition()
	{
		return AABB_position;
	}

private:
	// Graphics variables.
	bool loadMesh(const char* fileName)
	{
		vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		vector<Vector4D> temp_vertices;
		vector<Vector4D> temp_uvs;
		vector<Vector4D> temp_normals;

		FILE* file = fopen(fileName, "r");
		if (file == NULL)
			return false;
		while (true)
		{
			char lineHead[128];
			int res = fscanf(file, "%s", lineHead);
			if (res == EOF) // End Of File
				break;

			if (strcmp(lineHead, "v") == 0) // Vertex
			{
				Vector4D vertex;
				fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHead, "vt") == 0) // Texture
			{
				Vector4D uv;
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHead, "vn") == 0) // Normals
			{
				Vector4D normal;
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHead, "f") == 0)
			{
				string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				assert(matches == 9, "File is to complex for this parser.");

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		int indexCount = 0;
		map<string, int> checkup;
		
		meshInfo.vertices = temp_vertices;
		meshInfo.uvs = temp_uvs;
		meshInfo.normals = temp_normals;

		Vertex temp;
		float t[3];

		for (int i = 0; i < vertexIndices.size(); i++)
		{
			string check = to_string(vertexIndices[i]) + "/" + to_string(uvIndices[i]) + "/" + to_string(normalIndices[1]);
			if (checkup.count(check) <= 0)
			{
				meshInfo.indices.push_back(indexCount);
				checkup.insert(pair<string, int>(check, indexCount));

				temp.pos[0] = meshInfo.vertices[vertexIndices[i] - 1][0];
				temp.pos[1] = meshInfo.vertices[vertexIndices[i] - 1][1];
				temp.pos[2] = meshInfo.vertices[vertexIndices[i] - 1][2];

				temp.uv[0] = meshInfo.uvs[uvIndices[i] - 1][0];
				temp.uv[1] = meshInfo.uvs[uvIndices[i] - 1][1];
				
				temp.normal[0] = meshInfo.normals[normalIndices[i] - 1][0];
				temp.normal[1] = meshInfo.normals[normalIndices[i] - 1][1];
				temp.normal[2] = meshInfo.normals[normalIndices[i] - 1][2];
				
				meshInfo.combinedBuffer.push_back(temp);

				indexCount++;
			}
			else
			{
				meshInfo.indices.push_back(checkup.at(check));
			}
		}
		return true;
	}
	bool loadTexture(const char* fileName)
	{
		glGenTextures(1, &texInfo.texture);
		glBindTexture(GL_TEXTURE_2D, texInfo.texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texInfo.imageData = stbi_load(fileName, &texInfo.width, &texInfo.height, &texInfo.numComponents, 0);
		if (texInfo.imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texInfo.width, texInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texInfo.imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			return false;
		}
		stbi_image_free(texInfo.imageData);
		return true;
	}
	void setup()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshInfo.combinedBuffer.size(), &meshInfo.combinedBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * meshInfo.indices.size(), &meshInfo.indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));
		glBindVertexArray(0);

		//Shaders
		//Setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		std::string tempString = loadVertexShader();
		vs = tempString.c_str();
		GLint length = static_cast<GLint>(strlen(vs));
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		//Error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
		//Setup pixel shader
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		tempString = loadFragmentShader();
		fs = tempString.c_str();
		length = static_cast<GLint>(strlen(fs));
		glShaderSource(this->fragmentShader, 1, &fs, &length);
		glCompileShader(this->fragmentShader);

		//Error log
		shaderLogSize;
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->fragmentShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
	}
	string loadFragmentShader()
	{
		ifstream file;
		file.open("fs.shader");

		if (file.fail()) {
			cout << "Failed to load fragmentShader" << endl;
			return 0;
		}
		else {
			stringstream tempstream;
			tempstream << file.rdbuf();
			string temp = tempstream.str();
			return temp;
		}
	}
	string loadVertexShader()
	{
		ifstream file;
		file.open("vs.shader");

		if (file.fail()) {
			cout << "Failed to load vertexShader" << endl;
			return 0;
		}
		else {
			stringstream tempstream;
			tempstream << file.rdbuf();
			string temp = tempstream.str();
			return temp;
		}
	}

	MeshInfo meshInfo;
	TextureInfo texInfo;
	const char* vs;
	const char* fs;
	unsigned int VAO, EBO, VBO, vertexShader, fragmentShader, program;

	// Shared variables.
	Vector4D position, direction, scale;

	// AxisAlignedBoundingBox variables.
	void makeAABB()
	{
		for (int i = 0; i < meshInfo.combinedBuffer.size(); i++)
		{
			float* t = meshInfo.combinedBuffer[i].pos;
			Vector4D temp_vec = Matrix4D::getPositionMatrix(position) * Matrix4D::getRotationMatrix(direction) * Matrix4D::getScaleMatrix(scale) * Vector4D(t[0], t[1], t[2]);

			if (temp_vec[0] > maxPoint[0])
				maxPoint[0] = temp_vec[0];

			else if (temp_vec[0] < minPoint[0])
				minPoint[0] = temp_vec[0];

			if (temp_vec[1] > maxPoint[1])
				maxPoint[1] = temp_vec[1];

			else if (temp_vec[1] < minPoint[1])
				minPoint[1] = temp_vec[1];

			if (temp_vec[2] > maxPoint[2])
				maxPoint[2] = temp_vec[2];

			else if (temp_vec[2] < minPoint[2])
				minPoint[2] = temp_vec[2];
		}

		AABB_dimention = Vector4D(maxPoint[0] - minPoint[0], maxPoint[1] - minPoint[1], maxPoint[2] - minPoint[2], 1);
		AABB_position = Vector4D((minPoint[0] + (AABB_dimention[0] / 2)), (minPoint[1] + (AABB_dimention[1] / 2)), (minPoint[2] + (AABB_dimention[2] / 2)));


		AABB_squares.push_back(Square(Vector4D(AABB_position[0], AABB_position[1] + AABB_dimention[1]/2, AABB_position[2]), Vector2D(AABB_dimention[0], AABB_dimention[2]), Vector4D(0, 1, 0), true)); // top
		AABB_squares.push_back(Square(Vector4D(AABB_position[0], AABB_position[1] - AABB_dimention[1]/2, AABB_position[2]), Vector2D(AABB_dimention[0], AABB_dimention[2]), Vector4D(0,-1, 0), true)); // bottom
		AABB_squares.push_back(Square(Vector4D(AABB_position[0], AABB_position[1], AABB_position[2] + AABB_dimention[2]/2), Vector2D(AABB_dimention[0], AABB_dimention[1]), Vector4D(0, 0, 1), true)); // front
		AABB_squares.push_back(Square(Vector4D(AABB_position[0], AABB_position[1], AABB_position[2] - AABB_dimention[2]/2), Vector2D(AABB_dimention[0], AABB_dimention[1]), Vector4D(0, 0,-1), true)); // back
		AABB_squares.push_back(Square(Vector4D(AABB_position[0] - AABB_dimention[0]/2, AABB_position[1], AABB_position[2]), Vector2D(AABB_dimention[2], AABB_dimention[1]), Vector4D(-1, 0,0), true)); // left
		AABB_squares.push_back(Square(Vector4D(AABB_position[0] + AABB_dimention[0]/2, AABB_position[1], AABB_position[2]), Vector2D(AABB_dimention[2], AABB_dimention[1]), Vector4D(1, 0, 0), true)); // right
	}
	void updateAABB()
	{
		clearAABB();
		makeAABB();
	}
	void clearAABB()
	{
		for (int i = 0; i < AABB_squares.size(); i++)
		{
			int index = AABB_squares[i].getDebugIndex();
			DebugManager::getInstance().removeAABB(index);
		}
		maxPoint = Vector4D(-99999, -99999, -99999);
		minPoint = Vector4D(99999, 99999, 99999);
		AABB_squares.clear();
	}

	Vector4D maxPoint = Vector4D(-99999, -99999, -99999);
	Vector4D minPoint = Vector4D(99999, 99999, 99999);

	Vector4D AABB_dimention, AABB_position;
	vector<Square> AABB_squares; //top, bottom, front, back, left, right
};