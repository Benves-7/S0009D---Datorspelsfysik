#pragma once
#include "Skeleton.h"
#include "Animation.h"
#include "Graphics.h"
#include "Cubesphere.h"
#include <GL/GLU.h>

#define PI 3.14159265

class Footman
{
public:
	Footman() {};
	~Footman() {};

	void load(Matrix4D projectionMatrix, Matrix4D* viewMatrix)
	{

		// Skeleton
		skeleton.loadSkeleton("Unit_Footman.constants");
		// Animation
		animation.loadAnimations("Unit_Footman.nax3");
		// Graphics
		graphics.loadMesh("Unit_Footman.nvx2");
	}
	void update(float runtime)
	{
		float animationSpeed = runtime / animation.clips[clipToPlay].keyDuration;

		if (runAnimation)
		{
			for (int i = 0; i < skeleton.joints->size(); ++i)
			{
				//Load animation data for one key in a clip
				Vector4D pos = animation.getKey(clipToPlay, animationSpeed, i * 4, 0);
				Matrix4D po = Matrix4D::getPositionMatrix(pos);
				Vector4D rot = animation.getKey(clipToPlay, animationSpeed, i * 4 + 1, 1);
				Matrix4D ro = Matrix4D::getRotationFromQuaternian(rot);
				Vector4D scale = animation.getKey(clipToPlay, animationSpeed, i * 4 + 2, 0);
				Matrix4D sc = Matrix4D::getScaleMatrix(scale);
				Vector4D vel = animation.getKey(clipToPlay, animationSpeed, i * 4 + 3, 0);
				Matrix4D res = (po * ro) * sc;
				skeleton.joints->at(i).localTransform = res;
			}
		}
		skeleton.update(0);
	}
	void draw()
	{
		for (int i = 0; i < skeleton.joints->size(); ++i)
		{
			Joint joint = skeleton.joints->at(i);
			Vector4D a = joint.worldspaceTransform.getPositionVector();
			glBegin(GL_LINES);
			if (drawSkeleton)
			{
				if (joint.parent != -1)
				{
					glColor3f(255, 0, 0);
					Vector4D b = skeleton.joints->at(joint.parent).worldspaceTransform.getPositionVector();
					glVertex3f(a[0], a[1], a[2]);
					glVertex3f(b[0], b[1], b[2]);
				}
			}
			if (drawBalls)
			{

				glColor3f(0, 255, 0);
				int Stacks = 20;
				int Slices = 20;
				float radius = 0.03f;
				// Calc The Vertices
				for (int i = 0; i <= Stacks; ++i) {

					float V = i / (float)Stacks;
					float phi = V * PI;

					// Loop Through Slices
					for (int j = 0; j <= Slices; ++j) {

						float U = j / (float)Slices;
						float theta = U * (PI * 2);

						// Calc The Vertex Positions
						float x = a[0] + (cosf(theta) * sinf(phi) * radius);
						float y = a[1] + (cosf(phi) * radius);
						float z = a[2] + (sinf(theta) * sinf(phi) * radius);
						glVertex3f(x, y, z);
					}
				}
			}
			glEnd();
		}
		if (drawMesh)
		{
			graphics.setupMesh();
			graphics.draw();
			graphics.unbindBuffers();
		}
	}

	void ds()
	{
		drawSkeleton = !drawSkeleton;
	}
	void db()
	{
		drawBalls = !drawBalls;
	}
	void dm()
	{
		drawMesh = !drawMesh;
	}
	void ra()
	{
		runAnimation = !runAnimation;
	}

	void setAnimationClip(int i)
	{
		clipToPlay = i;
	}

private:
	Skeleton skeleton;
	Animation animation;
	Graphics graphics;

	unsigned int clipToPlay = 0;

	bool drawSkeleton = true;
	bool drawBalls = true;
	bool drawMesh = false;
	bool runAnimation = true;

};