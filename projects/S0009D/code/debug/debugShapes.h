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
		if (!errorsent)
		{
			std::cout << "Draw function missing" << std::endl;
			errorsent = true;
		}
	}

	Vector4D* getColor()
	{
		return &color;
	}
	virtual void setColor(Vector4D col)
	{
		color = col;
	}
	virtual void setPosition(Vector4D pos)
	{
		position = pos;
	}
	virtual void setNormal(Vector4D nor)
	{
		direction = nor;
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
		string temp = setupDebugVertexShader();
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
		temp = setupDebugFragmentShader();
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

	string setupDebugFragmentShader()
	{
		ifstream file("dfs.shader");
		if (file.fail())
		{
			if (!errorsent)
			{
				cout << "Error: failed to load fragmentshader" << endl;
				errorsent = true;
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
	string setupDebugVertexShader()
	{
		ifstream file("dvs.shader");
		if (file.fail())
		{
			if (!errorsent)
			{
				cout << "Error: failed to load vertexshader" << endl;
				errorsent = true;
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

	bool errorsent = false;
	vector<float>vertices;
	vector<int>indices;
	unsigned int VAO, EBO, VBO, vertexShader, fragmentShader, program;
	const char* vs;
	const char* fs;
	Vector4D color, position, direction;
};

class DebugSquare : public DebugBase
{
public:
	DebugSquare(Vector4D pos, Vector2D dim, Vector4D dir, bool wf, Vector4D col = Vector4D(1, 1, 1, 1))
	{
		position = pos;
		direction = Vector4D::normalize(dir);
		dimentions = Vector4D(dim[0], dim[1], 1, 1);
		color = col;
		wireframe = wf;

		float w = dim[0] / 2;
		float h = dim[1] / 2;

		vertices.push_back(w);  vertices.push_back(h);  vertices.push_back(0);
		vertices.push_back(w);  vertices.push_back(-h); vertices.push_back(0);
		vertices.push_back(-w); vertices.push_back(-h); vertices.push_back(0);
		vertices.push_back(-w); vertices.push_back(h);  vertices.push_back(0);

		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(3); indices.push_back(2);

		setup();
	}
	DebugSquare(Vector4D pos, Vector2D dim, Vector4D dir)
	{
		position = pos;
		direction = Vector4D::normalize(dir);
		dimentions = Vector4D(dim[0], dim[1], 1, 1);
		color = Vector4D(0,0,1);
		wireframe = true;

		float w = dim[0] / 2;
		float h = dim[1] / 2;

		vertices.push_back(w);  vertices.push_back(h);  vertices.push_back(0);
		vertices.push_back(w);  vertices.push_back(-h); vertices.push_back(0);
		vertices.push_back(-w); vertices.push_back(-h); vertices.push_back(0);
		vertices.push_back(-w); vertices.push_back(h);  vertices.push_back(0);

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
		glUniformMatrix4fv(location, 1, GL_TRUE, (Matrix4D::getPositionMatrix(position) * Matrix4D::getRotationMatrix(direction)).getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);		
		if (wireframe)
		{
			glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, NULL);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:

	Vector4D dimentions;
	bool wireframe;
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

	void setColor(Vector4D col)
	{
		color = col;
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
	DebugCube(Vector4D pos, Vector4D dim, bool wf, Vector4D col = Vector4D(10, 30, 30, 1))
	{
		position = pos;
		dimentions = dim;
		color = col;
		wireframe = wf;

		float w = dim[0] / 2;
		float h = dim[1] / 2;
		float d = dim[2] / 2;

		vertices.push_back(w); vertices.push_back(h); vertices.push_back(d);
		vertices.push_back(w); vertices.push_back(h); vertices.push_back(-d);
		vertices.push_back(-w); vertices.push_back(h); vertices.push_back(-d);
		vertices.push_back(-w); vertices.push_back(h); vertices.push_back(d);

		vertices.push_back(w); vertices.push_back(-h); vertices.push_back(d);
		vertices.push_back(w); vertices.push_back(-h); vertices.push_back(-d);
		vertices.push_back(-w); vertices.push_back(-h); vertices.push_back(-d);
		vertices.push_back(-w); vertices.push_back(-h); vertices.push_back(d);

		indices.push_back(0); indices.push_back(1); indices.push_back(2); // TOP
		indices.push_back(0); indices.push_back(2); indices.push_back(3);

		indices.push_back(4); indices.push_back(5); indices.push_back(6); // BOT
		indices.push_back(4); indices.push_back(6); indices.push_back(7);

		indices.push_back(4); indices.push_back(0); indices.push_back(3); // FRO
		indices.push_back(4); indices.push_back(3); indices.push_back(7);

		indices.push_back(5); indices.push_back(1); indices.push_back(2); // BAC
		indices.push_back(5); indices.push_back(2); indices.push_back(6);

		indices.push_back(6); indices.push_back(2); indices.push_back(3); // LEF
		indices.push_back(6); indices.push_back(3); indices.push_back(7);

		indices.push_back(4); indices.push_back(0); indices.push_back(1); // RIG
		indices.push_back(4); indices.push_back(1); indices.push_back(5);

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
		glUniformMatrix4fv(location, 1, GL_TRUE, Matrix4D::getPositionMatrix(position).getPointer());

		location = glGetUniformLocation(program, "color");
		glUniform4fv(location, 1, color.GetPointer());

		glBindVertexArray(VAO);
		if (wireframe)
		{
			glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, NULL);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:

	bool wireframe;
	Vector4D dimentions;
};

class DebugSphere : public DebugBase
{
public:
	DebugSphere(Vector4D pos, float rad, Vector4D col = Vector4D(0, 1, 0))
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

	float radius;
};