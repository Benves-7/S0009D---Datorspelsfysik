#pragma once
#include <math.h>
#include <iostream>
#include "Vector4D.h"

class Matrix4D
{
public:
	Matrix4D()
	{
		matrix[0] = 1.0f;
		matrix[1] = 0.0f;
		matrix[2] = 0.0f;
		matrix[3] = 0.0f;

		matrix[4] = 0;
		matrix[5] = 1;
		matrix[6] = 0;
		matrix[7] = 0;

		matrix[8] = 0;
		matrix[9] = 0;
		matrix[10] = 1;
		matrix[11] = 0;

		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;
	}
	Matrix4D(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
	{
		matrix[0] = a;
		matrix[1] = b;
		matrix[2] = c;
		matrix[3] = d;
		matrix[4] = e;
		matrix[5] = f;
		matrix[6] = g;
		matrix[7] = h;
		matrix[8] = i;
		matrix[9] = j;
		matrix[10] = k;
		matrix[11] = l;
		matrix[12] = m;
		matrix[13] = n;
		matrix[14] = o;
		matrix[15] = p;
	}
	Matrix4D(const Matrix4D& mat)
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = mat.matrix[i];
		}
	}
	Matrix4D(float mat[16])
	{
		matrix[0] = mat[0];
		matrix[1] = mat[1];
		matrix[2] = mat[2];
		matrix[3] = mat[3];
		matrix[4] = mat[4];
		matrix[5] = mat[5];
		matrix[6] = mat[6];
		matrix[7] = mat[7];
		matrix[8] = mat[8];
		matrix[9] = mat[9];
		matrix[10] = mat[10];
		matrix[11] = mat[11];
		matrix[12] = mat[12];
		matrix[13] = mat[13];
		matrix[14] = mat[14];
		matrix[15] = mat[15];
	}
	~Matrix4D() {}
	float operator[](const int index) const { return matrix[index]; }
	float& operator[](const int index) { return matrix[index]; }
	Matrix4D operator*(const Matrix4D& rhs) const
	{
		float returnMatrix[16];

		returnMatrix[0]  = (matrix[0]  * rhs.matrix[0]) + (matrix[1]  * rhs.matrix[4]) + (matrix[2]  * rhs.matrix[8])  + (matrix[3]  * rhs.matrix[12]);
		returnMatrix[1]  = (matrix[0]  * rhs.matrix[1]) + (matrix[1]  * rhs.matrix[5]) + (matrix[2]  * rhs.matrix[9])  + (matrix[3]  * rhs.matrix[13]);
		returnMatrix[2]  = (matrix[0]  * rhs.matrix[2]) + (matrix[1]  * rhs.matrix[6]) + (matrix[2]  * rhs.matrix[10]) + (matrix[3]  * rhs.matrix[14]);
		returnMatrix[3]  = (matrix[0]  * rhs.matrix[3]) + (matrix[1]  * rhs.matrix[7]) + (matrix[2]  * rhs.matrix[11]) + (matrix[3]  * rhs.matrix[15]);
									   								  								 								 
		returnMatrix[4]  = (matrix[4]  * rhs.matrix[0]) + (matrix[5]  * rhs.matrix[4]) + (matrix[6]  * rhs.matrix[8])  + (matrix[7]  * rhs.matrix[12]);
		returnMatrix[5]  = (matrix[4]  * rhs.matrix[1]) + (matrix[5]  * rhs.matrix[5]) + (matrix[6]  * rhs.matrix[9])  + (matrix[7]  * rhs.matrix[13]);
		returnMatrix[6]  = (matrix[4]  * rhs.matrix[2]) + (matrix[5]  * rhs.matrix[6]) + (matrix[6]  * rhs.matrix[10]) + (matrix[7]  * rhs.matrix[14]);
		returnMatrix[7]  = (matrix[4]  * rhs.matrix[3]) + (matrix[5]  * rhs.matrix[7]) + (matrix[6]  * rhs.matrix[11]) + (matrix[7]  * rhs.matrix[15]);
									   								  
		returnMatrix[8]  = (matrix[8]  * rhs.matrix[0]) + (matrix[9]  * rhs.matrix[4]) + (matrix[10] * rhs.matrix[8])  + (matrix[11] * rhs.matrix[12]);
		returnMatrix[9]  = (matrix[8]  * rhs.matrix[1]) + (matrix[9]  * rhs.matrix[5]) + (matrix[10] * rhs.matrix[9])  + (matrix[11] * rhs.matrix[13]);
		returnMatrix[10] = (matrix[8]  * rhs.matrix[2]) + (matrix[9]  * rhs.matrix[6]) + (matrix[10] * rhs.matrix[10]) + (matrix[11] * rhs.matrix[14]);
		returnMatrix[11] = (matrix[8]  * rhs.matrix[3]) + (matrix[9]  * rhs.matrix[7]) + (matrix[10] * rhs.matrix[11]) + (matrix[11] * rhs.matrix[15]);

		returnMatrix[12] = (matrix[12] * rhs.matrix[0]) + (matrix[13] * rhs.matrix[4]) + (matrix[14] * rhs.matrix[8])  + (matrix[15] * rhs.matrix[12]);
		returnMatrix[13] = (matrix[12] * rhs.matrix[1]) + (matrix[13] * rhs.matrix[5]) + (matrix[14] * rhs.matrix[9])  + (matrix[15] * rhs.matrix[13]);
		returnMatrix[14] = (matrix[12] * rhs.matrix[2]) + (matrix[13] * rhs.matrix[6]) + (matrix[14] * rhs.matrix[10]) + (matrix[15] * rhs.matrix[14]);
		returnMatrix[15] = (matrix[12] * rhs.matrix[3]) + (matrix[13] * rhs.matrix[7]) + (matrix[14] * rhs.matrix[11]) + (matrix[15] * rhs.matrix[15]);

		return Matrix4D(returnMatrix);
	}
	Vector4D operator*(const Vector4D& rhs) const
	{
		return(Vector4D(
			rhs[0] * matrix[0] + rhs[1] * matrix[1] + rhs[2] * matrix[2] + rhs[3] * matrix[3],
			rhs[0] * matrix[4] + rhs[1] * matrix[5] + rhs[2] * matrix[6] + rhs[3] * matrix[7],
			rhs[0] * matrix[8] + rhs[1] * matrix[9] + rhs[2] * matrix[10] + rhs[3] * matrix[11],
			rhs[0] * matrix[12] + rhs[1] * matrix[13] + rhs[2] * matrix[14] + rhs[3] * matrix[15]));
	}
	Matrix4D operator+(const Matrix4D& rhs) const
	{
		float returnVector[16];
		/// adds all the corresponding elements in both matricies to eachother
		returnVector[0] = matrix[0] + rhs.matrix[0];
		returnVector[1] = matrix[1] + rhs.matrix[1];
		returnVector[2] = matrix[2] + rhs.matrix[2];
		returnVector[3] = matrix[3] + rhs.matrix[3];
		returnVector[4] = matrix[4] + rhs.matrix[4];
		returnVector[5] = matrix[5] + rhs.matrix[5];
		returnVector[6] = matrix[6] + rhs.matrix[6];
		returnVector[7] = matrix[7] + rhs.matrix[7];
		returnVector[8] = matrix[8] + rhs.matrix[8];
		returnVector[9] = matrix[9] + rhs.matrix[9];
		returnVector[10] = matrix[10] + rhs.matrix[10];
		returnVector[11] = matrix[11] + rhs.matrix[11];
		returnVector[12] = matrix[12] + rhs.matrix[12];
		returnVector[13] = matrix[13] + rhs.matrix[13];
		returnVector[14] = matrix[14] + rhs.matrix[14];
		returnVector[15] = matrix[15] + rhs.matrix[15];

		return Matrix4D(returnVector);
	}
	Matrix4D operator*(const float scalar) const
	{
		float returnVector[16];
		returnVector[0] = matrix[0] * scalar;
		returnVector[1] = matrix[1] * scalar;
		returnVector[2] = matrix[2] * scalar;
		returnVector[3] = matrix[3] * scalar;
		returnVector[4] = matrix[4] * scalar;
		returnVector[5] = matrix[5] * scalar;
		returnVector[6] = matrix[6] * scalar;
		returnVector[7] = matrix[7] * scalar;
		returnVector[8] = matrix[8] * scalar;
		returnVector[9] = matrix[9] * scalar;
		returnVector[10] = matrix[10] * scalar;
		returnVector[11] = matrix[11] * scalar;
		returnVector[12] = matrix[12] * scalar;
		returnVector[13] = matrix[13] * scalar;
		returnVector[14] = matrix[14] * scalar;
		returnVector[15] = matrix[15] * scalar;

		return Matrix4D(returnVector);
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
	static Matrix4D perspective(float n, float f, float right, float left, float top, float bot)
	{
		Matrix4D result(
			(2 * n) / (right - left), 0, (right + left) / (right - left), 0,
			0, (2 * n) / (top - bot), (top + bot) / (top - bot), 0,
			0, 0, -((f + n) / (f - n)), -((2 * f * n) / (f - n)),
			0, 0, -1, 0
		);
		return result;
	}
	Vector4D getPositionVec()
	{
		return Vector4D(matrix[3], matrix[7], matrix[11], matrix[15]);
	}
	float returnFirst() 
	{ 
		return matrix[0]; 
	}
	void operator=(const Matrix4D& rhs)
	{
		matrix[0] = rhs.matrix[0];
		matrix[1] = rhs.matrix[1];
		matrix[2] = rhs.matrix[2];
		matrix[3] = rhs.matrix[3];

		matrix[4] = rhs.matrix[4];
		matrix[5] = rhs.matrix[5];
		matrix[6] = rhs.matrix[6];
		matrix[7] = rhs.matrix[7];

		matrix[8] = rhs.matrix[8];
		matrix[9] = rhs.matrix[9];
		matrix[10] = rhs.matrix[10];
		matrix[11] = rhs.matrix[11];

		matrix[12] = rhs.matrix[12];
		matrix[13] = rhs.matrix[13];
		matrix[14] = rhs.matrix[14];
		matrix[15] = rhs.matrix[15];
	}
	static Matrix4D rotX(float angle)
	{
		const float cosAng = cos(angle);
		const float sinAng = sin(angle);
		float returnMatrix[16];

		returnMatrix[0] = 1;
		returnMatrix[1] = 0;
		returnMatrix[2] = 0;
		returnMatrix[3] = 0;

		returnMatrix[4] = 0;
		returnMatrix[5] = cosAng;
		returnMatrix[6] = -sinAng;
		returnMatrix[7] = 0;

		returnMatrix[8] = 0;
		returnMatrix[9] = sinAng;
		returnMatrix[10] = cosAng;
		returnMatrix[11] = 0;

		returnMatrix[12] = 0;
		returnMatrix[13] = 0;
		returnMatrix[14] = 0;
		returnMatrix[15] = 1;

		return Matrix4D(returnMatrix);
	}
	static Matrix4D rotY(float angle)
	{
		const float cosAng = cos(angle);
		const float sinAng = sin(angle);
		float returnMatrix[16];

		returnMatrix[0] = cosAng;
		returnMatrix[1] = 0;
		returnMatrix[2] = sinAng;
		returnMatrix[3] = 0;

		returnMatrix[4] = 0;
		returnMatrix[5] = 1;
		returnMatrix[6] = 0;
		returnMatrix[7] = 0;

		returnMatrix[8] = -sinAng;
		returnMatrix[9] = 0;
		returnMatrix[10] = cosAng;
		returnMatrix[11] = 0;

		returnMatrix[12] = 0;
		returnMatrix[13] = 0;
		returnMatrix[14] = 0;
		returnMatrix[15] = 1;

		return Matrix4D(returnMatrix);
	}
	static Matrix4D rotZ(float angle)
	{
		const float cosAng = cos(angle);
		const float sinAng = sin(angle);
		float returnMatrix[16];

		returnMatrix[0] = cosAng;
		returnMatrix[1] = -sinAng;
		returnMatrix[2] = 0;
		returnMatrix[3] = 0;

		returnMatrix[4] = sinAng;
		returnMatrix[5] = cosAng;
		returnMatrix[6] = 0;
		returnMatrix[7] = 0;

		returnMatrix[8] = 0;
		returnMatrix[9] = 0;
		returnMatrix[10] = 1;
		returnMatrix[11] = 0;

		returnMatrix[12] = 0;
		returnMatrix[13] = 0;
		returnMatrix[14] = 0;
		returnMatrix[15] = 1;

		return Matrix4D(returnMatrix);
	}
	static Matrix4D rotVec(Vector4D& vec, const float angle)
	{
		Vector4D normVec = Vector4D::normalize(vec);

		Matrix4D matC(0, -normVec[2], normVec[1], 0,
			normVec[2], 0, -normVec[0], 0,
			-normVec[1], normVec[0], 0, 0,
			0, 0, 0, 1);

		Matrix4D I;
		Matrix4D rotMat = I + (matC * sin(angle)) + (matC * matC) * (1 - cos(angle));
		rotMat.matrix[15] = 1;

		return rotMat;
	}
	static Matrix4D getPositionMatrix(Vector4D& inVector)
	{
		float tempArr[16];
		tempArr[0] = 1;
		tempArr[1] = 0;
		tempArr[2] = 0;
		tempArr[3] = inVector[0];


		tempArr[4] = 0;
		tempArr[5] = 1;
		tempArr[6] = 0;
		tempArr[7] = inVector[1];

		tempArr[8] = 0;
		tempArr[9] = 0;
		tempArr[10] = 1;
		tempArr[11] = inVector[2];

		tempArr[12] = 0;
		tempArr[13] = 0;
		tempArr[14] = 0;
		tempArr[15] = 1;
		return Matrix4D(tempArr);
	}
	static Matrix4D getRotationFromQuaternian(Vector4D& vec)
	{
		float xx = vec[0] * vec[0];
		float xy = vec[0] * vec[1];
		float xz = vec[0] * vec[2];
		float xw = vec[0] * vec[3];

		float yy = vec[1] * vec[1];
		float yz = vec[1] * vec[2];
		float yw = vec[1] * vec[3];

		float zz = vec[2] * vec[2];
		float zw = vec[2] * vec[3];

		return Matrix4D(1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0,
			2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0,
			2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0,
			0, 0, 0, 1);
	}
	static Matrix4D getScaleMatrix(Vector4D& vec)
	{
		float C[16] = { vec[0], 0, 0, 0,
				0, vec[1] , 0, 0,
				0, 0, vec[2], 0,
				0, 0, 0, 1 };
		return Matrix4D(C);
	}
	Vector4D getPositionVector() 
	{
		return Vector4D(matrix[3], matrix[7], matrix[11], matrix[15]); 
	}
	static Matrix4D transpose(const Matrix4D& mat)
	{
		float returnMatrix[16];
		returnMatrix[0] = mat.matrix[0];
		returnMatrix[4] = mat.matrix[1];
		returnMatrix[8] = mat.matrix[2];
		returnMatrix[12] = mat.matrix[3];

		returnMatrix[1] = mat.matrix[4];
		returnMatrix[5] = mat.matrix[5];
		returnMatrix[9] = mat.matrix[6];
		returnMatrix[13] = mat.matrix[7];

		returnMatrix[2] = mat.matrix[8];
		returnMatrix[6] = mat.matrix[9];
		returnMatrix[10] = mat.matrix[10];
		returnMatrix[14] = mat.matrix[11];

		returnMatrix[3] = mat.matrix[12];
		returnMatrix[7] = mat.matrix[13];
		returnMatrix[11] = mat.matrix[14];
		returnMatrix[15] = mat.matrix[15];

		return Matrix4D(returnMatrix);
	}
	static Matrix4D inverse(const Matrix4D& mat)
	{
		float inv[16], determinant, out[16];

		inv[0] = mat.matrix[5] * mat.matrix[10] * mat.matrix[15] -
			mat.matrix[5] * mat.matrix[11] * mat.matrix[14] -
			mat.matrix[9] * mat.matrix[6] * mat.matrix[15] +
			mat.matrix[9] * mat.matrix[7] * mat.matrix[14] +
			mat.matrix[13] * mat.matrix[6] * mat.matrix[11] -
			mat.matrix[13] * mat.matrix[7] * mat.matrix[10];

		inv[4] = -mat.matrix[4] * mat.matrix[10] * mat.matrix[15] +
			mat.matrix[4] * mat.matrix[11] * mat.matrix[14] +
			mat.matrix[8] * mat.matrix[6] * mat.matrix[15] -
			mat.matrix[8] * mat.matrix[7] * mat.matrix[14] -
			mat.matrix[12] * mat.matrix[6] * mat.matrix[11] +
			mat.matrix[12] * mat.matrix[7] * mat.matrix[10];

		inv[8] = mat.matrix[4] * mat.matrix[9] * mat.matrix[15] -
			mat.matrix[4] * mat.matrix[11] * mat.matrix[13] -
			mat.matrix[8] * mat.matrix[5] * mat.matrix[15] +
			mat.matrix[8] * mat.matrix[7] * mat.matrix[13] +
			mat.matrix[12] * mat.matrix[5] * mat.matrix[11] -
			mat.matrix[12] * mat.matrix[7] * mat.matrix[9];

		inv[12] = -mat.matrix[4] * mat.matrix[9] * mat.matrix[14] +
			mat.matrix[4] * mat.matrix[10] * mat.matrix[13] +
			mat.matrix[8] * mat.matrix[5] * mat.matrix[14] -
			mat.matrix[8] * mat.matrix[6] * mat.matrix[13] -
			mat.matrix[12] * mat.matrix[5] * mat.matrix[10] +
			mat.matrix[12] * mat.matrix[6] * mat.matrix[9];

		inv[1] = -mat.matrix[1] * mat.matrix[10] * mat.matrix[15] +
			mat.matrix[1] * mat.matrix[11] * mat.matrix[14] +
			mat.matrix[9] * mat.matrix[2] * mat.matrix[15] -
			mat.matrix[9] * mat.matrix[3] * mat.matrix[14] -
			mat.matrix[13] * mat.matrix[2] * mat.matrix[11] +
			mat.matrix[13] * mat.matrix[3] * mat.matrix[10];

		inv[5] = mat.matrix[0] * mat.matrix[10] * mat.matrix[15] -
			mat.matrix[0] * mat.matrix[11] * mat.matrix[14] -
			mat.matrix[8] * mat.matrix[2] * mat.matrix[15] +
			mat.matrix[8] * mat.matrix[3] * mat.matrix[14] +
			mat.matrix[12] * mat.matrix[2] * mat.matrix[11] -
			mat.matrix[12] * mat.matrix[3] * mat.matrix[10];

		inv[9] = -mat.matrix[0] * mat.matrix[9] * mat.matrix[15] +
			mat.matrix[0] * mat.matrix[11] * mat.matrix[13] +
			mat.matrix[8] * mat.matrix[1] * mat.matrix[15] -
			mat.matrix[8] * mat.matrix[3] * mat.matrix[13] -
			mat.matrix[12] * mat.matrix[1] * mat.matrix[11] +
			mat.matrix[12] * mat.matrix[3] * mat.matrix[9];

		inv[13] = mat.matrix[0] * mat.matrix[9] * mat.matrix[14] -
			mat.matrix[0] * mat.matrix[10] * mat.matrix[13] -
			mat.matrix[8] * mat.matrix[1] * mat.matrix[14] +
			mat.matrix[8] * mat.matrix[2] * mat.matrix[13] +
			mat.matrix[12] * mat.matrix[1] * mat.matrix[10] -
			mat.matrix[12] * mat.matrix[2] * mat.matrix[9];

		inv[2] = mat.matrix[1] * mat.matrix[6] * mat.matrix[15] -
			mat.matrix[1] * mat.matrix[7] * mat.matrix[14] -
			mat.matrix[5] * mat.matrix[2] * mat.matrix[15] +
			mat.matrix[5] * mat.matrix[3] * mat.matrix[14] +
			mat.matrix[13] * mat.matrix[2] * mat.matrix[7] -
			mat.matrix[13] * mat.matrix[3] * mat.matrix[6];

		inv[6] = -mat.matrix[0] * mat.matrix[6] * mat.matrix[15] +
			mat.matrix[0] * mat.matrix[7] * mat.matrix[14] +
			mat.matrix[4] * mat.matrix[2] * mat.matrix[15] -
			mat.matrix[4] * mat.matrix[3] * mat.matrix[14] -
			mat.matrix[12] * mat.matrix[2] * mat.matrix[7] +
			mat.matrix[12] * mat.matrix[3] * mat.matrix[6];

		inv[10] = mat.matrix[0] * mat.matrix[5] * mat.matrix[15] -
			mat.matrix[0] * mat.matrix[7] * mat.matrix[13] -
			mat.matrix[4] * mat.matrix[1] * mat.matrix[15] +
			mat.matrix[4] * mat.matrix[3] * mat.matrix[13] +
			mat.matrix[12] * mat.matrix[1] * mat.matrix[7] -
			mat.matrix[12] * mat.matrix[3] * mat.matrix[5];

		inv[14] = -mat.matrix[0] * mat.matrix[5] * mat.matrix[14] +
			mat.matrix[0] * mat.matrix[6] * mat.matrix[13] +
			mat.matrix[4] * mat.matrix[1] * mat.matrix[14] -
			mat.matrix[4] * mat.matrix[2] * mat.matrix[13] -
			mat.matrix[12] * mat.matrix[1] * mat.matrix[6] +
			mat.matrix[12] * mat.matrix[2] * mat.matrix[5];

		inv[3] = -mat.matrix[1] * mat.matrix[6] * mat.matrix[11] +
			mat.matrix[1] * mat.matrix[7] * mat.matrix[10] +
			mat.matrix[5] * mat.matrix[2] * mat.matrix[11] -
			mat.matrix[5] * mat.matrix[3] * mat.matrix[10] -
			mat.matrix[9] * mat.matrix[2] * mat.matrix[7] +
			mat.matrix[9] * mat.matrix[3] * mat.matrix[6];

		inv[7] = mat.matrix[0] * mat.matrix[6] * mat.matrix[11] -
			mat.matrix[0] * mat.matrix[7] * mat.matrix[10] -
			mat.matrix[4] * mat.matrix[2] * mat.matrix[11] +
			mat.matrix[4] * mat.matrix[3] * mat.matrix[10] +
			mat.matrix[8] * mat.matrix[2] * mat.matrix[7] -
			mat.matrix[8] * mat.matrix[3] * mat.matrix[6];

		inv[11] = -mat.matrix[0] * mat.matrix[5] * mat.matrix[11] +
			mat.matrix[0] * mat.matrix[7] * mat.matrix[9] +
			mat.matrix[4] * mat.matrix[1] * mat.matrix[11] -
			mat.matrix[4] * mat.matrix[3] * mat.matrix[9] -
			mat.matrix[8] * mat.matrix[1] * mat.matrix[7] +
			mat.matrix[8] * mat.matrix[3] * mat.matrix[5];

		inv[15] = mat.matrix[0] * mat.matrix[5] * mat.matrix[10] -
			mat.matrix[0] * mat.matrix[6] * mat.matrix[9] -
			mat.matrix[4] * mat.matrix[1] * mat.matrix[10] +
			mat.matrix[4] * mat.matrix[2] * mat.matrix[9] +
			mat.matrix[8] * mat.matrix[1] * mat.matrix[6] -
			mat.matrix[8] * mat.matrix[2] * mat.matrix[5];

		/// Calculate the determinant
		determinant = mat.matrix[0] * inv[0] + mat.matrix[1] * inv[4] + mat.matrix[2] * inv[8] + mat.matrix[3] * inv[12];
		/// If the determinant is zero the matrix is not inversible therefor the function returns the same matrix again


		/// Calculate once to save 16 calculations below
		determinant = 1.0 / determinant;

		/// Divide each element with the determinant
		out[0] = inv[0] * determinant;
		out[1] = inv[1] * determinant;
		out[2] = inv[2] * determinant;
		out[3] = inv[3] * determinant;
		out[4] = inv[4] * determinant;
		out[5] = inv[5] * determinant;
		out[6] = inv[6] * determinant;
		out[7] = inv[7] * determinant;
		out[8] = inv[8] * determinant;
		out[9] = inv[9] * determinant;
		out[10] = inv[10] * determinant;
		out[11] = inv[11] * determinant;
		out[12] = inv[12] * determinant;
		out[13] = inv[13] * determinant;
		out[14] = inv[14] * determinant;
		out[15] = inv[15] * determinant;


		return Matrix4D(out);
	}
	float* getPointer()
	{
		return matrix;
	}
	static Matrix4D lookAt(Vector4D position, Vector4D target, Vector4D up)
	{
		Vector4D f = (target - position);
		Vector4D u = Vector4D::normalize(up);
		Vector4D s = Vector4D::normalize(Vector4D::cross(f, u));
		u = Vector4D::cross(s, f);

		float temp[] = { s[0],  s[1],  s[2], -(Vector4D::dot(s, position)),
						 u[0],  u[1],  u[2], -(Vector4D::dot(u, position)),
						-f[0], -f[1], -f[2],   Vector4D::dot(f, position),
						    0,     0,     0,    1 };
		return Matrix4D(temp);
	}
	void print() 
	{
		std::cout <<
			matrix[0] << " : " << matrix[1] << " : " << matrix[2] << " : " << matrix[3] << "\n" <<
			matrix[4] << " : " << matrix[5] << " : " << matrix[6] << " : " << matrix[7] << "\n" <<
			matrix[8] << " : " << matrix[9] << " : " << matrix[10] << " : " << matrix[11] << "\n" <<
			matrix[12] << " : " << matrix[13] << " : " << matrix[14] << " : " << matrix[15] << std::endl;
	}
	void printPosition() 
	{
		std::cout <<
			matrix[3] << "\n" <<
			matrix[7] << "\n" <<
			matrix[11] << "\n" <<
			matrix[15] << std::endl;
	}

private:
	float matrix[16];
};
