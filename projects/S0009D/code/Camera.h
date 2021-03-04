#pragma once
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "Matrix4D.h"

class Camera
{
public:

	Camera();
	~Camera();

	Matrix4D run();
	void setup();
	/// moves the camera.
	inline void moveCameraF() { cameraPos += (cameraFront * cameraSpeed); }
	inline void moveCameraB() { cameraPos -= (cameraFront * cameraSpeed); }
	inline void moveCameraL() { cameraPos -= Vector4D::normalize(Vector4D::cross(cameraFront, cameraUp)) * cameraSpeed; }
	inline void moveCameraR() { cameraPos += Vector4D::normalize(Vector4D::cross(cameraFront, cameraUp)) * cameraSpeed; }
	inline void moveCameraU() { cameraPos += (cameraUp * cameraSpeed); }
	inline void moveCameraD() { cameraPos -= (cameraUp * cameraSpeed); }
	/// changes the camera front to rotate the camera.
	inline void changeCameraFront(Vector4D vec)
	{
		cameraFront = vec;
	}
	inline void printCameraPos()
	{
		cameraPos.print();
	}
	inline Vector4D getCameraPos()
	{
		return cameraPos;
	}
	inline void getCamera()
	{
		printf("Camera pos = %f:%f:%f \nCamera dir = %f:%f:%f\n", cameraPos.getX(), cameraPos.getY(), cameraPos.getZ(), cameraFront.getX(), cameraFront.getY(), cameraFront.getZ());
	}

private:

	// Camera variables
	float radius = 2.0f;

	Vector4D cameraPos;				// Position of the camera in the world space.
	Vector4D cameraTarget;			// the point the camera looks at (used in setup).
	Vector4D cameraFront;			// A vector infront of the camera (used to make camera right and in turn camera up).
	Vector4D cameraUp;				// a vector that controlls the up direction of the camera.
	Vector4D cameraRight;			// a vector that controlls the right direction of the camera.
	float cameraSpeed = 0.5f;

	Vector4D up = Vector4D(0.0f, 1.0f, 0.0f, 1.0f); //WORLDSPACE UP

	Matrix4D view;

	Vector4D cameraDirection;

	// Perspectiv variables
	Matrix4D perspectiveProjection;
	float n = 0.1, f = 10000, t = 0.1, b = -0.1, r = 0.1, l = -0.1;
};

inline Camera::Camera()
{
}
inline Camera::~Camera()
{
}
/// setup the camera Perspectiv.
inline void Camera::setup()
{
	perspectiveProjection = Matrix4D
	(
		2 * n / (r - l), 0, 0, 0,
		0, 2 * n / (t - b), 0, 0,
		((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
		0, 0, -((2 * f*n) / (f - n)), 0
	);
	perspectiveProjection = Matrix4D::transpose(perspectiveProjection);
	
	//Camera setup
	cameraPos = Vector4D(0.0, 0.0, 5.0, 1.0);
	cameraFront = Vector4D(0.0, 0.0, -1.0, 1.0);
	up = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
	cameraUp = up;
	cameraTarget = (cameraPos + cameraFront);


}
/// calculate the view-matrix. returns the perspecivProjection Matrix * view Matrix. 
inline Matrix4D Camera::run()
{
	cameraTarget = (cameraPos + cameraFront);
	cameraDirection = Vector4D::normalize(cameraPos - cameraTarget);
	cameraRight = Vector4D::cross(up, cameraDirection);
	cameraUp = Vector4D::cross(cameraDirection, cameraRight);

	view = Matrix4D::lookAt(cameraPos, cameraTarget, cameraUp);
	return perspectiveProjection * view;

}