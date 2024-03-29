#pragma once
#include <iostream>
#include <string>
#include <math.h>

#pragma region Vector2D
class Vector2D
{
public:
	/// Constructor
	//====================================================================================================
	Vector2D() 
	{
		cord[0] = 0.0f;
		cord[1] = 0.0f;
	}
	Vector2D(float x, float y)
	{
		cord[0] = x;
		cord[1] = y;
	}

	/// Deconstructor
	//====================================================================================================
	~Vector2D()
	{

	}

	/// Functions
	//====================================================================================================
	void Set(float x, float y)
	{
		cord[0] = x;
		cord[1] = y;
	}
	void SetX(float x)
	{
		cord[0] = x;
	}
	void SetY(float y)
	{
		cord[1] = y;
	}

	float GetX() 
	{
		return cord[0];
	}
	float GetY() 
	{
		return cord[1]; 
	}

	float Norm() 
	{
		return sqrt((cord[0] * cord[0]) + (cord[1] * cord[1]));
	}
	void Print() const 
	{ 
		printf("%0.1f : %0.1f\n", cord[0], cord[1]);
	}

	/// Operators
	//====================================================================================================
	float operator*(Vector2D rhs)
	{
		return (this->GetX() * rhs.GetX() + this->GetY() * rhs.GetY());
	}
	void operator*(float rhs)
	{
		this->cord[0] = this->cord[0] * rhs;
		this->cord[1] = this->cord[1] * rhs;
	}
	Vector2D operator+(Vector2D& rhs)
	{
		return Vector2D((this->GetX() + rhs.GetX()), (this->GetY() + rhs.GetY()));
	}
	Vector2D operator-(Vector2D& rhs)
	{
		return Vector2D((this->GetX() - rhs.GetX()), (this->GetY() - rhs.GetY()));
	}
	void operator=(Vector2D& rhs)
	{
		this->cord[0] = rhs.GetX();
		this->cord[1] = rhs.GetY();
	}
	bool operator==(Vector2D& rhs)
	{
		if (this->GetX() == rhs.GetX() && this->GetY() == rhs.GetY())
		{
			return true;
		}
		return false;
	}
	bool operator!=(Vector2D& rhs)
	{
		if (this->GetX() != rhs.GetX() || this->GetY() != rhs.GetY())
		{
			return true;
		}
		return false;
	}
	bool operator<(Vector2D& rhs)
	{ // is this smaller then rhs
		if (this->GetX() < rhs.GetX() && this->GetY() < rhs.GetY())
		{
			return true;
		}
		return false;
	}
	bool operator<=(Vector2D& rhs)
	{ // is this smaller then rhs
		if (this->GetX() <= rhs.GetX() && this->GetY() <= rhs.GetY())
		{
			return true;
		}
		return false;
	}
	bool operator>(Vector2D& rhs)
	{ // is this bigger then rhs
		if (this->GetX() > rhs.GetX() && this->GetY() > rhs.GetY())
		{
			return true;
		}
		return false;
	}
	bool operator>=(Vector2D& rhs)
	{ // is this bigger then rhs
		if (this->GetX() >= rhs.GetX() && this->GetY() >= rhs.GetY())
		{
			return true;
		}
		return false;
	}
	//Returns a refrence to the value of the x position in the Vector
	float& operator[] (const int x)			//Set
	{
		return cord[x];
	}
	//Returns a constant to the value of the x position in the Vector
	float operator[] (const int x) const	//Get
	{
		return cord[x];
	}

private:
	float cord[2];
};
#pragma endregion

#pragma region Matrix2D
// Look of matrix
//      x1y1, x2y1			0, 1
//		x1y2, x2y2			2, 3

class Matrix2D
{
public:
	/// Constructor
	//====================================================================================================
	Matrix2D()
	{
		matrix[0] = 1.0f; matrix[1] = 0.0f;
		matrix[2] = 0.0f; matrix[3] = 1.0f;
	}
	Matrix2D(float x1y1, float x2y1, float x1y2, float x2y2)
	{
		matrix[0] = x1y1; matrix[1] = x2y1;
		matrix[2] = x1y2; matrix[3] = x2y2;
	}
	Matrix2D(const Matrix2D& a)
	{
		matrix[0] = a[0]; matrix[1] = a[1];
		matrix[2] = a[2]; matrix[3] = a[3];
	}
	Matrix2D(float degrees)
	{
		matrix[0] = cos(degrees); matrix[1] = -sin(degrees);
		matrix[2] = sin(degrees); matrix[3] = cos(degrees);
	}

	/// Deconstructor
	//====================================================================================================
	~Matrix2D() 
	{

	}

	/// Functions
	//====================================================================================================
	void Set(float x1y1, float x2y1, float x1y2, float x2y2)
	{
		matrix[0] = x1y1; matrix[1] = x2y1;
		matrix[2] = x1y2; matrix[3] = x2y2;
	}

	void Rot(float degrees)
	{
		matrix[0] = cos(degrees); matrix[1] = -sin(degrees);
		matrix[2] = sin(degrees); matrix[3] = cos(degrees);
	}
	static Matrix2D Transponat(Matrix2D a)
	{
		return Matrix2D(a[0], a[2],
						a[1], a[3]);
	}
	
	inline void Print() const 
	{
		printf("\n Matrix = %0.1f : %0.1f \n          %0.1f : %0.1f\n", matrix[0], matrix[1], matrix[2], matrix[3]); 
	}

	/// Operators
	//====================================================================================================
	void operator=(Matrix2D& rhs)
	{
		Set(rhs[0], rhs[1], rhs[2], rhs[3]);
	}
	void operator*(float rhs)
	{
		matrix[0] *= rhs;
		matrix[1] *= rhs;
		matrix[2] *= rhs;
		matrix[3] *= rhs;
	}
	Vector2D operator*(Vector2D rhs)
	{
		return Vector2D(matrix[0] * rhs[0] + matrix[1] * rhs[1], matrix[2] * rhs[0] + matrix[3] * rhs[1]);
	}
	Matrix2D operator*(Matrix2D rhs)
	{
		return Matrix2D(((matrix[0] * rhs[0]) + (matrix[1] * rhs[2])),((matrix[0] * rhs[1]) + (matrix[1] * rhs[3])),
						((matrix[2] * rhs[0]) + (matrix[3] * rhs[2])), ((matrix[2] * rhs[1]) + (matrix[3] * rhs[3])));
	}

	//Returns a refrence to the value of the x position in the Vector
	float& operator[] (const int x)			//Set
	{
		return matrix[x];
	}
	//Returns a constant to the value of the x position in the Vector
	float operator[] (const int x) const	//Get
	{
		return matrix[x];
	}

private:

	float matrix[4];

};
#pragma endregion

#pragma region Matrix3D
class Matrix3D
{
public:
	/// Constructor
	//====================================================================================================
	Matrix3D()
	{
		for (int i = 0; i < 9; i++)
				matrix[i] = 0.0f;
		matrix[0] = 1.0f;
		matrix[4] = 1.0f;
		matrix[8] = 1.0f;
	}
	Matrix3D(float x1 = 1.0f, float y1 = 0.0f, float z1 = 0.0f, float x2 = 0.0f, float y2 = 1.0f, float z2 = 0.0f, float x3 = 0.0f, float y3 = 0.0f, float z3 = 1.0f)
	{
		matrix[0] = x1;		matrix[1] = y1;		matrix[2] = z1;
		matrix[3] = x2;		matrix[4] = y2;		matrix[5] = z2;
		matrix[6] = x3;		matrix[7] = y3;		matrix[8] = z3;
	}
	Matrix3D(const Matrix2D& a)
	{
		matrix[0] = a[0];	matrix[1] = a[1];	matrix[2] = 0.0f;
		matrix[3] = a[2];	matrix[4] = a[3];	matrix[5] = 0.0f;
		matrix[6] = 0.0f;	matrix[7] = 0.0f;	matrix[8] = 1.0f;
	}
	Matrix3D(float degrees)
	{
		matrix[0] = cos(degrees);	matrix[1] = -sin(degrees);	matrix[2] = 0.0f;
		matrix[3] = sin(degrees);	matrix[4] = cos(degrees);	matrix[5] = 0.0f;
		matrix[6] = 0.0f;			matrix[7] = 0.0f;			matrix[8] = 1.0f;
	}

	/// Deconstructor
	//====================================================================================================
	~Matrix3D()
	{

	}

	/// Functions
	//====================================================================================================
	void Set(float x1 = 1.0f, float y1 = 0.0f, float z1 = 0.0f, float x2 = 0.0f, float y2 = 1.0f, float z2 = 0.0f, float x3 = 0.0f, float y3 = 0.0f, float z3 = 1.0f)
	{
		matrix[0] = x1;		matrix[1] = y1;		matrix[2] = z1;
		matrix[3] = x2;		matrix[4] = y2;		matrix[5] = z2;
		matrix[6] = x3;		matrix[7] = y3;		matrix[8] = z3;
	}
	void Rot(float degrees)
	{
		matrix[0] = cos(degrees);
		matrix[1] = -sin(degrees);
		matrix[2] = sin(degrees);
		matrix[3] = cos(degrees);
	}
	static Matrix3D Transponat2D(Matrix3D a)
	{
		return Matrix3D(a[0], a[3], a[2],
						a[1], a[4], a[5],
						a[6], a[7], a[8]);
	}
	static Matrix3D Transponat3D(Matrix3D a)
	{
		return Matrix3D(a[0], a[3], a[6],
						a[1], a[4], a[7],
						a[2], a[5], a[8]);
	}

	void SetRot(float degrees)
	{
		matrix[0] = cos(degrees);
		matrix[1] = -sin(degrees);
		matrix[2] = sin(degrees);
		matrix[3] = cos(degrees);
		matrix[8] = degrees;
	}
	void SetSize(float sizeX, float sizeY)
	{
		matrix[6] = sizeX;
		matrix[7] = sizeY;
	}
	void SetPosition(Vector2D v)
	{
		matrix[2] = v[0];
		matrix[5] = v[1];
	}

	Vector2D GetPosition()
	{
		return Vector2D(matrix[2], matrix[5]);
	}
	void TranslationMatrix(Vector2D v)
	{
		for (int i = 0; i < 9; i++)
		{
			matrix[i] = 0;
		}
		matrix[0] = 1.0f;
		matrix[4] = 1.0f;
		matrix[8] = 1.0f;
	}

	inline float GetSizeX()	const 
	{
		return matrix[2]; 
	};
	inline float GetSizeY()	const 
	{
		return matrix[5]; 
	};
	inline float GetRadius() const 
	{
		return matrix[8]; 
	};
	inline void Print()	const 
	{ 
		printf("\n Matrix = %0.1f : %0.1f			\n          %0.1f : %0.1f			\n Position X = %0.1f			\n          Y = %0.1f", matrix[0], matrix[1], matrix[3], matrix[4], matrix[2], matrix[5]); }

	/// Operators
	//====================================================================================================
	Vector2D operator* (Vector2D& rhs)
	{
		return Vector2D((matrix[0] * rhs[0]) + (matrix[1] * rhs[1]), (matrix[3] * rhs[0]) + (matrix[4] * rhs[1]));
	}

	//Returns a refrence to the value of the x position in the Vector
	float& operator[] (const int x)			//Set
	{
		return matrix[x];
	}
	//Returns a constant to the value of the x position in the Vector
	float operator[] (const int x) const	//Get
	{
		return matrix[x];
	}

private:

	float matrix[9];

};
#pragma endregion

#pragma region Vector4D
class Vector4D
{
public:
	/// Constructor
	//====================================================================================================
	explicit Vector4D(float x = 0, float y = 0, float z = 0, float w = 1)
	{
		cord[0] = x; cord[1] = y; cord[2] = z; cord[3] = w;
	}

	/// Deconstructor
	//====================================================================================================
	~Vector4D()
	{

	}

	/// Functions
	//====================================================================================================
	void Set(float x = 0, float y = 0, float z = 0, float w = 1)
	{
		cord[0] = x; cord[1] = y; cord[2] = z; cord[3] = w;
	}
	inline float* GetPointer()
	{
		return cord;
	}

	inline void print() const
	{
		printf("%0.1f : %0.1f : %0.1f : %0.1f\n", cord[0], cord[1], cord[2], cord[3]);
	}

	float length() const
	{
		float v = (cord[0] * cord[0]) + (cord[1] * cord[1]) + (cord[2] * cord[2]);
		v = (v != 0) ? v : 1.0f;
		return sqrt(v);
	}
	static Vector4D normalize(Vector4D vec)
	{
		float len = 1.0f / vec.length();
		return Vector4D(vec[0] * len, vec[1] * len, vec[2] * len, 1.0f);
	}
	void normalize()
	{
		float len = 1.0f / length();
		cord[0] *= len;
		cord[1] *= len;
		cord[2] *= len;
		cord[3] = 1.0f;
	}

	static float dot(const Vector4D a, const Vector4D& b)
	{
		return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
	}
	float dot(const Vector4D& rhs)
	{
		return cord[0] * rhs[0] + cord[1] * rhs[1] + cord[2] * rhs[2];
	}
	static Vector4D cross(const Vector4D a, const Vector4D b)
	{
		return Vector4D((a[1] * b[2] - a[2] * b[1]), (a[2] * b[0] - a[0] * b[2]), (a[0] * b[1] - a[1] * b[0]), 1.0f);
	}

	static Vector4D IBMulti(Vector4D a, Vector4D b) // IndexBasedMultiplication
	{
		return Vector4D(a[0] * b[0], a[1] * b[1], a[2] * b[2], 1.0f);
	}

	/// Operators
	//====================================================================================================
	Vector4D operator*(const float scalor) const // Scaling
	{
		return Vector4D(cord[0] * scalor, cord[1] * scalor, cord[2] * scalor);
	}
	Vector4D operator+(const Vector4D& rhs) const //Addition
	{
		return Vector4D((cord[0] + rhs[0]), (cord[1] + rhs[1]), (cord[2] + rhs[2]));
	}
	Vector4D operator-(const Vector4D& rhs) const //Subtraction
	{
		return Vector4D((cord[0] - rhs[0]), (cord[1] - rhs[1]), (cord[2] - rhs[2]));
	}
	void operator+=(Vector4D rhs) // Add to
	{
		cord[0] = cord[0] + rhs[0];
		cord[1] = cord[1] + rhs[1];
		cord[2] = cord[2] + rhs[2];
	}
	void operator-=(Vector4D rhs) // Remove from
	{
		cord[0] = cord[0] - rhs[0];
		cord[1] = cord[1] - rhs[1];
		cord[2] = cord[2] - rhs[2];
	}

	//Returns a refrence to the value of the x position in the Vector
	float& operator[] (const int x)			//Set
	{
		return cord[x];
	}
	//Returns a constant to the value of the x position in the Vector
	float operator[] (const int x) const	//Get
	{
		return cord[x];
	}

private:
	float cord[4];
};
#pragma endregion

#pragma region Matrix4D
class Matrix4D
{
public:
	/// Constructor
	//====================================================================================================
	Matrix4D(float arr[16])
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = arr[i];
		}
	}
	Matrix4D(float x1 = 1, float y1 = 0, float z1 = 0, float w1 = 0, float x2 = 0, float y2 = 1, float z2 = 0, float w2 = 0, float x3 = 0, float y3 = 0, float z3 = 1, float w3 = 0, float x4 = 0, float y4 = 0, float z4 = 0, float w4 = 1)
	{
		matrix[0]  = x1; matrix[1]  = y1; matrix[2]  = z1; matrix[3]  = w1;
		matrix[4]  = x2; matrix[5]  = y2; matrix[6]  = z2; matrix[7]  = w2;
		matrix[8]  = x3; matrix[9]  = y3; matrix[10] = z3; matrix[11] = w3;
		matrix[12] = x4; matrix[13] = y4; matrix[14] = z4; matrix[15] = w4;
	}
	Matrix4D(const Matrix4D& a)
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = a[i];
		}
	}

	/// Deconstructor
	//====================================================================================================
	~Matrix4D()
	{

	}

	/// Functions
	//====================================================================================================
	void SetI()
	{
		for (int i = 0; i < 16; i++)
		{
			if (i == 0 || i == 5 || i == 10 || i == 15)
			{
				matrix[i] = 1.0f;
			}
			else
			{
				matrix[i] = 0.0f;
			}
		}
	}

	static Matrix4D rotX(float degree) 
	{
		const float c = cos(degree);
		const float s = sin(degree);
		return Matrix4D(1, 0, 0, 0,
						0, c,-s, 0,
						0, s, c, 0,
						0, 0, 0, 1);
	}
	static Matrix4D rotY(float degree)
	{
		const float c = cos(degree);
		const float s = sin(degree);
		return Matrix4D(c, 0, s, 0,
						0, 1, 0, 0,
					   -s, 0, c, 0,
						0, 0, 0, 1);
	}
	static Matrix4D rotZ(float degree)
	{
		const float c = cos(degree);
		const float s = sin(degree);
		return Matrix4D(c,-s, 0, 0,
						s, c, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
	}
	static Matrix4D rotVec(Vector4D& vec, float degree)
	{
		Vector4D nv = Vector4D::normalize(vec);
		Matrix4D C(	0,    -nv[2], nv[1], 0,
					nv[2], 0,    -nv[0], 0,
				   -nv[1], nv[0], 0,	 0,
					0,	   0,	  0,	 1 );
		Matrix4D I;
		Matrix4D rotMat = I + (C * sin(degree)) + (C * C) * (1 - cos(degree));
		rotMat[15] = 1;

		return rotMat;
	}

	static Matrix4D transpose(const Matrix4D& mat)
	{
		float returnMatrix[16];
		returnMatrix[0] = mat[0];	returnMatrix[1] = mat[4];	returnMatrix[2] = mat[8];	returnMatrix[3] = mat[12];
		returnMatrix[4] = mat[1];	returnMatrix[5] = mat[5];	returnMatrix[6] = mat[9];	returnMatrix[7] = mat[13];
		returnMatrix[8] = mat[2];	returnMatrix[9] = mat[6];	returnMatrix[10] = mat[10]; returnMatrix[11] = mat[14];
		returnMatrix[12] = mat[3];	returnMatrix[13] = mat[7];	returnMatrix[14] = mat[11];	returnMatrix[15] = mat[15];
		return Matrix4D(returnMatrix);
	}
	static Matrix4D inverse(const Matrix4D& mat)
	{
		float inv[16], determinant, out[16];

		inv[0] = mat[5] * mat[10] * mat[15] -
			mat[5] * mat[11] * mat[14] -
			mat[9] * mat[6] * mat[15] +
			mat[9] * mat[7] * mat[14] +
			mat[13] * mat[6] * mat[11] -
			mat[13] * mat[7] * mat[10];

		inv[4] = -mat[4] * mat[10] * mat[15] +
			mat[4] * mat[11] * mat[14] +
			mat[8] * mat[6] * mat[15] -
			mat[8] * mat[7] * mat[14] -
			mat[12] * mat[6] * mat[11] +
			mat[12] * mat[7] * mat[10];

		inv[8] = mat[4] * mat[9] * mat[15] -
			mat[4] * mat[11] * mat[13] -
			mat[8] * mat[5] * mat[15] +
			mat[8] * mat[7] * mat[13] +
			mat[12] * mat[5] * mat[11] -
			mat[12] * mat[7] * mat[9];

		inv[12] = -mat[4] * mat[9] * mat[14] +
			mat[4] * mat[10] * mat[13] +
			mat[8] * mat[5] * mat[14] -
			mat[8] * mat[6] * mat[13] -
			mat[12] * mat[5] * mat[10] +
			mat[12] * mat[6] * mat[9];

		inv[1] = -mat[1] * mat[10] * mat[15] +
			mat[1] * mat[11] * mat[14] +
			mat[9] * mat[2] * mat[15] -
			mat[9] * mat[3] * mat[14] -
			mat[13] * mat[2] * mat[11] +
			mat[13] * mat[3] * mat[10];

		inv[5] = mat[0] * mat[10] * mat[15] -
			mat[0] * mat[11] * mat[14] -
			mat[8] * mat[2] * mat[15] +
			mat[8] * mat[3] * mat[14] +
			mat[12] * mat[2] * mat[11] -
			mat[12] * mat[3] * mat[10];

		inv[9] = -mat[0] * mat[9] * mat[15] +
			mat[0] * mat[11] * mat[13] +
			mat[8] * mat[1] * mat[15] -
			mat[8] * mat[3] * mat[13] -
			mat[12] * mat[1] * mat[11] +
			mat[12] * mat[3] * mat[9];

		inv[13] = mat[0] * mat[9] * mat[14] -
			mat[0] * mat[10] * mat[13] -
			mat[8] * mat[1] * mat[14] +
			mat[8] * mat[2] * mat[13] +
			mat[12] * mat[1] * mat[10] -
			mat[12] * mat[2] * mat[9];

		inv[2] = mat[1] * mat[6] * mat[15] -
			mat[1] * mat[7] * mat[14] -
			mat[5] * mat[2] * mat[15] +
			mat[5] * mat[3] * mat[14] +
			mat[13] * mat[2] * mat[7] -
			mat[13] * mat[3] * mat[6];

		inv[6] = -mat[0] * mat[6] * mat[15] +
			mat[0] * mat[7] * mat[14] +
			mat[4] * mat[2] * mat[15] -
			mat[4] * mat[3] * mat[14] -
			mat[12] * mat[2] * mat[7] +
			mat[12] * mat[3] * mat[6];

		inv[10] = mat[0] * mat[5] * mat[15] -
			mat[0] * mat[7] * mat[13] -
			mat[4] * mat[1] * mat[15] +
			mat[4] * mat[3] * mat[13] +
			mat[12] * mat[1] * mat[7] -
			mat[12] * mat[3] * mat[5];

		inv[14] = -mat[0] * mat[5] * mat[14] +
			mat[0] * mat[6] * mat[13] +
			mat[4] * mat[1] * mat[14] -
			mat[4] * mat[2] * mat[13] -
			mat[12] * mat[1] * mat[6] +
			mat[12] * mat[2] * mat[5];

		inv[3] = -mat[1] * mat[6] * mat[11] +
			mat[1] * mat[7] * mat[10] +
			mat[5] * mat[2] * mat[11] -
			mat[5] * mat[3] * mat[10] -
			mat[9] * mat[2] * mat[7] +
			mat[9] * mat[3] * mat[6];

		inv[7] = mat[0] * mat[6] * mat[11] -
			mat[0] * mat[7] * mat[10] -
			mat[4] * mat[2] * mat[11] +
			mat[4] * mat[3] * mat[10] +
			mat[8] * mat[2] * mat[7] -
			mat[8] * mat[3] * mat[6];

		inv[11] = -mat[0] * mat[5] * mat[11] +
			mat[0] * mat[7] * mat[9] +
			mat[4] * mat[1] * mat[11] -
			mat[4] * mat[3] * mat[9] -
			mat[8] * mat[1] * mat[7] +
			mat[8] * mat[3] * mat[5];

		inv[15] = mat[0] * mat[5] * mat[10] -
			mat[0] * mat[6] * mat[9] -
			mat[4] * mat[1] * mat[10] +
			mat[4] * mat[2] * mat[9] +
			mat[8] * mat[1] * mat[6] -
			mat[8] * mat[2] * mat[5];

		determinant = mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];

		if (determinant == 0)
			throw "Matrix does not have an inverse";

		determinant = 1.0 / determinant;

		for (int i = 0; i < 16; i++)
			out[i] = inv[i] * determinant;

		return Matrix4D(out);
	}

	float* getPointer()
	{
		return matrix;
	}

	static Matrix4D getPositionMatrix(const Vector4D& inVector)
	{
		return Matrix4D(1,0,0,inVector[0],
						0,1,0,inVector[1],
						0,0,1,inVector[2],
						0,0,0,1);
	}
	static Matrix4D getRotationMatrix(const Vector4D& dir, const Vector4D up = Vector4D(0, 1, 0))
	{
		Vector4D xaxis = Vector4D::cross(up, dir);
		//xaxis.normalize();

		Vector4D yaxis = Vector4D::cross(dir, xaxis);
		//yaxis.normalize();

		float rot[16];

		rot[0] = xaxis[0]; rot[1] = xaxis[1]; rot[2] = xaxis[2]; rot[3] = 0;
		rot[4] = yaxis[0]; rot[5] = yaxis[1]; rot[6] = yaxis[2]; rot[7] = 0;
		rot[8] = dir[0];   rot[9] = dir[1];   rot[10] = dir[2];  rot[11] = 0;
		rot[12] = 0;       rot[13] = 0;       rot[14] = 0;       rot[15] = 1;

		return Matrix4D(rot);
	}
	static Matrix4D getRotationMatrixNew(const Vector4D dir)
	{
		float c1 = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);
		float s1 = dir[2];
		float c2 = c1 ? dir[0] / c1 : 1.0f;
		float s2 = c1 ? dir[1] / c1 : 0.0f;

		return Matrix4D(dir[0], -s2, -s1 * c2, 0,
						dir[1], c2, -s1 * s2, 0,
						dir[2], 0, c1, 0,
						0, 0, 0, 1);
	}
	static Matrix4D getScaleMatrix(const Vector4D& inVector)
	{
		return Matrix4D(inVector[0], 0, 0, 0,
						0, inVector[1], 0, 0,
						0, 0, inVector[2], 0,
						0, 0, 0,           1);
	}
	static Matrix4D lookAt(Vector4D cameraPosition, Vector4D cameraTarget, Vector4D cameraUp) 
	{
		Vector4D f = (cameraTarget - cameraPosition);
		Vector4D u = Vector4D::normalize(cameraUp);
		Vector4D s = Vector4D::normalize(Vector4D::cross(f,u));
		u = Vector4D::cross(s,f);

		Matrix4D lookAt = Matrix4D(s[0], s[1], s[2], -(Vector4D::dot(s, cameraPosition)),
			u[0], u[1], u[2], -(Vector4D::dot(u, cameraPosition)),
			-f[0], -f[1], -f[2], Vector4D::dot(f, cameraPosition),
			0, 0, 0, 1
		);

		/*Matrix4D lookAt = Matrix4D(s.getFloat(0), u.getFloat(0),  f.getFloat(0), 0,
									s.getFloat(1), u.getFloat(1), f.getFloat(1), 0,
									s.getFloat(2), u.getFloat(2), f.getFloat(2), 0,
									-(s.dotProduct(cameraPosition)), -(u.dotProduct(cameraPosition)), -(f.dotProduct(cameraPosition)), 1
		);*/

		return lookAt;
	}
	static Matrix4D perspective(float fieldOfView, float aspectRatio, float nearClip, float farClip)
	{
		Matrix4D result;

		result = Matrix4D(
			1.0f / (aspectRatio * tan(fieldOfView / 2)), 0, 0, 0,
			0, 1.0f / (tan(fieldOfView / 2)), 0, 0,
			0, 0, ((-nearClip - farClip) / (nearClip - farClip)), -1,
			0, 0, ((2 * farClip * nearClip) / (farClip - nearClip)), 0
		);

		return result;
	}


	/// Operators
	//====================================================================================================
	Matrix4D operator*(const float scalar) const
	{
		float rV[16];
		for (int i = 0; i < 16; i++)
		{
			rV[i] = matrix[i] * scalar;
		}
		return Matrix4D(rV);
	}
	Vector4D operator*(const Vector4D rhs) const
	{
		return Vector4D(matrix[0]  * rhs[0]	+ matrix[1]  * rhs[1] + matrix[2]  * rhs[2] + matrix[3]  * rhs[3],
						matrix[4]  * rhs[0]	+ matrix[5]  * rhs[1] + matrix[6]  * rhs[2] + matrix[7]  * rhs[3],
						matrix[8]  * rhs[0]	+ matrix[9]  * rhs[1] + matrix[10] * rhs[2] + matrix[11] * rhs[3],
						matrix[12] * rhs[0] + matrix[13] * rhs[1] + matrix[14] * rhs[2] + matrix[15] * rhs[3]);
	}
	Matrix4D operator*(const Matrix4D rhs) const
	{
		float returnMatrix[16];

		returnMatrix[0] = matrix[0] * rhs[0] + matrix[1] * rhs[4] + matrix[2] * rhs[8] + matrix[3] * rhs[12];
		returnMatrix[1] = matrix[0] * rhs[1] + matrix[1] * rhs[5] + matrix[2] * rhs[9] + matrix[3] * rhs[13];
		returnMatrix[2] = matrix[0] * rhs[2] + matrix[1] * rhs[6] + matrix[2] * rhs[10] + matrix[3] * rhs[14];
		returnMatrix[3] = matrix[0] * rhs[3] + matrix[1] * rhs[7] + matrix[2] * rhs[11] + matrix[3] * rhs[15];

		returnMatrix[4] = matrix[4] * rhs[0] + matrix[5] * rhs[4] + matrix[6] * rhs[8] + matrix[7] * rhs[12];
		returnMatrix[5] = matrix[4] * rhs[1] + matrix[5] * rhs[5] + matrix[6] * rhs[9] + matrix[7] * rhs[13];
		returnMatrix[6] = matrix[4] * rhs[2] + matrix[5] * rhs[6] + matrix[6] * rhs[10] + matrix[7] * rhs[14];
		returnMatrix[7] = matrix[4] * rhs[3] + matrix[5] * rhs[7] + matrix[6] * rhs[11] + matrix[7] * rhs[15];

		returnMatrix[8] = matrix[8] * rhs[0] + matrix[9] * rhs[4] + matrix[10] * rhs[8] + matrix[11] * rhs[12];
		returnMatrix[9] = matrix[8] * rhs[1] + matrix[9] * rhs[5] + matrix[10] * rhs[9] + matrix[11] * rhs[13];
		returnMatrix[10] = matrix[8] * rhs[2] + matrix[9] * rhs[6] + matrix[10] * rhs[10] + matrix[11] * rhs[14];
		returnMatrix[11] = matrix[8] * rhs[3] + matrix[9] * rhs[7] + matrix[10] * rhs[11] + matrix[11] * rhs[15];

		returnMatrix[12] = matrix[12] * rhs[0] + matrix[13] * rhs[4] + matrix[14] * rhs[8] + matrix[15] * rhs[12];
		returnMatrix[13] = matrix[12] * rhs[1] + matrix[13] * rhs[5] + matrix[14] * rhs[9] + matrix[15] * rhs[13];
		returnMatrix[14] = matrix[12] * rhs[2] + matrix[13] * rhs[6] + matrix[14] * rhs[10] + matrix[15] * rhs[14];
		returnMatrix[15] = matrix[12] * rhs[3] + matrix[13] * rhs[7] + matrix[14] * rhs[11] + matrix[15] * rhs[15];

		return Matrix4D(returnMatrix);
	}
	void operator+= (const Matrix4D rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] += rhs[i];
		}
	}
	Matrix4D operator+ (const Matrix4D& rhs) const
	{
		float rV[16];
		for (int i = 0; i < 16; i++)
		{
			rV[i] = matrix[i] + rhs[i];
		}
		return Matrix4D(rV);
	}
	Matrix4D operator- (const Matrix4D& rhs) const
	{
		float rV[16];
		for (int i = 0; i < 16; i++)
		{
			rV[i] = matrix[i] - rhs[i];
		}
		return Matrix4D(rV);
	}

	//Returns a refrence to the value of the x position in the Vector
	float& operator[] (const int x)			//Set
	{
		return matrix[x];
	}
	//Returns a constant to the value of the x position in the Vector
	float operator[] (const int x) const	//Get
	{
		return matrix[x];
	}

private:
	float matrix[16];
};
#pragma endregion