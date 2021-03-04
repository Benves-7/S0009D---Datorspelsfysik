#pragma once
#include "meshresource.h"
#include "textureresource.h"
#include "Matrix4D.h"

struct Line
{
	Line(int* pixels, int size)
	{
		this->pixels = pixels;
		this->size = size;
	}
	int* pixels;
	int size;
};

class Renderer
{
public:

	std::vector<Vertex> points;
	std::vector<int> indices;

	Renderer(const int& xSize, const int& ySize)
	{
		frameBufferSize = xSize * ySize;
		width = xSize;
		height = ySize;
		frameBuffer = new Pixel[frameBufferSize];
		zBuffer = new float[frameBufferSize];

		fill_n(zBuffer, frameBufferSize, -20000);
	}
	Renderer()
	{

	}
	~Renderer()
	{

	}

	void setTransform(Matrix4D mat)
	{
		transform = mat;
	}
	void setLookat(Matrix4D mat)
	{
		lookAt = mat;
	}
	void setCameraPosition(const Vector4D& vec)
	{
		cameraPosition = vec;
	}
	void loadTexture(char* filename)
	{
		texture.loadFromFile(filename);
	}
	Pixel* getFrameBuffer()
	{
		return frameBuffer;
	}
	int getFrameBufferSize()
	{
		return frameBufferSize;
	}
	void setVertexShader(std::function<Vertex(Vertex vertex, Matrix4D lookat, Matrix4D modelMatrix)> shader)
	{
		this->vertexShader = shader;
	}
	void setFragmentShader(std::function<Vector4D(Vertex vertex, Vector4D cameraPosition, TextureResource tex)> shader)
	{
		this->fragmentShader = shader;
	}
	/// NOT DONE.
	void setBuffers()
	{
		MeshResource mesh;
		mesh.loadOBJ("tractor.obj");
		texture.loadFromFile("tractor.png");
		points = mesh.getVertexBuffer();
		indices = mesh.getIndexBuffer();
	}
	void clearZbuffer()
	{
		fill_n(zBuffer, frameBufferSize, -20000);
		Pixel p;
		fill_n(frameBuffer, frameBufferSize, p);
	}
	void rastTriangle(Vertex v1, Vertex v2, Vertex v3)
	{
		w1 = 1 / v1.pos[3];
		w2 = 1 / v2.pos[3];
		w3 = 1 / v3.pos[3];
		v1 = vertexShader(v1, transform, lookAt);
		v2 = vertexShader(v2, transform, lookAt);
		v3 = vertexShader(v3, transform, lookAt);

		if (v1.pos[1] < v2.pos[1]) // switches "1" and "2"
		{
			Vertex temp = v1;
			v1 = v2;
			v2 = temp;
		}
		if (v2.pos[1] < v3.pos[1]) // switches "1" and "3"
		{
			Vertex temp = v2;
			v2 = v3;
			v3 = temp;
		}
		if (v1.pos[1] < v2.pos[1]) // switches "2" and "3"
		{
			Vertex temp = v1;
			v1 = v2;
			v2 = temp;
		}
	
		float coordinateSpace[6];
		coordinateSpace[0] = v1.pos[0];
		coordinateSpace[1] = v1.pos[1];
		coordinateSpace[2] = v2.pos[0];
		coordinateSpace[3] = v2.pos[1];
		coordinateSpace[4] = v3.pos[0];
		coordinateSpace[5] = v3.pos[1];

		v1.pos[0] = std::roundf(v1.pos[0] * width + 1 / 2 + width / 2);
		v1.pos[1] = std::roundf(-v1.pos[1] * height + 1 / 2 + height / 2);

		v2.pos[0] = std::roundf(v2.pos[0] * width + 1 / 2 + width / 2);
		v2.pos[1] = std::roundf(-v2.pos[1] * height + 1 / 2 + height / 2);

		v3.pos[0] = std::roundf(v3.pos[0] * width + 1 / 2 + width / 2);
		v3.pos[1] = std::roundf(-v3.pos[1] * height + 1 / 2 + height / 2);

		Line edge1 = createLine2(v1, v3);
		Line edge2 = createLine2(v1, v2);
		Line edge3 = createLine2(v2, v3);
		int vert1Y = v1.pos[1];
		int vert2Y = v2.pos[1];

		Line lineArray[] = { edge2, edge3 };
		int u = 0;
		bool isSwapped = false;
		int z = 0;
		for (int i = 0; i < edge1.size; i++)
		{
			if (!isSwapped && u >= edge2.size) 
			{
				u = 1;
				z = 1;
				isSwapped = true;
				vert2Y = v3.pos[1];
			}
			linescan(edge1.pixels[i], lineArray[z].pixels[u], vert1Y + i, v1, v2, v3, coordinateSpace);
			u++;
		}
		/// Delete all the memory that was allocated with new
		delete[] edge1.pixels;
		delete[] edge2.pixels;
		delete[] edge3.pixels;
	}
	Line createLine2(Vertex v1, Vertex v2)
	{
		int vert1X = v1.pos[0];
		int vert1Y = v1.pos[1];
		int vert2X = v2.pos[0];
		int vert2Y = v2.pos[1];

		int dx, dy, F;

		dx = abs(vert1X - vert2X);
		dy = abs(vert1Y - vert2Y);

		Line line(new int[dy + 1], dy + 1);

		int dir = vert1X < vert2X ? 1 : -1;
		int y = 0;
		int h2 = 2 * dy;
		int wh2 = -2 * (dx - dy);

		if (dx >= dy)
		{
			F = 2 * dy - dx;
			while (vert1X != vert2X)
			{
				line.pixels[y] = vert1X;
				if (F < 0)
					F += h2;
				else
				{
					y += 1;
					F += wh2;
				}
				vert1X += dir;
			}
		}
		else
		{
			h2 = 2 * dx;
			wh2 = -2 * (dy - dx);
			F = 2 * dx - dy;
			while (y < dy)
			{
				line.pixels[y] = vert1X;
				if (F < 0)
					F += h2;
				else
				{
					vert1X += dir;
					F += wh2;
				}
				y += 1;
			}
		}
		line.pixels[y] = vert1X;
		return line;
	}
	void putPixel(int index, Vector4D color)
	{
		frameBuffer[index].red = color[0];
		frameBuffer[index].green = color[1];
		frameBuffer[index].blue = color[2];
	}
	void linescan(int x1, int x2, int y, const Vertex& v1, const Vertex& v2, const Vertex& v3, float* coordinateSpace)
	{
		// Switch if needed. 
		if (x1 > x2)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
		}
		if (x1 < 0)
			x1 = 0;
		if (x2 > width)
			x2 = width;
		if (y < 0 || y > height)
			return;

		int temp = y * width + x1 + 1;
		
		for (int x = x1; x <= x2; x++)
		{
			float u, v, w;
			barycentric(Vector4D(x, y, 0, 1), Vector4D(v1.pos[0], v1.pos[1], 1.0f, 1), Vector4D(v2.pos[0], v2.pos[1], 1.0f, 1), Vector4D(v3.pos[0], v3.pos[1], 1.0f, 1), u, v, w);

			float zIndex = (v1.pos[2] * u) + (v2.pos[2] * v) + (v3.pos[2] * w);

			Vertex temp2;
			temp2.pos[0] = (u * coordinateSpace[0] + v * coordinateSpace[2] + w * coordinateSpace[4]);
			temp2.pos[1] = (u * coordinateSpace[1] + v * coordinateSpace[3] + w * coordinateSpace[5]);

			temp2.uv[0] = ((u * v1.uv[0]) + (v * v2.uv[0]) + (w * v3.uv[0]));
			temp2.uv[1] = ((u * v1.uv[1]) + (v * v2.uv[1]) + (w * v3.uv[1]));

			temp2.normal[0] = ((u * v1.normal[0]) + (v * v2.normal[0]) + (w * v3.normal[0]));
			temp2.normal[1] = ((u * v1.normal[1]) + (v * v2.normal[1]) + (w * v3.normal[1]));
			temp2.normal[2] = ((u * v1.normal[2]) + (v * v2.normal[2]) + (w * v3.normal[2]));

			if (zBuffer[temp] < zIndex)
			{
				Vector4D color = fragmentShader(temp2, cameraPosition, texture);
				zBuffer[temp] = zIndex;
				putPixel(temp, color);
			}

			temp++;
		}

	}
	/// UNUSED.
	Vector4D getBary(int x, int y, Vertex v1, Vertex v2, Vertex v3)
	{
		float area = areaOfTriangle(v1, v2, v3);
		Vertex temp;
		temp.pos[0] = x;
		temp.pos[1] = y;
		Vector4D triangles;
		triangles[0] = areaOfTriangle(temp, v2, v3) / area;
		triangles[1] = areaOfTriangle(temp, v1, v3) / area;
		triangles[2] = areaOfTriangle(temp, v2, v1) / area;
		return triangles;
	}
	void barycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3)
	{
		Vector4D temp1 = vec2 - vec1, temp2 = vec3 - vec1, temp3 = point - vec1;
		float daa = Vector4D::dot(temp1, temp1);
		float dab = Vector4D::dot(temp1, temp2);
		float dbb = Vector4D::dot(temp2, temp2);
		float dca = Vector4D::dot(temp3, temp1);
		float dcb = Vector4D::dot(temp3, temp2);
		float denominator = (daa * dbb) - (dab * dab);
		p2 = (dbb * dca - dab * dcb) / denominator;
		p3 = (daa * dcb - dab * dca) / denominator;
		p1 = 1.0f - p2 - p3;
	}
	float areaOfTriangle(Vertex v1, Vertex v2, Vertex v3)
	{
		return abs(((v1.pos[0] * (v2.pos[1] - v3.pos[1])) + v2.pos[0] * (v3.pos[1]) + v3.pos[0] * (v1.pos[1] - v2.pos[1])) / 2);
	}

private:

	Pixel* frameBuffer;
	float* zBuffer;
	int frameBufferSize = 0;
	int width;
	int height;
	std::function<Vertex(Vertex vertex, Matrix4D lookat, Matrix4D modelMatrix)> vertexShader;
	std::function<Vector4D(Vertex vertex, Vector4D cameraPosition, TextureResource tex)> fragmentShader;
	Matrix4D transform;
	Matrix4D lookAt;
	Vector4D cameraPosition;
	Pixel* drawTexture;
	int drawTextureWidth;
	int drawTextureHeigth;
	TextureResource texture;
	float w1, w2, w3;
};
