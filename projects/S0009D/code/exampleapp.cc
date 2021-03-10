//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <cstring>

#define PI 3.14159265

using namespace Display;
namespace Example
{
	Matrix4D view;
	Matrix4D lookAt;

	Vector4D* colorpick = new Vector4D();
	int selectedIndex;

	Vector4D cameraPos = Vector4D(0.0f, 0.0f, 5.0f, 1);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1);

	Vector4D position = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);

	bool click = false;
	bool isHeldDown = false;

	int windowSizeX;
	int windowSizeY;
	float fov = 90.0f;

	float lastX, lastY, yaw = -90.0f, pitch = 0.0f;

	float radianConversion = PI / 180;

	ExampleApp::ExampleApp()
	{
	}
	ExampleApp::~ExampleApp()
	{
	}

	bool ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;

		window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
			{
				if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
				{
					click = true;
				}
				else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
				{
					click = false;
					isHeldDown = false;
				}
				if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				{
					// Shoot ray
					int width, height;
					window->GetSize(width, height);
					Vector4D nds = Vector4D((mousePos[0] * 2.0f) / width - 1.0f, 1.0f - (2.0f * mousePos[1]) / height, 1, 1);
					Vector4D clip = Vector4D(nds[0], nds[1], -1.0f, 1.0f);
					Vector4D eye = Matrix4D::inverse(perspectiveProjection) * clip;
					eye = Vector4D(eye[0], eye[1], -1.0f, 0.0f);
					Vector4D world = Matrix4D::inverse(lookAt) * eye;
					world.normalize();

					Ray ray = Ray(cameraPos, world);
					rays.push_back(ray);
					for (int i = 0; i < planes.size(); i++)
					{
						if (ray.checkIfHitting(planes[i]))
						{
							colorpick = planes[i].getColor();
							selectedIndex = i;
						}
					}
				}
			});
		window->SetMouseMoveFunction([this](float64 xPos, float64 yPos)
			{
				mousePos.Set(xPos, yPos);
				if (click)
				{
					if (!isHeldDown)
					{
						lastX = xPos;
						lastY = yPos;
						isHeldDown = true;
					}

					float xOffset = xPos - lastX;
					float yOffset = lastY - yPos;

					lastX = xPos;
					lastY = yPos;

					float sensitivity = 0.2;
					xOffset *= sensitivity;
					yOffset *= sensitivity;

					yaw += xOffset;
					pitch += yOffset;

					if (pitch > 89.0f)
					{
						pitch = 89.0f;
					}
					if (pitch < -89.0f)
					{
						pitch = -89.0f;
					}

					Vector4D front;
					front[0] = cos(yaw * radianConversion) * cos(pitch * radianConversion);
					front[1] = sin(pitch * radianConversion);
					front[2] = sin(yaw * radianConversion) * cos(pitch * radianConversion);
					front[3] = 1;
					front.normalize();
					cameraFront = front;
				}
			});
		window->SetKeyPressFunction([this](int32 key, int32 i, int32 action, int32 modifier)
			{
				float speed = 0.05f;
				if (key == GLFW_KEY_W)
				{
					cameraPos = cameraPos + (cameraFront * speed);
				}
				if (key == GLFW_KEY_S)
				{
					cameraPos = cameraPos - (cameraFront * speed);
				}
				if (key == GLFW_KEY_D)
				{
					cameraPos = cameraPos + (Vector4D::normalize(Vector4D::cross(cameraFront, cameraUp)) * speed);
				}
				if (key == GLFW_KEY_A)
				{
					cameraPos = cameraPos - (Vector4D::normalize(Vector4D::cross(cameraFront, cameraUp)) * speed);
				}


				if (key == GLFW_KEY_ESCAPE)
					closeApp = true;
			});


		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glEnable(GL_DEPTH_TEST);

			//Perspective projection
			const float n = 0.1f;
			const float f = 100000.0f;
			const float l = -0.1f;
			const float r = 0.1f;
			const float t = 0.1f;
			const float b = -0.1f;

			perspectiveProjection = Matrix4D(
				(2 * n) / (r - l), 0, ((r + l) / (r - l)), 0,
				0, (2 * n) / (t - b), ((t + b) / (t - b)), 0,
				0, 0, -((f + n) / (f - n)), -((2 * f * n) / (f - n)),
				0, 0, -1, 0
			);

			planes.push_back(Plane(Vector4D(0, 0, -1), Vector2D(2, 2), Vector4D(0, 0, 1), Vector4D(1, 1, 4)));
			planes.push_back(Plane(Vector4D(0, 3, 0), Vector2D(4, 2), Vector4D(0, 0, 1), Vector4D(1, 1, 4)));
			planes.push_back(Plane(Vector4D(6, 0, -1), Vector2D(2, 2), Vector4D(0, 0, 1), Vector4D(1, 1, 4)));

			this->window->SetUiRender([this]()
				{
					this->RenderUI();
				});

			return true;
		}
		return false;
	}

	void ExampleApp::RenderUI()
	{
		if (this->window->IsOpen())
		{
			bool show = true;
			ImGui::SetNextWindowSize(ImVec2(200, 400));
			ImGui::Begin("test UI", &show);

			ImGui::Checkbox("render", DebugManager::getInstance().bRender());

			ImGui::Text("rays: %i", rays.size());
			ImGui::Text("planes: %i", planes.size());
			
			if (ImGui::Button("Clear"))
			{
				rays.clear();
				DebugManager::getInstance().clear();
			}
			ImGui::Text("selected index: %i", selectedIndex);

			if (ImGui::CollapsingHeader("Square"))
			{
				float* pos = planes[selectedIndex].getPosition().GetPointer();
				if (ImGui::DragFloat3("position", pos, 0.5f, -10.0f, 10.0f, "%.1f"))
				{
					planes[selectedIndex].setPosition(Vector4D(pos[0], pos[1], pos[2]));
				}
				float* col = planes[selectedIndex].getColor()->GetPointer();
				if (ImGui::ColorEdit4("selected square color", col))
				{
					planes[selectedIndex].setColor(Vector4D(col));
				}
			}



			ImGui::End();
		}
	}

	void ExampleApp::Run()
	{
		while (this->window->IsOpen() && true)
		{
			this->window->Update();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			view = perspectiveProjection * lookAt;

			DebugManager::getInstance().drawDebugShapes(view);

			this->window->SwapBuffers();
			if (closeApp)
			{
				this->window->Close();
			}
		}
	}

} // namespace Example