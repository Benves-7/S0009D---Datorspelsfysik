#pragma once
#include "math.h"


class Ray
{
public:
	Ray() { }
	Ray(Vector4D pos, Vector4D dir)
	{
		origin = pos;
		direction = dir;
	}
	~Ray() { }

	Vector4D getOrigin()
	{
		return origin;
	}
	Vector4D getPointOnRay(int i = 0)
	{
		return origin + (direction * i);
	}

private:
	Vector4D origin, direction;
};