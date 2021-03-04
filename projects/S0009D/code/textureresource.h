#pragma once
#include <core/app.h>
#include "stb_image.h"

struct Pixel {
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;

	Pixel(unsigned char r, unsigned char g, unsigned char b) {
		red = r;
		green = g;
		blue = b;
	}
	Pixel() {

	}
};

class TextureResource
{
public:
	TextureResource()
	{

	}
	~TextureResource()
	{
		glDeleteTextures(1, &texture);
	}

	unsigned int loadFromFile(const char* filename) 
	{
		int width, heigth, numComponents;
		//stbi_set_flip_vertically_on_load(1);
		unsigned char* imageData = stbi_load(filename, &width, &heigth, &numComponents, 4);

		if (imageData == NULL)
		{
			std::cerr << "Texture loading failed at: " << filename << std::endl;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(imageData);

		return texture;
	}
	void loadBuffer()
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void loadFromArray(Pixel* buffer, int width, int height)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	float* getColor(float x, float y)
	{
		x = abs(x);
		y = abs(y);
		if (x > 1)
			x = 1;
		else if (x < 0)
			x = 0;
		if (y > 1)
			y = 1;
		else if (y < 0)
			y = 0;
		int xCord = (width * x);
		int yCord = ((heigth - 1) * y);

		int uvCoordinate = (xCord * numChannels) + (yCord * width * numChannels);
		float* rgb = new float[3];

		rgb[0] = imageData[uvCoordinate++];
		rgb[1] = imageData[uvCoordinate++];
		rgb[2] = imageData[uvCoordinate++];
		
		return rgb;
	}
	int getWidth()
	{
		return width;
	}
	int getHeigth()
	{
		return heigth;
	}

	unsigned int bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture);
		return unit;
	}

private:
	uint32 texture;
	unsigned char* imageData;
	int width;
	int heigth;
	int numChannels;
};