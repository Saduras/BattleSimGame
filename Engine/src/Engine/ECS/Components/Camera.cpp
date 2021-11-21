#include "epch.h"
#include "Camera.h"

#include "Engine/Application.h"


namespace Engine::Components
{
	Vec4 Camera::ScreenToView(Vec2 screenPoint, float screenWidth, float screenHeight)
	{
		// Note: Screen space has an inverted y-axis (top left is 0,0)
		Mat4 screenToView(1);
		screenToView = Translate(screenToView, Vec3(-1.0f, 1.0f, 0.0f));
		screenToView = Scale(screenToView, Vec3(2 / screenWidth, -2 / screenHeight, 0.0f));
		return screenToView * Vec4(screenPoint, 0.0f, 1.f);
	}

	Vec4 Camera::ViewToWorld(Camera camera, Vec4 viewPoint)
	{
		return camera.m_ProjInverse * viewPoint;
	}

	Vec4 Camera::ScreenToWorld(Camera camera, Vec2 screenPoint, float screenWidth, float screenHeight)
	{
		return ViewToWorld(camera, ScreenToView(screenPoint, screenWidth, screenHeight));
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top), m_ZNear(zNear), m_ZFar(zFar)
	{
		m_Proj = Ortho(left, right, bottom, top, m_ZNear, m_ZFar);
		m_ProjInverse = Inverse(m_Proj);
	}
}