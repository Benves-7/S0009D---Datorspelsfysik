#pragma once
#include "math.h"
#include "debug/debugShapes.h"

class Plane
{
public:
	Plane()
	{

	}
	Plane(Vector4D pos, Vector2D dim, Vector4D dir, Vector4D col = Vector4D(1, 1, 1))
	{
		float w = dim[0] / 2;
		float h = dim[1] / 2;
		position = pos;
		color = col;
		dimentions = dim;
		normal = dir;

		p1 = Vector4D(pos[0] + w, pos[1] + h, pos[2]);
		p2 = Vector4D(pos[0] + w, pos[1] - h, pos[2]);
		p3 = Vector4D(pos[0] - w, pos[1] - h, pos[2]);
		p4 = Vector4D(pos[0] - w, pos[1] + h, pos[2]);

		debugIndex = DebugManager::getInstance().addSafeShape(new DebugSquare(pos, dim, dir, col));
	}
	~Plane()
	{

	}

	void setPosition(Vector4D pos)
	{
		position = pos;
		DebugManager::getInstance().getSavedShape(debugIndex)->setPosition(pos);
	}
	Vector4D& getPosition()
	{
		return position;
	}

	void setNormal(Vector4D dir)
	{
		normal = dir;
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
		return &color;
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
