#pragma once
#include "math.h"

class Plane
{
public:
	Plane(Vector4D pos, Vector4D dir)
	{
		position = pos;
		normal = dir;
	}
	~Plane()
	{

	}

	void setPosition(const Vector4D pos)
	{
		position = pos;
	}
	Vector4D& getPosition()
	{
		return position;
	}

	void setNormal(const Vector4D dir)
	{
		normal = dir;
	}
	Vector4D& getNormal()
	{
		return normal;
	}

private:
	Vector4D position, normal;
};