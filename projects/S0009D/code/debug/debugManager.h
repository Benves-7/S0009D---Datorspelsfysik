#pragma once
#include "debugShapes.h"
#include <vector>

class DebugManager
{
public:
	static DebugManager& getInstance()
	{
		static DebugManager instance;
		return instance;
	}

	~DebugManager()
	{

	}
	void drawDebugShapes(Matrix4D viewMatrix)
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

	//void setViewMatrix(Matrix4D view)
	//{
	//	viewMatrix = view;
	//}
	//void createLine(Vector4D p1, Vector4D p2, Vector4D color = Vector4D(255, 0, 0))
	//{
	//	addShape(new DebugLine(p1, p2, color));
	//}
	//void createSquare(Vector4D pos, Vector2D dim, Vector4D color = Vector4D(20, 10, 40))
	//{
	//	addSafeShape(new DebugSquare(pos, dim, color));
	//}
	//void createCube(Vector4D pos, Vector4D dim, Vector4D color = Vector4D(10, 30, 30))
	//{
	//	addSafeShape(new DebugCube(pos, dim, color));
	//}
	//void createSphere(Vector4D pos, float radius, Vector4D color = Vector4D(50, 20, 20))
	//{
	//	addSafeShape(new DebugSphere(pos, radius, color));
	//}

	int addShape(DebugBase* shape)
	{
		debugShapes.push_back(std::shared_ptr<DebugBase>(shape));
		return debugShapes.size() - 1;
	}
	int addSafeShape(DebugBase* shape)
	{
		savedShapes.push_back(std::shared_ptr<DebugBase>(shape));
		return savedShapes.size() - 1;
	}

	shared_ptr<DebugBase> getSavedShape(int index)
	{
		return savedShapes[index];
	}
	shared_ptr<DebugBase> getDebugShape(int index)
	{
		return debugShapes[index];
	}


	void clear()
	{
		debugShapes.clear();
	}

	bool* bRender()
	{
		return &render;
	}

private:

	DebugManager()
	{
		render = true;
	}

	std::vector<std::shared_ptr<DebugBase>> debugShapes;
	std::vector<std::shared_ptr<DebugBase>> savedShapes;
	bool render;
};