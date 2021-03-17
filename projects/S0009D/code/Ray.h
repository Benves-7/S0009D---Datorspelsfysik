#pragma once
#include "debug/debugShapes.h"
#include "Square.h"
#include "MeshObject.h"
#include <limits>

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
		DebugManager::getInstance().createLine(pos, (pos + dir * 100));
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

	// Goes through all planes in the vector. (returns the index of the closest intersect plane.)
	int checkHit(vector<Square> planes)
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
		if (closest == std::numeric_limits<float>::infinity())
			closest = -1;
		return best;
	}
	// Check if a point is in individual square. (returns the distance to intersect point.) 
	float checkIfHitting(Square plane)
	{
		IntersectData result = intersect(plane);
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
	// Check if ray intersect plane. (returns point of intersect and distance to intersect.) (IntersectData)
	IntersectData intersect(Square plane)
	{
		Vector4D diff = origin - plane.getPosition();

		float prod1 = diff.dot(plane.getNormal());
		float prod2 = direction.dot(plane.getNormal());
		float prod3 = prod1 / prod2;

		// with the distance a point on the plane can be calculated.
		return IntersectData((origin - direction * prod3), -prod3);
	}

	// Goes through all objects in the vector. (return the index of the closest intersect object.)
	int checkHit(vector<MeshObject> objects)
	{
		vector<MeshObject> hits;
		// Check if AABB is hit by the ray. add the object to temporary vector.
		for (int i = 0; i < objects.size(); i++)
			if (checkHit(objects[i].getAABB()) != -1)
				hits.push_back(objects[i]);

		// Go deeper.
		float closest = std::numeric_limits<float>::infinity();
		int best = -1;
		for (int i = 0; i < hits.size(); i++)
		{
			MeshInfo mi = hits[i].getMeshInfo();
			Vector4D n1, n2, n3;
			origin = Matrix4D::inverse(Matrix4D::getPositionMatrix(hits[i].getPosition())) * origin;
			direction = Matrix4D::inverse(Matrix4D::getPositionMatrix(hits[i].getPosition())) * direction;
			for (int j = 0; j < mi.indices.size(); j += 3)
			{
				n1[0] = mi.combinedBuffer[mi.indices[i + 0]].normal[0]; n1[1] = mi.combinedBuffer[mi.indices[i + 0]].normal[1]; n1[2] = mi.combinedBuffer[mi.indices[i + 0]].normal[2];
				n2[0] = mi.combinedBuffer[mi.indices[i + 1]].normal[0]; n2[1] = mi.combinedBuffer[mi.indices[i + 1]].normal[1]; n2[2] = mi.combinedBuffer[mi.indices[i + 1]].normal[2];
				n3[0] = mi.combinedBuffer[mi.indices[i + 2]].normal[0]; n3[1] = mi.combinedBuffer[mi.indices[i + 2]].normal[1]; n3[2] = mi.combinedBuffer[mi.indices[i + 2]].normal[2];

				Vector4D normal = (n1 + n2 + n3) * (1.0f / 3.0f);
				if (Vector4D::dot(normal, direction) < 0)
				{
					Vector4D pos1, pos2, pos3;
					pos1[0] = mi.combinedBuffer[mi.indices[i+0]].pos[0]; pos1[1] = mi.combinedBuffer[mi.indices[i+0]].pos[1]; pos1[2] = mi.combinedBuffer[mi.indices[i+0]].pos[2];
					pos2[0] = mi.combinedBuffer[mi.indices[i+1]].pos[0]; pos2[1] = mi.combinedBuffer[mi.indices[i+1]].pos[1]; pos2[2] = mi.combinedBuffer[mi.indices[i+1]].pos[2];
					pos3[0] = mi.combinedBuffer[mi.indices[i+2]].pos[0]; pos3[1] = mi.combinedBuffer[mi.indices[i+2]].pos[1]; pos3[2] = mi.combinedBuffer[mi.indices[i+2]].pos[2];
					Vector4D v2v1 = pos2 - pos1; Vector4D v3v2 = pos3 - pos2; Vector4D v1v3 = pos1 - pos3;
					Vector4D pos = (pos1 + pos2 + pos3) * (1.0f / 3.0f);

					IntersectData temp = intersect(Square(pos, Vector2D(1, 1), normal));
					Vector4D PV0 = temp.point - pos1; Vector4D PV1 = temp.point - pos2; Vector4D PV2 = temp.point - pos3;
					if (Vector4D::dot(normal, Vector4D::cross(v2v1, PV0)) > 0 &&
						Vector4D::dot(normal, Vector4D::cross(v3v2, PV1)) > 0 &&
						Vector4D::dot(normal, Vector4D::cross(v1v3, PV2)) > 0)
					{
						DebugManager::getInstance().createSphere(Matrix4D::getPositionMatrix(hits[i].getPosition()) * temp.point, 0.03);
						best = i;
					}
				}
			}
		}
		if (closest == std::numeric_limits<float>::infinity())
			closest = -1;
		return best;
	}
	float checkIfHitting(vector<Vector4D> points)
	{
		IntersectData result = intersect(points[0], points[1], points[2]);
		if (pointInTriangle(result.point, points))
		{
			return result.distance;
		}
		return -1;
	}
	IntersectData intersect(Vector4D A, Vector4D B, Vector4D C)
	{
		Vector4D AB = (B - A); Vector4D AC = (C - A);
		Vector4D N = Vector4D::cross(AB, AC);

		float d = Vector4D::dot(direction, N);
		if (d == 0)
			return IntersectData(Vector4D(0, 0, 0, 0), -1);

		float dist = Vector4D::dot((A - direction), N) / d;

		return IntersectData((origin - direction * dist), -dist);
	}
	bool pointInTriangle(Vector4D p, vector<Vector4D> points)
	{
		points[0] -= p;
		points[1] -= p;
		points[2] -= p;

		Vector4D u = Vector4D::cross(points[1], points[2]);
		Vector4D v = Vector4D::cross(points[2], points[0]);
		Vector4D w = Vector4D::cross(points[0], points[1]);

		if (Vector4D::dot(u, v) < 0.0f && Vector4D::dot(u, w) < 0.0f)
			return false;
		return true;
	}
private:
	Vector4D origin, direction;
	vector<IntersectData> intersections;
};