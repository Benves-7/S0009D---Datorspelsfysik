#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "Camera.h"
#include "debug/debugManager.h"

#include "Ray.h"
#include "Plane.h"

#include <chrono>
#include <vector>


namespace Example
{
	class ExampleApp : public Core::App
	{
	public:

		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		/// open app
		bool Open();
		/// run app
		void Run();

	private:

		/// UI
		void RenderUI();

		Vector2D mousePos;

		bool closeApp = false;

		std::vector<DebugBase*> shapes;

		std::chrono::high_resolution_clock clock = std::chrono::high_resolution_clock();
		using ms = std::chrono::duration<float, std::milli>;

		Display::Window* window;

		Camera camera;

		bool useImGui = false;
		int test = 0;

		Matrix4D perspectiveProjection;

		vector<Plane> planes;
		vector<Ray> rays;
	};
}