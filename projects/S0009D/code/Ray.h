#pragma once
#include "debug/debugShapes.h"
#include "Plane.h"

/// TODO: make seperate .cpp for ray and plane so to stop the circular include dependency..

struct IntersectData
{
	Vector4D point;
	float distance;
	IntersectData(Vector4D point, float distance) : point(point), distance(distance) {};
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

	IntersectData intersect(Plane plane)
	{
		Vector4D diff = origin - plane.getPosition();

		float prod1 = diff.dot(plane.getNormal());
		float prod2 = direction.dot(plane.getNormal());
		float prod3 = prod1 / prod2;

		// with the distance a point on the plane can be calculated.
		return IntersectData((origin - direction * prod3), prod2);
	}
	bool checkIfHitting(Plane plane)
	{
		IntersectData result = intersect(plane);

		if (result.distance == -1)
			return false;

		Vector4D point = result.point;
		point[3] = 1;
		point = Matrix4D::inverse(Matrix4D::getPositionMatrix(plane.getPosition()) * Matrix4D::getRotationMatrix(plane.getNormal())) * point;

		Vector4D normal1 = Vector4D::normalize(plane.P1() - plane.P2());
		Vector4D normal2 = Vector4D::normalize(plane.P2() - plane.P1());
		Vector4D normal3 = Vector4D::normalize(plane.P2() - plane.P3());
		Vector4D normal4 = Vector4D::normalize(plane.P3() - plane.P2());

		Vector4D intersectVector1 = Vector4D::normalize(point - plane.P1());
		Vector4D intersectVector2 = Vector4D::normalize(point - plane.P2());
		Vector4D intersectVector3 = Vector4D::normalize(point - plane.P3());
		Vector4D intersectVector4 = Vector4D::normalize(point - plane.P4());

		float t1 = Vector4D::dot(intersectVector1, normal3);
		float t2 = Vector4D::dot(intersectVector3, normal4);
		float t3 = Vector4D::dot(intersectVector2, normal2);
		float t4 = Vector4D::dot(intersectVector4, normal1);

		if (t1 < 0 && t2 < 0 && t3 < 0 && t4 < 0)
		{
			intersections.push_back(result);
			DebugManager::getInstance().addShape(new DebugSphere(result.point, 0.05f));
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	Vector4D origin, direction;
	vector<IntersectData> intersections;
};