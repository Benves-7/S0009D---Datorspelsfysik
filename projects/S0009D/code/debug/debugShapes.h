#pragma once
#include "../math.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "core/app.h"


using namespace std;

class DebugBase
{
public:
	virtual void draw(Matrix4D viewMatrix)
	{
		glUseProgram(program);

		unsigned int location = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(location, 1, GL_TRUE, viewMatrix.getPointer());

		location = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(location, 1, GL_TRUE, Matrix4D().getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
	}

protected:

	virtual void setup()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glBindVertexArray(0);

		// Vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		string temp = setupVertexShader();
		vs = temp.c_str();
		GLint len = static_cast<GLint>(strlen(vs));
		glShaderSource(vertexShader, 1, &vs, &len);
		glCompileShader(vertexShader);

		// Error log
		GLint shaderLogSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// Fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		temp = setupFragmentShader();
		fs = temp.c_str();
		len = static_cast<GLint>(strlen(fs));
		glShaderSource(fragmentShader, 1, &fs, &len);
		glCompileShader(fragmentShader);

		// Error log
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
	}

	string setupFragmentShader()
	{
		ifstream file("dfs.shader");
		if (file.fail())
		{
			if (!errorcode)
			{
				cout << "Error: failed to load fragmentshader" << endl;
				errorcode = true;
				return 0;
			}
		}
		else
		{
			stringstream tempstream;
			tempstream << file.rdbuf();
			string temp = tempstream.str();
			return temp;
		}
	}
	string setupVertexShader()
	{
		ifstream file("dvs.shader");
		if (file.fail())
		{
			if (!errorcode)
			{
				cout << "Error: failed to load vertexshader" << endl;
				errorcode = true;
				return 0;
			}
		}
		else
		{
			stringstream tempstream;
			tempstream << file.rdbuf();
			string temp = tempstream.str();
			return temp;
		}
	}

	bool errorcode = false;
	vector<float>vertices;
	vector<int>indices;
	unsigned int VAO, EBO, VBO, vertexShader, fragmentShader, program;
	const char* vs;
	const char* fs;
	Vector4D color;
};

class DebugSquare : public DebugBase
{
public:
	DebugSquare(Vector4D pos, Vector2D dim, Vector4D col = Vector4D(1, 1, 1, 1))
	{
		position = pos;
		dimentions = dim;
		color = col;

		float w = dim[0] / 2;
		float h = dim[1] / 2;

		vertices.push_back(pos[0] + w); vertices.push_back(pos[1] + h); vertices.push_back(pos[2]);
		vertices.push_back(pos[0] + w); vertices.push_back(pos[1] - h); vertices.push_back(pos[2]);
		vertices.push_back(pos[0] - w); vertices.push_back(pos[1] - h); vertices.push_back(pos[2]);
		vertices.push_back(pos[0] - w); vertices.push_back(pos[1] + h); vertices.push_back(pos[2]);

		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(3); indices.push_back(2);

		setup();
	}
	~DebugSquare()
	{

	}

	void draw(Matrix4D viewMatrix)
	{
		glUseProgram(program);

		unsigned int location = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(location, 1, GL_TRUE, viewMatrix.getPointer());

		location = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(location, 1, GL_TRUE, Matrix4D().getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:

	Vector4D position;
	Vector2D dimentions;
};

class DebugLine : public DebugBase
{
public:
	DebugLine(Vector4D p1, Vector4D p2, Vector4D c = Vector4D(255, 0, 0, 1))
	{
		point1 = p1;
		point2 = p2;
		color = c;

		vertices.push_back(p1[0]);
		vertices.push_back(p1[1]);
		vertices.push_back(p1[2]);
		vertices.push_back(p2[0]);
		vertices.push_back(p2[1]);
		vertices.push_back(p2[2]);

		indices.push_back(0);
		indices.push_back(1);

		setup();
	}
	~DebugLine()
	{

	}

	void draw(Matrix4D viewMatrix)
	{
		glUseProgram(program);

		unsigned int location = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(location, 1, GL_TRUE, viewMatrix.getPointer());

		location = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(location, 1, GL_TRUE, Matrix4D().getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:

	Vector4D point1, point2;
};

class DebugCube : public DebugBase
{
public:
	DebugCube(Vector4D pos, Vector4D dim, Vector4D col = Vector4D(10, 30, 30, 1))
	{
		position = pos;
		dimentions = dim;
		color = col;

		float w = dim[0] / 2;
		float h = dim[1] / 2;
		float d = dim[2] / 2;

		vertices.push_back(pos[0] + w); vertices.push_back(pos[1] + h); vertices.push_back(pos[2] + d);
		vertices.push_back(pos[0] + w); vertices.push_back(pos[1] + h); vertices.push_back(pos[2] - d);
		vertices.push_back(pos[0] - w); vertices.push_back(pos[1] + h); vertices.push_back(pos[2] - d);
		vertices.push_back(pos[0] - w); vertices.push_back(pos[1] + h); vertices.push_back(pos[2] + d);

		vertices.push_back(pos[0] + w); vertices.push_back(pos[1] - h); vertices.push_back(pos[2] + d);
		vertices.push_back(pos[0] + w); vertices.push_back(pos[1] - h); vertices.push_back(pos[2] - d);
		vertices.push_back(pos[0] - w); vertices.push_back(pos[1] - h); vertices.push_back(pos[2] - d);
		vertices.push_back(pos[0] - w); vertices.push_back(pos[1] - h); vertices.push_back(pos[2] + d);

		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);

		indices.push_back(0); indices.push_back(4); indices.push_back(5);
		indices.push_back(0); indices.push_back(1); indices.push_back(5);

		indices.push_back(1); indices.push_back(5); indices.push_back(6);
		indices.push_back(1); indices.push_back(2); indices.push_back(6);

		indices.push_back(2); indices.push_back(3); indices.push_back(6);
		indices.push_back(6); indices.push_back(7); indices.push_back(3);

		indices.push_back(0); indices.push_back(3); indices.push_back(7);
		indices.push_back(0); indices.push_back(4); indices.push_back(7);
		
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);

		setup();
	}
	~DebugCube()
	{

	}

	void draw(Matrix4D viewMatrix)
	{
		glUseProgram(program);

		unsigned int location = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(location, 1, GL_TRUE, viewMatrix.getPointer());

		location = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(location, 1, GL_TRUE, Matrix4D().getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:

	Vector4D position, dimentions;
};

class DebugSphere : public DebugBase
{
public:
	DebugSphere(Vector4D pos, float rad, Vector4D col = Vector4D(100, 40, 150))
	{
		position = pos;
		radius = rad;
		color = col;

		int rings = 30;
		int sectors = 200;

		float const R = 1.0 / (float)(rings);
		float const S = 1.0 / (float)(sectors);
		int r, s;

		vertices.resize(rings * sectors * 3);
		indices.resize(rings * sectors * 4);
		vector<GLfloat>::iterator v = vertices.begin();
		vector<GLint>::iterator i = indices.begin();
		for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++)
		{
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}

		setup();
	}
	~DebugSphere()
	{

	}

	void draw(Matrix4D viewMatrix)
	{
		glUseProgram(program);

		unsigned int location = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(location, 1, GL_TRUE, viewMatrix.getPointer());

		location = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(location, 1, GL_TRUE, Matrix4D::getPositionMatrix(position).getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:

	Vector4D position;
	float radius;
};