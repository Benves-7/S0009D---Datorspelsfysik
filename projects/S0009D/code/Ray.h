#pragma once
#include "debug/debugShapes.h"
#include "Plane.h"
#include <limits>

/// TODO: make seperate .cpp for ray and plane so to stop the circular include dependency..

struct IntersectData
{
	Vector4D point;
	float distance;
	IntersectData(Vector4D point, float distance) : point(point), distance(distance) {};
};
struct ReturnInfo
{
	int index;
};

class Ray
{
public:
	Ray()
	{

	}
	Ray(Vector4D pos, Vector4D dir)
	{
		origin = pos;
		direction = dir;
		DebugManager::getInstance().addShape(new DebugLine(pos, (pos + dir * 100)));
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

	int checkHit(vector<Plane> planes)
	{
		float closest = std::numeric_limits<float>::infinity();
		int best = -1;
		for (int i = 0; i < planes.size(); i++)
		{
			float dist = checkIfHitting(planes.at(i));
			if (dist < closest && dist != -1)
			{
				closest = dist;
				best = i;
			}
		}
		if (best != -1)
		{
			return best;
		}
	}

	IntersectData intersect(Plane plane)
	{
		Vector4D diff = origin - plane.getPosition();

		float prod1 = diff.dot(plane.getNormal());
		float prod2 = direction.dot(plane.getNormal());
		float prod3 = prod1 / prod2;

		// with the distance a point on the plane can be calculated.
		return IntersectData((origin - direction * prod3), -prod3);
	}
	float checkIfHitting(Plane plane)
	{
		IntersectData result = intersect(plane);
		//DebugManager::getInstance().addSafeShape(new DebugSphere(result.point, 0.05));
		if (result.distance == -1)
			return -1;

		Vector4D a = result.point;
		Vector4D b = plane.P1();
		Vector4D c = plane.P2();
		Vector4D d = plane.P3();
		Vector4D e = plane.P4();

		if ((Vector4D::dot(b, c - b) <= Vector4D::dot(a, c - b) && Vector4D::dot(a, c - b) <= Vector4D::dot(c, c - b)) &&
			(Vector4D::dot(b, e - b) <= Vector4D::dot(a, e - b) && Vector4D::dot(a, e - b) <= Vector4D::dot(e, e - b)))
		{
			intersections.push_back(result);
			DebugManager::getInstance().addShape(new DebugSphere(result.point, 0.05f));
			return result.distance;
		}
		else
		{
			return -1;
		}
	}

private:
	Vector4D origin, direction;
	vector<IntersectData> intersections;
};