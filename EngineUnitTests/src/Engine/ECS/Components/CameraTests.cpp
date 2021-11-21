#include "pch.h"

#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/Camera.cpp"

#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine::Components;


namespace CameraTests
{
	TEST_CLASS(OrthographicCameraTests)
	{
	public:
		TEST_METHOD(ViewToWorldTest)
		{
			OrthographicCamera camera(-2.0, 2.0, -1.5f, 1.5f, -5.0f, 5.0f);
			
			std::vector<Engine::Vec4> viewPoints = {
				{-1.0f, -1.0f, -1.0f, 1.0f },
				{ 1.0f, -1.0f, -1.0f, 1.0f },
				{ 1.0f,  1.0f, -1.0f, 1.0f },
				{-1.0f,  1.0f, -1.0f, 1.0f },
				{-1.0f, -1.0f,  1.0f, 1.0f },
				{ 1.0f, -1.0f,  1.0f, 1.0f },
				{ 1.0f,  1.0f,  1.0f, 1.0f },
				{-1.0f,  1.0f,  1.0f, 1.0f },
			};

			std::vector<Engine::Vec4> worldPoints = {
				{-2.0f, -1.5f,  5.0f, 1.0f },
				{ 2.0f, -1.5f,  5.0f, 1.0f },
				{ 2.0f,  1.5f,  5.0f, 1.0f },
				{-2.0f,  1.5f,  5.0f, 1.0f },
				{-2.0f, -1.5f, -5.0f, 1.0f },
				{ 2.0f, -1.5f, -5.0f, 1.0f },
				{ 2.0f,  1.5f, -5.0f, 1.0f },
				{-2.0f,  1.5f, -5.0f, 1.0f },
			};

			for (size_t i = 0; i < viewPoints.size(); i++) {
				Engine::Vec4 result = Camera::ViewToWorld(camera, viewPoints[i]);
				AssertUtils::glmAreEqual(result, worldPoints[i]);
			}
		}

		TEST_METHOD(ScreenToViewTest)
		{
			OrthographicCamera camera(-2.0, 2.0, -1.5f, 1.5f, -5.0f, 5.0f);
			float screenWidth = 1080.0f;
			float screenHeight = 960.0f;

			std::vector<Engine::Vec2> screenPoints = {
				{          0,            0}, // top left
				{screenWidth,            0},
				{screenWidth, screenHeight}, // bottom right
				{          0, screenHeight},
			};

			std::vector<Engine::Vec4> viewPoints = {
				{-1.0f,  1.0f, 0.0f, 1.0f},
				{ 1.0f,  1.0f, 0.0f, 1.0f},
				{ 1.0f, -1.0f, 0.0f, 1.0f},
				{-1.0f, -1.0f, 0.0f, 1.0f},
			};

			for (size_t i = 0; i < screenPoints.size(); i++) {
				Engine::Vec4 result = Camera::ScreenToView(screenPoints[i], screenWidth, screenHeight);
				AssertUtils::glmAreEqual(result, viewPoints[i]);
			}
		}
	};
}
