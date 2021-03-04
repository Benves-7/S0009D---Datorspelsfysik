#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <cmath>

#pragma region Vector4D
class Vector4D
{
public:

	explicit Vector4D(float x = 0, float y = 0, float z = 0, float w = 1)
	{
		cord[0] = x; cord[1] = y; cord[2] = z; cord[3] = w;
	}
	Vector4D(float* xyzw)
	{
		cord[0] = xyzw[0]; cord[1] = xyzw[1]; cord[2] = xyzw[2]; cord[3] = xyzw[3];
	}

	~Vector4D() {}

	void set(float x, float y, float z, float w)
	{
		cord[0] = x; cord[1] = y; cord[2] = z; cord[3] = w;
	}

	void setX(float x)
	{
		cord[0] = x;
	}
	void setY(float y)
	{
		cord[1] = y;
	}
	void setZ(float z)
	{
		cord[2] = z;
	}
	void setW(float w)
	{
		cord[3] = w;
	}

	inline float getX()
	{
		return cord[0];
	}
	inline float getY()
	{
		return cord[1];
	}
	inline float getZ()
	{
		return cord[2];
	}
	inline float getW()
	{
		return cord[3];
	}

	inline float* getPointer()
	{
		return cord;
	}

	inline void print() const 
	{
		printf("%0.5f : %0.5f : %0.5f : %0.5f\n", cord[0], cord[1], cord[2], cord[3]); 
	}

	static Vector4D normalize(Vector4D vec)
	{
		float length = vec.length();
		return Vector4D(vec[0] / length, vec[1] / length, vec[2] / length, vec[3] / length);
	}
	static Vector4D normalize3(Vector4D vec)
	{
		float length = vec.length3();
		return Vector4D(vec[0] / length, vec[1] / length, vec[2] / length, vec[3]);
	}
	void normalize()
	{
		float length = this->length();
		set(cord[0] / length, cord[1] / length, cord[2] / length, cord[3] / length);
	}
	void normalize3()
	{
		float length = this->length3();
		set(cord[0] / length, cord[1] / length, cord[2] / length, cord[3]);
	}
	static Vector4D IBMulti(Vector4D a, Vector4D b)
	{
		return Vector4D((a[0] * b[0]), (a[1] * b[1]), (a[2] * b[2]), 1);
	}
	float length()
	{
		return sqrt((this->cord[0] * this->cord[0]) + (this->cord[1] * this->cord[1]) + (this->cord[2] * this->cord[2]) + (this->cord[3] * this->cord[3]));
	}	
	float length3()
	{
		return sqrt((this->cord[0] * this->cord[0]) + (this->cord[1] * this->cord[1]) + (this->cord[2] * this->cord[2]));
	}
	static Vector4D cross(Vector4D lhs, Vector4D rhs)
	{
		Vector4D temp((lhs.cord[1] * rhs.cord[2] - lhs.cord[2] * rhs.cord[1]), (lhs.cord[2] * rhs.cord[0] - lhs.cord[0] * rhs.cord[2]), (lhs.cord[0] * rhs.cord[1] - lhs.cord[1] * rhs.cord[0]), 1.0f);
		return temp;
	}
	static float dot(Vector4D first, Vector4D second)
	{
		return ((first.cord[0] * second.cord[0]) + (first.cord[1] * second.cord[1]) + (first.cord[2] * second.cord[2]) + (first.cord[3] * second.cord[3]));
	}	
	static float dot3(Vector4D first, Vector4D second)
	{
		return ((first.cord[0] * second.cord[0]) + (first.cord[1] * second.cord[1]) + (first.cord[2] * second.cord[2]));
	}
	float dotProduct(const Vector4D& vec) const
	{
		return ((cord[0] * vec.cord[0]) + (cord[1] * vec.cord[1]) + (cord[2] * vec.cord[2]) + (cord[3] * vec.cord[3]));
	}
	static Vector4D Lerp(Vector4D start, Vector4D end, float percent)
	{
		return start + (end - start) * percent;
	}
	static Vector4D Slerp(Vector4D first, Vector4D second, float by)
	{

		double dot = first.dotProduct(second);

		if (dot < 0.0f)
		{
			second = second * -1;
			dot = -dot;
		}

		const double dotThreshold = 0.9995;
		if (dot > dotThreshold)
		{
			return Lerp(first, second, by);
		}

		double theta_0 = acos(dot);
		double theta = theta_0 * by;
		double sin_theta = sin(theta);
		double sin_theta_0 = sin(theta_0);

		double s0 = cos(theta) - dot * sin_theta / sin_theta_0;
		double s1 = sin_theta / sin_theta_0;
		return (first * s0) + (second * s1);

		//float dot = Vector4D::dot((start), (end));

		//dot = clamp(dot, -1.0f, 1.0f);

		//float theta = acos(dot) * percent;

		//Vector4D relVec = end - start * dot; // Might need a look;

		//relVec.normalize();

		//return ((start * cos(theta)) + (relVec * sin(theta)));
	}


	Vector4D operator*(float rhs)
	{
		return Vector4D(this->cord[0] * rhs, this->cord[1] * rhs, this->cord[2] * rhs, this->cord[3] * rhs);
	}
	Vector4D operator+(Vector4D& rhs)
	{
		return Vector4D((this->cord[0] + rhs.cord[0]), (this->cord[1] + rhs.cord[1]), (this->cord[2] + rhs.cord[2]), (this->cord[3] + rhs.cord[3]));
	}
	Vector4D operator-(Vector4D& rhs)
	{
		return Vector4D((this->cord[0] - rhs.cord[0]), (this->cord[1] - rhs.cord[1]), (this->cord[2] - rhs.cord[2]), (this->cord[3] - rhs.cord[3]));
	}
	void operator+=(Vector4D rhs) 
	{
		this->cord[0] = this->cord[0] + rhs.cord[0];
		this->cord[1] = this->cord[1] + rhs.cord[1];
		this->cord[2] = this->cord[2] + rhs.cord[2];
	}
	void operator-=(Vector4D rhs) 
	{
		this->cord[0] = this->cord[0] - rhs.cord[0];
		this->cord[1] = this->cord[1] - rhs.cord[1];
		this->cord[2] = this->cord[2] - rhs.cord[2];
	}
	float& operator[] (const int x)	{ return cord[x]; }
	float operator[] (const int x) const { return cord[x]; }

private:
	float cord[4];

	static float clamp(float num, float lo, float hi)
	{
		if (num <= lo)
			return lo;
		else if (num >= hi)
			return hi;
		else
			return num;
	}
};
#pragma endregion