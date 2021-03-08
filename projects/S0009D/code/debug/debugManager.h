#pragma once

#include <vector>
#include "debugShapes.h"

class DebugManager
{
public:
	//static DebugManager* getInstance()
	//{
	//	// Singelton generator.
	//	if (!instance)
	//	{
	//		instance = new DebugManager;
	//	}
	//	return instance;
	//}
	//~DebugManager()
	//{
	//}
	//void drawDebugShapes()
	//{
	//	for (int i = 0; i < debugShapes.size(); i++)
	//	{
	//		debugShapes[i]->draw(viewMatrix);
	//	}
	//}
	//void setViewMatrix(Matrix4D view)
	//{
	//	viewMatrix = view;
	//}
	//void createLine(Vector4D p1, Vector4D p2, Vector4D color = Vector4D(255, 0, 0, 1))
	//{
	//	addShape(new DebugLine(p1, p2, color));
	//}
	//void createSphere()
	//{
	//	// addShape(new DebugSphere(Vector4D center, float r, Vector4D color)) ? might look like this...
	//}
	//void clear()
	//{
	//	debugShapes.clear();
	//}

	DebugManager()
	{
		render = true;
	}
	~DebugManager()
	{

	}
	void drawDebugShapes()
	{
		if (!render)
			return;
		for (int i = 0; i < debugShapes.size(); i++)
		{
			debugShapes[i]->draw(viewMatrix);
		}
		for (int i = 0; i < savedShapes.size(); i++)
		{
			savedShapes[i]->draw(viewMatrix);
		}
	}
	void setViewMatrix(Matrix4D view)
	{
		viewMatrix = view;
	}
	void createLine(Vector4D p1, Vector4D p2, Vector4D color = Vector4D(255, 0, 0, 1))
	{
		addShape(new DebugLine(p1, p2, color));
	}
	void createSquare(Vector4D pos, Vector2D dim, Vector4D color = Vector4D(20, 10, 40, 1))
	{
		addSafeShape(new DebugSquare(pos, dim, color));
	}
	void createCube(Vector4D pos, Vector4D dim, Vector4D color = Vector4D(10, 30, 30, 1))
	{
		addSafeShape(new DebugCube(pos, dim, color));
	}


	void addShape(DebugBase* shape)
	{
		debugShapes.push_back(std::shared_ptr<DebugBase>(shape));
	}
	void addSafeShape(DebugBase* shape)
	{
		savedShapes.push_back(std::shared_ptr<DebugBase>(shape));
	}

	void clear()
	{
		debugShapes.clear();
	}

	bool* bRender()
	{
		return &render;
	}

	std::vector<std::shared_ptr<DebugBase>> debugShapes;
	std::vector<std::shared_ptr<DebugBase>> savedShapes;

private:
	Matrix4D viewMatrix;
	bool render;
};