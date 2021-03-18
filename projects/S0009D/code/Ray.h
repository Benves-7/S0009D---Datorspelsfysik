#pragma once
#include "debug/debugShapes.h"
#include "Square.h"
#include "Plane.h"
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
	// Get a point i steps in the rays direction.
	Vector4D getPointOnRay(int i = 0)
	{
		return origin + (direction * i);
	}

	// Goes through all squares in the vector. (returns the index of the closest intersect square.)
	int bestHit(vector<Square> squares)
	{
		float closest = std::numeric_limits<float>::infinity();
		int best = -1;
		for (int i = 0; i < squares.size(); i++)
		{
			float dist = checkIfHitting(squares[i]);
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
	// Check if intersection exists and is valid, then check if inside square. (returns the distance to intersect point.) 
	float checkIfHitting(Square square)
	{
		// Check if intersecting the plane
		IntersectData result = intersect(Plane(square.getPosition(), square.getNormal()));
		if (result.distance == -1)
			return -1;
		
		// check if intersection is inside the square.
		if (pointInSquare(result.point, square))
		{
			DebugManager::getInstance().createSphere(result.point, 0.03, Vector4D(0,1,0));
			return result.distance;
		}
		return -1;
	}

	// Goes through all objects in the vector. (return the index of the closest intersect object.)
	int bestHit(vector<MeshObject> objects)
	{
		vector<MeshObject> hits;
		// Check if AABB is hit by the ray. add the object to temporary vector.
		for (int i = 0; i < objects.size(); i++)
			if (bestHit(objects[i].getAABB()) != -1)
				hits.push_back(objects[i]);

		// Go deeper.
		float closest = std::numeric_limits<float>::infinity();
		int best = -1;
		for (int i = 0; i < hits.size(); i++)
		{
			float dist = checkIfHitting(hits[i]);
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
	// Check if intersection exists and is valid, then check if inside triangle of mesh. (returns the distance to intersect point.)
	float checkIfHitting(MeshObject object)
	{
		vector<IntersectData> temp;
		int closest = numeric_limits<int>::infinity();
		MeshInfo mi = object.getMeshInfo();
		// Go through all the indicies in steps of threes.
		for (int i = 0; i < mi.indices.size(); i += 3)
		{
			// Step through the three vertices and extract the normals into Vector class.
			Vector4D n1, n2, n3;
			n1[0] = mi.combinedBuffer[mi.indices[i+0]].normal[0]; n1[1] = mi.combinedBuffer[mi.indices[i+0]].normal[1]; n1[2] = mi.combinedBuffer[mi.indices[i+0]].normal[2];
			n2[0] = mi.combinedBuffer[mi.indices[i+1]].normal[0]; n2[1] = mi.combinedBuffer[mi.indices[i+1]].normal[1]; n2[2] = mi.combinedBuffer[mi.indices[i+1]].normal[2];
			n3[0] = mi.combinedBuffer[mi.indices[i+2]].normal[0]; n3[1] = mi.combinedBuffer[mi.indices[i+2]].normal[1]; n3[2] = mi.combinedBuffer[mi.indices[i+2]].normal[2];
			// the planes normal will be the three normals added together and divided by three.
			Vector4D nor = (n1+n2+n3) * (1.0f / 3.0f);
			// move and rotate and scale the normal acording to the objects new position and rotation. the position and the scale is not needed, since it is only a direction. (however it seems worth having still)
			nor = Matrix4D::getPositionMatrix(object.getPosition()) * Matrix4D::getRotationMatrix(object.getDirection()) * Matrix4D::getScaleMatrix(object.getScale()) * nor;
			if (Vector4D::dot(nor, direction) < 0)
			{
				// Step through the three vertices and extract the positions into Vector class.
				Vector4D p1, p2, p3;
				p1[0] = mi.combinedBuffer[mi.indices[i + 0]].pos[0]; p1[1] = mi.combinedBuffer[mi.indices[i + 0]].pos[1]; p1[2] = mi.combinedBuffer[mi.indices[i + 0]].pos[2];
				p2[0] = mi.combinedBuffer[mi.indices[i + 1]].pos[0]; p2[1] = mi.combinedBuffer[mi.indices[i + 1]].pos[1]; p2[2] = mi.combinedBuffer[mi.indices[i + 1]].pos[2];
				p3[0] = mi.combinedBuffer[mi.indices[i + 2]].pos[0]; p3[1] = mi.combinedBuffer[mi.indices[i + 2]].pos[1]; p3[2] = mi.combinedBuffer[mi.indices[i + 2]].pos[2];
				// the planes normal will be the three positions added together and divided by three.
				Vector4D pos = (p1 + p2 + p3) * (1.0f / 3.0f);
				// move and rotate and scale the position acording to the objects new position and rotation. a point in space so all the matrixes are needed.
				pos = Matrix4D::getPositionMatrix(object.getPosition()) * Matrix4D::getRotationMatrix(object.getDirection()) * Matrix4D::getScaleMatrix(object.getScale()) * pos;

				// Check if intersecting the plane
				IntersectData result = intersect(Plane(pos, nor));
				if (result.distance == -1)
					continue;
				// Check if intersection is inside the square.
				if (pointInTriangle(result.point, p1, p2, p3, nor))
				{
					// Instead of returning straight away, add it to a new list (safe guard to insure two matches can't be found. (head and ass of cat.) 
					// (though this should never be the case when that normal should be in the wrong direction)
					temp.push_back(result);
				}
			}
		}
		for (int i = 0; i < temp.size(); i++)
		{
			// Find the closest match and return that distance.
			DebugManager::getInstance().createSphere(temp[i].point, 0.005, Vector4D(0, 0, 1));
			if (temp[i].distance < closest)
			{
				closest = temp[i].distance;
			}
		}
		return closest;
	}

	// Check if a point is inside the bounding box of three other points.
	bool pointInTriangle(Vector4D p, Vector4D a, Vector4D b, Vector4D c, Vector4D normal)
	{
		Vector4D ab = b - a; Vector4D bc = c - b; Vector4D ca = a - c;
		Vector4D ap = p - a; Vector4D bp = p - b; Vector4D cp = p - c;

		if (Vector4D::dot(normal, Vector4D::cross(ab, ap)) > 0 && Vector4D::dot(normal, Vector4D::cross(bc, bp)) > 0 && Vector4D::dot(normal, Vector4D::cross(ca, cp)) > 0)
			return true;
		return false;
	}
	// Check if a point is inside the bounding box of a square object.
	bool pointInSquare(Vector4D p, Square s)
	{
		Vector4D b = s.P1();
		Vector4D c = s.P2();
		Vector4D d = s.P3();
		Vector4D e = s.P4();

		if ((Vector4D::dot(b, c - b) <= Vector4D::dot(p, c - b) && Vector4D::dot(p, c - b) <= Vector4D::dot(c, c - b)) && 
			(Vector4D::dot(b, e - b) <= Vector4D::dot(p, e - b) && Vector4D::dot(p, e - b) <= Vector4D::dot(e, e - b)))
			return true;
		return false;
	}

	// Check if ray intersect plane. (returns point of intersect and distance to intersect.) (IntersectData)
	IntersectData intersect(Plane plane)
	{
		Vector4D diff = origin - plane.getPosition();

		float prod1 = Vector4D::dot(plane.getNormal(), diff);
		if (prod1 < 0)
			return IntersectData(Vector4D(), -1);
		float prod2 = Vector4D::dot(plane.getNormal(), direction);
		if (prod2 == 0)
			return IntersectData(Vector4D(), -1);
		float prod3 = -(prod1 / prod2);

		return IntersectData((origin + direction * prod3), prod3);
	}

private:
	Vector4D origin, direction;
	vector<IntersectData> intersections;
};