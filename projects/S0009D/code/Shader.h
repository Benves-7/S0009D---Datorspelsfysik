#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	Shader() {}
	~Shader() {}

	void setup()
	{
		loadShaders("vs.shader", "fs.shader");
		linkShaders();
	}
	void createProgram() 
	{ 
		this->program = glCreateProgram(); 
	}
	void loadShaders(const char* vs, const char* fs)
	{
		loadVertexShader(vs);
		loadFragmentShader(fs);
	}
	void loadVertexShader(const char* filename)
	{
		std::ifstream inFile;
		inFile.open(filename);

		if (inFile.fail())
		{
			printf("File %s failed to load.\n", filename);
			system("pause");
			exit(1);
		}
		else {
			std::stringstream buffer;
			buffer << inFile.rdbuf();
			std::string shader = buffer.str();

			vertexShader = shader.c_str();
			setupVertexShader();
		}
	}
	void loadFragmentShader(const char* filename) 
	{
		std::ifstream inFile;
		inFile.open(filename);

		if (inFile.fail())
		{
			printf("File %s failed to load.\n", filename);
			system("pause");
			exit(1);
		}
		else {
			std::stringstream buffer;
			buffer << inFile.rdbuf();

			std::string shader = buffer.str();
			fragmentShader = shader.c_str();
			setupFragmentShader();
		}
	}
	void setupVertexShader()
	{
		vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		int length = strlen(this->vertexShader);
		glShaderSource(vertexShaderHandle, 1, &this->vertexShader, &length);
		glCompileShader(vertexShaderHandle);
		
		// Error log.
		int shaderLogSize;
		glGetShaderiv(this->vertexShaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShaderHandle, shaderLogSize, NULL, buf);
			printf(buf);
			delete[] buf;
		}
	}
	void setupFragmentShader()
	{
		fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		int length = strlen(this->fragmentShader);
		glShaderSource(fragmentShaderHandle, 1, &this->fragmentShader, &length);
		glCompileShader(fragmentShaderHandle);

		// Error log.
		int shaderLogSize;
		glGetShaderiv(this->fragmentShaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->fragmentShaderHandle, shaderLogSize, NULL, buf);
			printf(buf);
			delete[] buf;
		}
	}
	void linkShaders()
	{
		this->program = glCreateProgram();
		glAttachShader(this->program, vertexShaderHandle);
		glAttachShader(this->program, fragmentShaderHandle);
		glLinkProgram(this->program);

		// Error log.
		int shaderLogSize;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf(buf);
			delete[] buf;
		}
	}
	void modifyUniformMatrix(std::string name, float* mat)
	{
		unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
		glUniformMatrix4fv(uniform, 1, GL_TRUE, mat);
	}
	void modifyUniformVector(std::string name, Vector4D vec)
	{
		unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
		glUniform4fv(uniform, 1, vec.getPointer());
	}
	void modifyUniformFloat(std::string name, float value)
	{
		unsigned int uniform = glGetUniformLocation(this->program, name.c_str());
		glUniform1f(uniform, value);
	}
	void useProgram()
	{
		glUseProgram(this->program);
	}

private:

	const char* vertexShader;
	const char* fragmentShader;
	unsigned int fragmentShaderHandle;
	unsigned int vertexShaderHandle;
	unsigned int program;

};