#pragma once
#include "light.h"
#include "meshresource.h"
#include "textureresource.h"
#include "Shader.h"
#include "Camera.h"
#include <memory>

class GraphicsNode
{
public:
	GraphicsNode() {}
	~GraphicsNode() {}

	void setMeshClass(MeshResource _mesh)
	{
		mesh = _mesh;
	}
	MeshResource* getMeshClass()
	{
		return &mesh;
	}
	void setTextureResource(TextureResource _texture)
	{
		texture = _texture;
	}
	TextureResource* getTextureClass()
	{
		return &texture;
	}
	void setShaderObject(Shader _shader)
	{
		shader = _shader;
	}
	Shader* getShaderClass()
	{
		return &shader;
	}
	void setProjectionMatrix(Matrix4D proj)
	{
		projection = proj;
	}
	void setViewMatrix(Matrix4D* v)
	{
		view = v;
	}
	void setCamerapos(Vector4D cam)
	{
		camera = cam;
	}

	void setup(void* ib, void* vb, int ibSize, int vbSize, int numVerticies, int numIndicies)
	{
		mesh.setup(ib,vb,ibSize,vbSize, numVerticies, numIndicies);
		shader.setup();
		//texId = texture.bind(texture.loadFromFile("Footman_Diffuse.tga"));
	}
	void draw()
	{
		shader.useProgram();
		//shader.modifyUniformMatrix("view", view->getPointer());
		//shader.modifyUniformMatrix("projection", projection.getPointer());
		//shader.modifyUniformVector("cameraPosition", camera);
		//shader.modifyUniformMatrix("transform", transform.getPointer());
		int i = mesh.getIndexBufferSize();

		glDrawElements(GL_TRIANGLES, i, GL_UNSIGNED_INT, 0);
	}


private:

	Shader shader;
	MeshResource mesh;
	TextureResource texture;
	unsigned int texId;

	Matrix4D projection;
	Matrix4D* view;
	Vector4D camera;

	Matrix4D transform;

};

//class GraphicsNode
//{
//public:
//
//	GraphicsNode() { transform = Matrix4D::rotY(1); }
//	~GraphicsNode()
//	{
//		this->shader = 0;
//		this->meshResource = 0;
//		this->textureResource = 0;
//	}
//
//	void setShaderObject(std::shared_ptr<Shader> shader) { this->shader = shader; }
//	void setTextureResource(std::shared_ptr<TextureResource> textureResource) { this->textureResource = textureResource; }
//	void setMeshResource(std::shared_ptr<MeshResource> meshResource) { this->meshResource = meshResource; }
//	Shader* getShaderObject() { return shader.get(); }
//	TextureResource* getTextureResource() { return textureResource.get(); }
//	MeshResource* getMeshResource() { return meshResource.get(); }
//	Matrix4D getTransform() { return transform; }
//	void setTransform(Matrix4D mat) { transform = mat; }
//	unsigned int load(string filename, string vertexShaderName, string fragmentShaderName, int texture)
//	{
//		unsigned int texI;
//		if (texture >= 0)
//		{
//			texI = textureResource.get()->loadFromFile(filename.c_str());
//		}
//		shader.get()->loadVertexShader(vertexShaderName.c_str());
//		shader.get()->loadFragmentShader(fragmentShaderName.c_str());
//		shader.get()->linkShaders();
//		if (texture >= 0)
//		{
//			textureResource.get()->bind(texture);
//		}
//		light = LightSource(Vector4D(5, 2, 0, 1), Vector4D(0.5f, 0.5f, 0.5f, 1), 1);
//		shader.get()->useProgram();
//		if (texture >= 0)
//		{
//			if (texI >= 0)
//				return texI;
//		}
//		return -1;
//	}
//	void draw()
//	{
//		rend.clearZbuffer();
//		shader.get()->modifyUniformMatrix("transform", transform.getPointer());
//
//		shader.get()->modifyUniformVector("lightPos", light.getPosition());
//		shader.get()->modifyUniformVector("lightColor", light.getColor());
//		shader.get()->modifyUniformFloat("intensity", light.getIntensity());
//
//		meshResource.get()->bind();
//		glDrawElements(GL_TRIANGLES, meshResource->getIndexSize(), GL_UNSIGNED_INT, 0);
//		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
//		meshResource.get()->unBindBuffers();
//	}
//
//private:
//
//	Renderer rend;
//	shared_ptr<Shader> shader;
//	shared_ptr<TextureResource> textureResource;
//	shared_ptr<MeshResource> meshResource;
//
//	Matrix4D transform;
//	float rotation = 0;
//	vector<Vertex> vertices;
//	vector<int> indices;
//	vector<Vector4D> uv;
//	vector<Vector4D> normals;
//	vector<Vector4D> combinedBuffer;
//
//	LightSource light;
//};