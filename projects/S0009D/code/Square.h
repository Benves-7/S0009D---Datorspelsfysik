#pragma once
#include "math.h"
#include "debug/debugManager.h"

class Square
{
public:
	Square()
	{

	}
	Square(Vector4D pos, Vector2D dim, Vector4D dir, Vector4D col = Vector4D(1, 1, 1), bool wireframe = false)
	{
		float w = dim[0] / 2;
		float h = dim[1] / 2;
		position = pos;
		color = col;
		dimentions = dim;
		normal = Vector4D::normalize(dir);

		p1 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(w, h);
		p2 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(w, -h);
		p3 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(-w, -h);
		p4 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(-w, h);

		debugIndex = DebugManager::getInstance().createSquare(pos, dim, dir, wireframe, col);
	}
	Square(Vector4D pos, Vector2D dim, Vector4D dir, bool AABB)
	{
		float w = dim[0] / 2;
		float h = dim[1] / 2;
		position = pos;
		color = Vector4D(0,0,1);
		dimentions = dim;
		normal = Vector4D::normalize(dir);

		p1 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(w, h);
		p2 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(w, -h);
		p3 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(-w, -h);
		p4 = Matrix4D::getPositionMatrix(pos) * Matrix4D::getRotationMatrix(dir) * Vector4D(-w, h);

		debugIndex = DebugManager::getInstance().createAABBSquare(pos, dim, dir);
	}

	~Square()
	{

	}

	void setPosition(Vector4D pos)
	{
		position = pos;
		float w = dimentions[0] / 2;
		float h = dimentions[1] / 2;
		p1 = Matrix4D::getRotationMatrix(normal) * Vector4D(pos[0] + w, pos[1] + h, pos[2]);
		p2 = Matrix4D::getRotationMatrix(normal) * Vector4D(pos[0] + w, pos[1] - h, pos[2]);
		p3 = Matrix4D::getRotationMatrix(normal) * Vector4D(pos[0] - w, pos[1] - h, pos[2]);
		p4 = Matrix4D::getRotationMatrix(normal) * Vector4D(pos[0] - w, pos[1] + h, pos[2]);
		DebugManager::getInstance().getSavedShape(debugIndex)->setPosition(pos);
	}
	Vector4D& getPosition()
	{
		return position;
	}

	void setNormal(Vector4D dir)
	{
		normal = Vector4D::normalize(dir);
		DebugManager::getInstance().getSavedShape(debugIndex)->setNormal(normal);
	}
	Vector4D& getNormal()
	{
		return normal;
	}

	void setColor(Vector4D col)
	{
		color = col;
		DebugManager::getInstance().getSavedShape(debugIndex)->setColor(col);
	}
	Vector4D* getColor()
	{
		return DebugManager::getInstance().getSavedShape(debugIndex)->getColor();
	}

	int getDebugIndex()
	{
		return debugIndex;
	}

	Vector4D getPoint(Vector4D point)
	{
		return Vector4D();
	}

	Vector4D P1()
	{
		return p1;
	}
	Vector4D P2()
	{
		return p2;
	}
	Vector4D P3()
	{
		return p3;
	}
	Vector4D P4()
	{
		return p4;
	}

private:
	
	Vector4D position, color, normal, p1, p2, p3, p4;
	Vector2D dimentions;
	int debugIndex;
};
